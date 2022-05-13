using System;

namespace macxx.Interop
{
    [AttributeUsage(AttributeTargets.Struct | AttributeTargets.Interface | AttributeTargets.Method, AllowMultiple = true)]
    public class AvailabilityAttribute : Attribute
    {
        public string Platform;
        public bool Unavailable;

        public int[] Introduced;
        public int[] Deprecated;
        public int[] Obsoleted;

        public string Message;

        public AvailabilityAttribute(string platform)
        {
            Platform = platform;
        }
    }
}
