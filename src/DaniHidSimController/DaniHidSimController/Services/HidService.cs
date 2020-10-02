using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace DaniHidSimController.Services
{
    public interface IHidService
    {
        DaniDeviceState GetDeviceState(IntPtr rawInputHandle, out byte[] bytes);
    }

    public sealed class HidService : IHidService
    {
        public DaniDeviceState GetDeviceState(IntPtr rawInputHandle, out byte[] bytes)
        {
            var skipper = 24u;
            var dwSize = 0u;
            var nativeBuffer = IntPtr.Zero;
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                skipper);

            nativeBuffer = Marshal.AllocHGlobal((int)dwSize);
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                skipper);

            bytes = new byte[dwSize];
            Marshal.Copy(nativeBuffer, bytes, 0, (int)dwSize);

            return Marshal.PtrToStructure<DaniDeviceState>(IntPtr.Add(nativeBuffer, (int)(dwSize - 24)));
        }
    }
}
