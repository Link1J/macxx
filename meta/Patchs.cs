using System;
using ClangSharp;
using ClangSharp.Interop;
using HarmonyLib;

namespace meta
{
    [HarmonyPatch]
    public class Patch
    {
        public static void Init()
        {
            var harmony = new Harmony("me.link1j.macxx.meta");
            Harmony.DEBUG = true;
            harmony.PatchAll();
        }

        [HarmonyPrefix]
        [HarmonyPatch(typeof(ClangSharp.Decl), "Create")]
        static bool Prefix1(ref Decl __result, CXCursor handle)
        {
            switch (handle.DeclKind)
            {
                default:
                    return true;
                case CX_DeclKind.CX_DeclKind_ObjCProtocol:
                    __result = Helpers.CreateInstance<ObjCProtocolDecl>(handle);
                    return false;
            }
        }

        [HarmonyPrefix]
        [HarmonyPatch(typeof(Cursor), MethodType.Constructor)]
        [HarmonyPatch(new System.Type[] { typeof(CXCursor), typeof(CXCursorKind) })]
        static void Prefix2(CXCursor handle, ref CXCursorKind expectedCursorKind)
        {
            switch (handle.DeclKind)
            {
                case CX_DeclKind.CX_DeclKind_FirstTypedefName:
                    expectedCursorKind = CXCursorKind.CXCursor_TemplateTypeParameter;
                    break;
            }
        }

        [HarmonyPrefix]
        [HarmonyPatch(typeof(ClangSharp.Type), MethodType.Constructor)]
        [HarmonyPatch(new System.Type[] { typeof(CXType), typeof(CXTypeKind), typeof(CX_TypeClass) })]
        static void Prefix3(CXType handle, ref CXTypeKind expectedKind, CX_TypeClass expectedTypeClass)
        {
            switch (handle.kind)
            {
                case CXTypeKind.CXType_ObjCId:
                    expectedKind = CXTypeKind.CXType_ObjCId;
                    break;
                case CXTypeKind.CXType_ObjCInterface:
                    expectedKind = CXTypeKind.CXType_ObjCInterface;
                    break;
                case CXTypeKind.CXType_ObjCClass:
                    expectedKind = CXTypeKind.CXType_ObjCClass;
                    break;
                case CXTypeKind.CXType_ObjCSel:
                    expectedKind = CXTypeKind.CXType_ObjCSel;
                    break;
            }

            if (handle.kind != expectedKind)
            {
                throw new ArgumentOutOfRangeException(nameof(handle));
            }

            if ((handle.TypeClass == CX_TypeClass.CX_TypeClass_Invalid) || (handle.TypeClass != expectedTypeClass))
            {
                throw new ArgumentOutOfRangeException(nameof(handle));
            }
        }
    }
}