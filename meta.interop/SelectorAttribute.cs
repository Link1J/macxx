using System;

namespace macxx.Interop
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class SelectorAttribute : Attribute
    {
        public string Selector;

        public SelectorAttribute(string selector)
        {
            Selector = selector;
        }
    }
}
