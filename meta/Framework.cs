using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text.RegularExpressions;
using System.Diagnostics;
using ClangSharp;

namespace meta
{
    [DebuggerDisplay("Framework = {name}")]
    class Framework
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        static readonly List<Framework> frameworks = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        static readonly Dictionary<string, Framework> frameworks_map = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        static readonly Dictionary<string, Interface> interface_map = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        static readonly Dictionary<string, Protocol> protocol_map = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        static readonly Dictionary<string, Enum> enum_map = new();

        static string GetFrameworkName(Cursor cursor)
        {
            var framework_name = "<ObjCRuntime>";
            cursor.Location.GetExpansionLocation(out var file, out _, out _, out _);
            var regex = Regex.Match(file.Name.CString, @"[\\/]([^.\\/]*?)(\.framework)[\\/]", RegexOptions.IgnoreCase);
            if (regex.Captures.Count != 0)
            {
                framework_name = regex.Captures[0].Value;
                framework_name = framework_name[1..framework_name.IndexOf('.')];
            }

            if (!frameworks_map.ContainsKey(framework_name))
            {
                var newFramework = new Framework(framework_name);
                frameworks.Add(newFramework);
                frameworks_map.Add(framework_name, newFramework);
            }

            return framework_name;
        }

        public static Framework AddToFramework(Cursor cursor, Interface interf)
        {
            var framework = frameworks_map[GetFrameworkName(cursor)];
            framework.interfaces.Add(interf);
            interface_map.Add(interf.Name, interf);
            return framework;
        }

        public static Framework AddToFramework(Cursor cursor, Protocol interf)
        {
            var framework = frameworks_map[GetFrameworkName(cursor)];
            framework.protocols.Add(interf);
            protocol_map.Add(interf.Name, interf);
            return framework;
        }

        public static Framework AddToFramework(Cursor cursor, Category interf)
        {
            var framework = frameworks_map[GetFrameworkName(cursor)];
            framework.categories.Add(interf);
            return framework;
        }

        public static Framework AddToFramework(Cursor cursor, Enum interf)
        {
            var framework = frameworks_map[GetFrameworkName(cursor)];
            framework.enums.Add(interf);
            enum_map.Add(interf.Name, interf);
            return framework;
        }

        public static Framework AddToFramework(Cursor cursor, Function interf)
        {
            var framework = frameworks_map[GetFrameworkName(cursor)];
            framework.functions.Add(interf);
            return framework;
        }

        public static ReadOnlyCollection<Framework> All { get => frameworks.AsReadOnly(); }

        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string name;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Interface> interfaces = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Protocol> protocols = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Category> categories = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Enum> enums = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<Function> functions = new();

        public string Name { get => name; }
        public ReadOnlyCollection<Interface> Interfaces { get => interfaces.AsReadOnly(); }
        public ReadOnlyCollection<Protocol> Protocols { get => protocols.AsReadOnly(); }
        public ReadOnlyCollection<Category> Categories { get => categories.AsReadOnly(); }
        public ReadOnlyCollection<Enum> Enums { get => enums.AsReadOnly(); }
        public ReadOnlyCollection<Function> Functions { get => functions.AsReadOnly(); }

        public static Protocol FindProtocol(string name) => protocol_map[name];
        public static Interface FindInterface(string name) => interface_map[name];

        public Framework(string name)
        {
            this.name = name;
        }

        public Framework()
        {
            name = "";
        }
    }
}