using System;
using ClangSharp;
using ClangSharp.Interop;

namespace meta
{
    public class Type
    {
        public static Type Create(ClangSharp.Type type)
        {
            if (type is ObjCObjectPointerType data)
                return new ObjectiveCObject_Type(data);
            return new Type();
        }
    }

    class ObjectiveCObject_Type : Type
    {
        public ObjectiveCObject_Type(ObjCObjectPointerType type)
        {
        }
    }
}