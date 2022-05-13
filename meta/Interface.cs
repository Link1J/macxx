using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using ClangSharp;
using ClangSharp.Interop;
using System.Reflection.Metadata;

namespace meta
{
    [DebuggerDisplay("Interface = {name}")]
    class Interface
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name = "";
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> classMethods = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> instanceMethods = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Property> classProperties = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Property> instanceProperties = new();

        public string Name { get => name; }
        public readonly string SuperClass = "";
        public ReadOnlyCollection<Method> ClassMethods { get => classMethods.AsReadOnly(); }
        public ReadOnlyCollection<Method> InstanceMethods { get => instanceMethods.AsReadOnly(); }
        public ReadOnlyCollection<Property> ClassProperties { get => classProperties.AsReadOnly(); }
        public ReadOnlyCollection<Property> InstanceProperties { get => instanceProperties.AsReadOnly(); }
        public readonly string[] Protocols = Array.Empty<string>();
        public readonly Framework Framework;

        public TypeDefinitionHandle typeDefinitionHandle;

        public Interface(ObjCInterfaceDecl decl)
        {
            name = decl.Name;
            if (decl.SuperClass != null)
                SuperClass = decl.SuperClass.Name;

            try
            {
                Protocols = decl.CursorChildren
                    .Where(x => x.CursorKind == CXCursorKind.CXCursor_ObjCProtocolRef)
                    .Select(x => x.ToString())
                    .ToArray();
            }
            catch (Exception)
            {
            }

            foreach (var method in decl.ClassMethods)
                classMethods.Add(new(method, false));
            foreach (var method in decl.InstanceMethods)
                instanceMethods.Add(new(method, true));
            foreach (var prop in decl.ClassProperties)
                classProperties.Add(new(prop, false));
            foreach (var prop in decl.InstanceProperties)
                instanceProperties.Add(new(prop, true));

            Framework = Framework.AddToFramework(decl, this);
        }
    }

    [DebuggerDisplay("Protocol = {name}")]
    class Protocol
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> classMethods = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> instanceMethods = new();

        public string Name { get => name; }
        public ReadOnlyCollection<Method> ClassMethods { get => classMethods.AsReadOnly(); }
        public ReadOnlyCollection<Method> InstanceMethods { get => instanceMethods.AsReadOnly(); }
        public readonly string[] Protocols;
        public readonly Framework Framework;

        public TypeDefinitionHandle typeDefinitionHandle;

        public Protocol(ObjCProtocolDecl decl)
        {
            name = decl.Name;
            Protocols = decl.Protocols.Select(x => x.Name).ToArray();

            foreach (var method in decl.ClassMethods)
                classMethods.Add(new(method, false));
            foreach (var method in decl.InstanceMethods)
                instanceMethods.Add(new(method, true));

            Framework = Framework.AddToFramework(decl, this);
        }
    }

    [DebuggerDisplay("Category = {name} for {type}")]
    class Category
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string type;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> classMethods = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Method> instanceMethods = new();

        public string Name { get => name; }
        public ReadOnlyCollection<Method> ClassMethods { get => classMethods.AsReadOnly(); }
        public ReadOnlyCollection<Method> InstanceMethods { get => instanceMethods.AsReadOnly(); }

        public Category(ObjCCategoryDecl decl)
        {
            name = decl.Name;
            type = decl.ClassInterface.Name;

            foreach (var method in decl.ClassMethods)
                classMethods.Add(new(method, false));
            foreach (var method in decl.InstanceMethods)
                instanceMethods.Add(new(method, true));

            Framework.AddToFramework(decl, this);
        }
    }

    [DebuggerDisplay("Enum = {name}")]
    class Enum
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name = "";
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<string> members = new();

        public string Name { get => name; }
        public ReadOnlyCollection<string> Members { get => members.AsReadOnly(); }

        public Enum(EnumDecl decl)
        {
            if (decl.Name == "")
                return;
            if (!decl.IsCanonicalDecl)
                return;

            name = decl.Name;

            foreach (var member in decl.Enumerators)
                members.Add(member.Name);

            Framework.AddToFramework(decl, this);
        }
    }

    [DebuggerDisplay("Function = {name}")]
    class Function
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name;

        public string Name { get => name; }

        public Function(FunctionDecl decl)
        {
            name = decl.Name;

            Framework.AddToFramework(decl, this);
        }
    }
}