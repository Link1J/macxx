using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using ClangSharp.Interop;

namespace meta
{
    public static class Helpers
    {
        public static T CreateInstance<T>(params object[] args)
        {
            var type = typeof(T);
            var instance = type.Assembly.CreateInstance(
                type.FullName!, false,
                BindingFlags.Instance | BindingFlags.NonPublic,
                null, args, null, null);
            return (T)instance!;
        }

        public static Version ToVersion(this CXVersion version)
        {
            return new Version(version.Major != -1 ? version.Major : 0, version.Minor != -1 ? version.Minor : 0, version.Subminor != -1 ? version.Subminor : 0);
        }


        public static string Reverse(this string s)
        {
            char[] charArray = s.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
        }


        public static int[] ToArray(this Version version)
        {
            var items = new List<int>();
            items.Add(version.Major);
            items.Add(version.Minor);
            if (version.Build != 0)
                items.Add(version.Build);
            return items.ToArray();
        }

        public static MetadataReader? GetMetadataReader(this Assembly assembly)
        {
            unsafe
            {
                if (assembly.TryGetRawMetadata(out var blob, out var length))
                {
                    return new MetadataReader(blob, length);
                }
                else
                {
                    return null;
                }
            }
        }

        public static StringHandle GetOrAddString(this MetadataBuilder metadata, MetadataReader reader, StringHandle stringHandle)
        {
            return metadata.GetOrAddString(reader.GetString(stringHandle));
        }

        static object[] ToObjectArray(IEnumerable enumerableObject)
        {
            List<object> oList = new List<object>();
            foreach (object item in enumerableObject) { oList.Add(item); }
            return oList.ToArray();
        }

        public static void ConstArgument(this object arg, LiteralEncoder literalEncoder)
        {
            if (!arg.GetType().IsArray)
            {
                literalEncoder.Scalar().Constant(arg);
            }
            else
            {
                var items = (IList)arg;
                var encoder = literalEncoder.Vector().Count(items.Count);

                foreach (var value in items)
                    value.ConstArgument(encoder.AddLiteral());
            }
        }

        public static void ConstArguments(this FixedArgumentsEncoder argumentsEncoder, IList<object> arguments)
        {
            foreach (var argument in arguments)
            {
                argument.ConstArgument(argumentsEncoder.AddArgument());
            }
        }

        public static void ToType(this object arg, CustomAttributeElementTypeEncoder encoder)
        {
            var type = arg.GetType()!;
            var name = type.Name;
            if (type.IsArray)
                name = name[0..^2];

            switch (name)
            {
                case "Boolean":
                    encoder.Boolean();
                    break;
                case "Byte":
                    encoder.Byte();
                    break;
                case "Char":
                    encoder.Char();
                    break;
                case "Double":
                    encoder.Double();
                    break;
                case "Int16":
                    encoder.Int16();
                    break;
                case "Int32":
                    encoder.Int32();
                    break;
                case "Int64":
                    encoder.Int64();
                    break;
                case "SByte":
                    encoder.SByte();
                    break;
                case "Single":
                    encoder.Single();
                    break;
                case "String":
                    encoder.String();
                    break;
                case "SystemType":
                    encoder.SystemType();
                    break;
                case "UInt16":
                    encoder.UInt16();
                    break;
                case "UInt32":
                    encoder.UInt32();
                    break;
                case "UInt64":
                    encoder.UInt64();
                    break;
            }
        }
    }
}