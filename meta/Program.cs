using System;
using System.IO;
using System.Linq;
using ClangSharp;
using ClangSharp.Interop;

namespace meta
{
    class ParseAST
    {
        public static void Generate(TranslationUnitDecl cursor)
        {
            foreach (var item in cursor.CursorChildren)
                Generate((dynamic)item);
        }
        static void Generate(ObjCInterfaceDecl cursor)
        {
            new Interface(cursor);
        }
        static void Generate(ObjCProtocolDecl cursor)
        {
            new Protocol(cursor);
        }
        static void Generate(ObjCCategoryDecl cursor)
        {
            new Category(cursor);
        }
        static void Generate(EnumDecl cursor)
        {
            new Enum(cursor);
        }
        static void Generate(FunctionDecl cursor)
        {
            new Function(cursor);
        }
        static void Generate(Cursor _)
        {
        }
    }

    class MFile : IDisposable
    {
        public MFile(string macosSDK)
        {
            var imports = "#import <objc/NSObject.h>\n";
            var frameworksPath = macosSDK + "/System/Library/Frameworks/";
            foreach (var dir in Directory.EnumerateDirectories(frameworksPath))
            {
                var name = dir[frameworksPath.Length..].Split('.', 2)[0];
                if (!File.Exists(dir + $"/Headers/{name}.h"))
                    continue;
                imports += $"#import <{name}/{name}.h>\n";
            }
            File.WriteAllText("test.m", imports);
        }

        public void Dispose()
        {
            File.Delete("test.m");
        }
    }

    class Program
    {
        static readonly string macosSDK = @"R:\MacOS\MacOS SDK\MacOSX10.15.sdk";
        static readonly string clangIncludes = @"C:\Program Files\LLVM\lib\clang\12.0.0\include";
        static readonly string macosVersion = @"10.15.0";

        static readonly string[] clang_command_args = {
            "-target","x86_64-apple-macosx",
            "-isysroot",
            "-isystem",
            "-fobjc-runtime=macosx-", "-fblocks",
            "-Wall", "-Wno-unused-command-line-argument"
        };

        static TranslationUnit TryParse(ClangSharp.Index index, string filename, ReadOnlySpan<string> args, CXTranslationUnit_Flags flags)
        {
            var error = CXTranslationUnit.TryParse(index.Handle, filename, args, Array.Empty<CXUnsavedFile>(), flags, out var handle);
            if (error != CXErrorCode.CXError_Success)
            {
                Console.WriteLine($"Error: Parsing failed for '{filename}' due to '{error}'.");
                throw new Exception();
            }
            return TranslationUnit.GetOrCreate(handle);
        }

        static readonly CXTranslationUnit_Flags clang_flags = CXTranslationUnit_Flags.CXTranslationUnit_VisitImplicitAttributes;

        static TranslationUnit Parse()
        {
            using var _ = new MFile(macosSDK);
            var index = ClangSharp.Index.Create(false, true);
            return TryParse(index, "test.m", clang_command_args, clang_flags);
        }

        static void Main(string[] args)
        {
            Patch.Init();

            clang_command_args[2] += macosSDK;
            clang_command_args[3] += clangIncludes;
            clang_command_args[1] += macosVersion;
            clang_command_args[4] += macosVersion;

            var unit = Parse();
            ParseAST.Generate(unit.TranslationUnitDecl);

            MetadataWriter.Write("macos_metadata");
        }

        static void PrintInfo()
        {
            using StreamWriter sw = File.CreateText("meta.cs");

            Indent indent = new();
            sw.WriteLine(indent.Format("namespace macos {{"));
            indent++;
            foreach (var framework in Framework.All)
            {
                foreach (var enumer in framework.Enums)
                {
                    sw.WriteLine(indent.Format("enum {0} {{", enumer.Name));
                    indent++;
                    foreach (var member in enumer.Members)
                    {
                        sw.WriteLine(indent.Format("{0},", member));
                    }
                    indent--;
                    sw.WriteLine(indent.Format("}}"));
                }
                foreach (var protocol in framework.Protocols)
                {
                    if (protocol.ClassMethods.Count > 0)
                    {
                        sw.WriteLine(indent.Format("static class {0}_Static {{", protocol.Name));
                        indent++;

                        foreach (var method in protocol.ClassMethods)
                        {
                            sw.WriteLine(indent.Format(
                                "{1} {0}({2});",
                                method.Name,
                                "void",
                                string.Join(", ", method.Parameters.Select(r => $"{r.type} {r.name}"))
                            ));
                        }

                        indent--;
                        sw.WriteLine(indent.Format("}}"));
                        sw.WriteLine(indent.Format("[StaticItems(\"{0}_Static\")]", protocol.Name));
                    }

                    sw.WriteLine(indent.Format("interface {0} {{", protocol.Name));
                    indent++;

                    foreach (var method in protocol.InstanceMethods)
                    {
                        sw.WriteLine(indent.Format("[Selector(\"{0}\")]", method.Selector));
                        sw.WriteLine(indent.Format(
                            "{1} {0}({2});",
                            method.Name,
                            "void",
                            string.Join(", ", method.Parameters.Select(r => $"{r.type} {r.name}"))
                        ));
                    }

                    indent--;
                    sw.WriteLine(indent.Format("}}"));
                }
            }
            indent--;
            sw.WriteLine(indent.Format("}}"));
        }
    }

    class Indent
    {
        int index;

        public static Indent operator ++(Indent i)
        {
            i.index++;
            return i;
        }
        public static Indent operator --(Indent i)
        {
            i.index--;
            return i;
        }

        public override string ToString() => new('\t', index);
        public string Format(string format, params object[] args) => ToString() + string.Format(format, args);
    }
}
