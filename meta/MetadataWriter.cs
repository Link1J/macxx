using System;
using System.IO;
using System.Linq;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Emit;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Reflection.PortableExecutable;

namespace meta
{
    class MetadataWriter
    {
        static readonly Guid winmdGuid = new("87D4DBE1-1143-4FAD-AAB3-1001F92068E6");

        readonly string assemblyName;
        readonly MetadataBuilder metadata = new();
        readonly BlobBuilder blob = new();

        readonly Dictionary<string, AssemblyReferenceHandle> assemblyRefs = new();
        readonly Dictionary<string, TypeReferenceHandle> typeRefs = new();
        readonly Dictionary<string, MemberReferenceHandle> attrCtorRef = new();

        public static void Write(string outputFilename)
        {
            var writer = new MetadataWriter(outputFilename);

            foreach (var item in Framework.All)
                writer.Generate(item);

            writer.WritePeFile();
        }

        string Filename => assemblyName + ".winmd";

        MetadataWriter(string assemblyName)
        {
            this.assemblyName = assemblyName;
            metadata.AddModule(
                0,
                metadata.GetOrAddString(Filename),
                metadata.GetOrAddGuid(winmdGuid),
                default, default
            );
            metadata.AddAssembly(
                metadata.GetOrAddString(assemblyName),
                version: new Version(1, 0, 0, 0),
                culture: default,
                publicKey: default,
                flags: 0,
                hashAlgorithm: AssemblyHashAlgorithm.None
            );

            AddAssemblyReference("mscorlib", new Version(4, 0, 0, 0), new byte[] { 0xB7, 0x7A, 0x5C, 0x56, 0x19, 0x34, 0xE0, 0x89 });
            AddAssemblyReference(typeof(macxx.Interop.AvailabilityAttribute).Assembly);

            AddTypeReference(assemblyRefs["mscorlib"], "System", "Object");

            var interopMeta = typeof(macxx.Interop.AvailabilityAttribute).Assembly.GetMetadataReader()!;
            foreach (var handle in interopMeta.MethodDefinitions)
            {
                var method = interopMeta.GetMethodDefinition(handle);
                if (interopMeta.GetString(method.Name) != ".ctor")
                    continue;

                var type = interopMeta.GetTypeDefinition(method.GetDeclaringType());
                var space = interopMeta.GetString(type.Namespace);
                var name = interopMeta.GetString(type.Name);
                AddTypeReference(assemblyRefs["macxx.Interop"], space, name);
                AddAttrCtor(space + "." + name, interopMeta.GetBlobBytes(method.Signature));
            }
        }

        AssemblyReferenceHandle AddAssemblyReference(string name, Version version, byte[] publicKeyOrToken)
        {
            var assemblyRef = metadata.AddAssemblyReference(
                name: metadata.GetOrAddString(name),
                version: version,
                culture: default,
                publicKeyOrToken: metadata.GetOrAddBlob(publicKeyOrToken),
                flags: default,
                hashValue: default
            );
            assemblyRefs[name] = assemblyRef;
            return assemblyRef;
        }

        AssemblyReferenceHandle AddAssemblyReference(Assembly assembly)
        {
            var name = assembly.GetName()!;
            return AddAssemblyReference(name.Name!, name.Version!, name.GetPublicKeyToken()!);
        }

        TypeReferenceHandle AddTypeReference(EntityHandle resolutionScope, string @namespace, string @name)
        {
            var typeRef = metadata.AddTypeReference(resolutionScope, metadata.GetOrAddString(@namespace), metadata.GetOrAddString(@name));
            typeRefs[@namespace + "." + @name] = typeRef;
            return typeRef;
        }

        MemberReferenceHandle AddAttrCtor(string type, byte[] sig)
        {
            var attrCtor = metadata.AddMemberReference(typeRefs[type], metadata.GetOrAddString(".ctor"), metadata.GetOrAddBlob(sig));
            attrCtorRef[type] = attrCtor;
            return attrCtor;
        }

        void Generate(Framework framework)
        {
            foreach (var protocol in framework.Protocols)
                Generate(protocol);

            foreach (var interf in framework.Interfaces)
                Generate(interf);
        }

        void Generate(Protocol protocol)
        {
            if (!protocol.typeDefinitionHandle.IsNil)
                return;

            protocol.typeDefinitionHandle = metadata.AddTypeDefinition(
                TypeAttributes.Interface | TypeAttributes.Public,
                metadata.GetOrAddString($"macos.{protocol.Framework.Name}"),
                metadata.GetOrAddString(protocol.Name),
                baseType: default,
                fieldList: MetadataTokens.FieldDefinitionHandle(1),
                methodList: Generate(protocol.InstanceMethods, protocol.ClassMethods)
            );
        }

        void Generate(Interface interf)
        {
            if (!interf.typeDefinitionHandle.IsNil)
                return;

            var baseType = default(TypeDefinitionHandle);
            if (interf.SuperClass.Length != 0)
            {
                var baseClass = Framework.FindInterface(interf.SuperClass);
                Generate(baseClass);
                baseType = baseClass.typeDefinitionHandle;
            }

            interf.typeDefinitionHandle = metadata.AddTypeDefinition(
                TypeAttributes.Class | TypeAttributes.Public,
                metadata.GetOrAddString($"macos.{interf.Framework.Name}"),
                metadata.GetOrAddString(interf.Name),
                baseType: baseType,
                fieldList: Generate(interf.InstanceProperties, interf.ClassProperties),
                methodList: Generate(interf.InstanceMethods, interf.ClassMethods)
            );
        }

        void AddAttribute(EntityHandle parent, string attrType, object[] args)
        {
            var attributeSignature = new BlobBuilder();
            new BlobEncoder(attributeSignature)
                .CustomAttributeSignature(
                    fixedArguments => fixedArguments.ConstArguments(args),
                    namedArguments => namedArguments.Count(0)
                );

            metadata.AddCustomAttribute(parent, attrCtorRef[attrType], metadata.GetOrAddBlob(attributeSignature));
        }

        void AddAttribute(EntityHandle parent, string attrType, object[] args, (string, object)[] namedArgs)
        {
            var attributeSignature = new BlobBuilder();
            new BlobEncoder(attributeSignature)
                .CustomAttributeSignature(
                    fixedArguments => fixedArguments.ConstArguments(args),
                    namedArguments =>
                    {
                        var argumentsEncoder = namedArguments.Count(namedArgs.Length);
                        foreach (var (name, value) in namedArgs)
                        {
                            argumentsEncoder.AddArgument(true, encoder =>
                            {
                                if (!value.GetType().IsArray)
                                    value.ToType(encoder.ScalarType());
                                else
                                    value.ToType(encoder.SZArray().ElementType());
                            },
                            encoder => { encoder.Name(name); },
                            encoder => { value.ConstArgument(encoder); }
                            );
                        }
                    }
                );

            metadata.AddCustomAttribute(parent, attrCtorRef[attrType], metadata.GetOrAddBlob(attributeSignature));
        }

        int nextMethod = 1;
        MethodDefinitionHandle Generate(ReadOnlyCollection<Method> a, ReadOnlyCollection<Method> b)
        {
            var methods = a.Union(b).Select(x => Generate(x)).Append(MetadataTokens.MethodDefinitionHandle(nextMethod)).ToArray();
            nextMethod += methods.Length - 1;
            return methods[0];
        }
        MethodDefinitionHandle Generate(Method method)
        {
            var signature = new BlobBuilder();
            new BlobEncoder(signature).
                MethodSignature().
                Parameters(0, returnType => returnType.Void(), parameters => { });

            var attr = MethodAttributes.Public;
            if (method.Static)
                attr |= MethodAttributes.Static;

            var def = metadata.AddMethodDefinition(
                attr,
                MethodImplAttributes.Native,
                metadata.GetOrAddString(method.Name),
                metadata.GetOrAddBlob(signature),
                -1,
                MetadataTokens.ParameterHandle(1)
            );

            foreach (var (platform, state, message, introduced, deprecated, obsoleted) in method.Availability)
            {
                var list = new List<(string, object)>();

                if (state == AvailabilityState.Unavailable)
                    list.Add(("Unavailable", true));
                if (introduced.Major != 0)
                    list.Add(("Introduced", introduced.ToArray()));
                if (deprecated.Major != 0)
                    list.Add(("Deprecated", deprecated.ToArray()));
                if (obsoleted.Major != 0)
                    list.Add(("Obsoleted", obsoleted.ToArray()));

                if (message.Length != 0)
                    list.Add(("Message", message));

                AddAttribute(def, "macxx.Interop.AvailabilityAttribute", new object[] { platform }, list.ToArray());
            }

            AddAttribute(def, "macxx.Interop.SelectorAttribute", new object[] { method.Selector });

            return def;
        }

        int nextField = 1;
        FieldDefinitionHandle Generate(ReadOnlyCollection<Property> a, ReadOnlyCollection<Property> b)
        {
            var properties = a.Union(b).Select(x => Generate(x)).Append(MetadataTokens.FieldDefinitionHandle(nextField)).ToArray();
            nextField += properties.Length - 1;
            return properties[0];
        }
        FieldDefinitionHandle Generate(Property method)
        {
            var signature = new BlobBuilder();
            new BlobEncoder(signature).FieldSignature().Object();

            var attr = FieldAttributes.Public;
            if (method.Static)
                attr |= FieldAttributes.Static;

            return metadata.AddFieldDefinition(
                attr,
                metadata.GetOrAddString(method.Name),
                metadata.GetOrAddBlob(signature)
            );
        }

        void WritePeFile()
        {
            var peHeaderBuilder = new PEHeaderBuilder();
            var peBuilder = new ManagedPEBuilder(
                peHeaderBuilder,
                new MetadataRootBuilder(metadata),
                blob,
                flags: CorFlags.ILLibrary
            );

            var peBlob = new BlobBuilder();
            peBuilder.Serialize(peBlob);
            using var peStream = new FileStream(Filename, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            peBlob.WriteContentTo(peStream);
        }
    }
}