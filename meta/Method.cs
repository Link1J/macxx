using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
using ClangSharp.Interop;

namespace meta
{
    enum AvailabilityState
    {
        Introduced,
        Deprecated,
        Obsoleted,
        Unavailable,
    }

    [DebuggerDisplay("Method = {Name} ({selector})")]
    class Method
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly Type returnType;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<(string name, Type type)> parameters = new();
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string selector;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly bool constructor;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly bool @static;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly List<(string platform, AvailabilityState state, string message, Version introduced, Version deprecated, Version obsoleted)> availability = new();

        public string Name { get => selector.Split(':', 2)[0]; }
        public string Selector { get => selector; }
        public bool Constructor { get => constructor; }
        public bool Static { get => @static; }
        public ReadOnlyCollection<(string name, Type type)> Parameters { get => parameters.AsReadOnly(); }
        public ReadOnlyCollection<(string platform, AvailabilityState state, string message, Version introduced, Version deprecated, Version obsoleted)> Availability { get => availability.AsReadOnly(); }
        public Type ReturnType { get => returnType; }

        public Method(ClangSharp.ObjCMethodDecl method, bool instance)
        {
            selector = method.Name;
            @static = !instance;
            constructor = instance && selector.StartsWith("init");

            var count = method.Handle.GetPlatformAvailability(out var alwaysDeprecated, out var deprecatedMessage, out var alwaysUnavailable, out var unavailableMessage, null);
            //if (alwaysUnavailable)
            //{
            //    availability.Add(("", AvailabilityState.Unavailable, new Version(), unavailableMessage.CString));
            //}
            //else if (alwaysDeprecated)
            //{
            //    availability.Add(("", AvailabilityState.Deprecated, new Version(), deprecatedMessage.CString));
            //}
            //else
            if (count != 0)
            {
                var availabilities = new CXPlatformAvailability[count];
                method.Handle.GetPlatformAvailability(out _, out _, out _, out _, availabilities);
                foreach (var avail in availabilities)
                {
                    if (avail.Unavailable != 0)
                    {
                        availability.Add((avail.Platform.CString, AvailabilityState.Unavailable, avail.Message.CString, new Version(0, 0, 0), new Version(0, 0, 0), new Version(0, 0, 0)));
                    }
                    else
                    {
                        availability.Add((avail.Platform.CString, AvailabilityState.Introduced, "", avail.Introduced.ToVersion(), avail.Deprecated.ToVersion(), avail.Obsoleted.ToVersion()));
                    }
                }
            }

            foreach (var attr in method.Attrs)
            {
                if (attr.Kind == CX_AttrKind.CX_AttrKind_Availability)
                {
                }
            }

            returnType = Type.Create(method.ReturnType);
            foreach (var param in method.Parameters)
                parameters.Add((param.Name, Type.Create(param.Type)));
        }
    }

    [DebuggerDisplay("Property = {Name}")]
    class Property
    {
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string selector;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string? getter;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly string? setter;
        [DebuggerBrowsable(DebuggerBrowsableState.Never)]
        readonly bool @static;

        public string Name { get => selector; }
        //public readonly Type Type;
        public bool Static { get => @static; }
        public string Getter { get => getter ?? Name; }
        public string Setter { get => setter ?? "set" + Name + ":"; }

        public Property(ClangSharp.ObjCPropertyDecl prop, bool instance)
        {
            selector = prop.Name;
            @static = !instance;

            if (prop.GetterMethodDecl != null)
                getter = prop.GetterMethodDecl.Name;
            if (prop.SetterMethodDecl != null)
                setter = prop.SetterMethodDecl.Name;
        }
    }
}