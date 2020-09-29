using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace DaniHidSimController.Services
{
    public interface IHidService
    {
        DaniDeviceState GetDeviceState(IntPtr rawInputHandle);
    }

    public sealed class HidService : IHidService
    {
        public DaniDeviceState GetDeviceState(IntPtr rawInputHandle)
        {
            var dwSize = 0u;
            var nativeBuffer = IntPtr.Zero;
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                (uint)Marshal.SizeOf(typeof(RAWINPUT)));

            nativeBuffer = Marshal.AllocHGlobal((int)dwSize);
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                (uint)Marshal.SizeOf(typeof(RAWINPUTHEADER)));

            byte[] managedArray = new byte[dwSize];
            Marshal.Copy(nativeBuffer, managedArray, 0, (int)dwSize);

            return Marshal.PtrToStructure<DaniDeviceState>(IntPtr.Add(nativeBuffer, Marshal.SizeOf<RAWINPUT>()));
        }
    }
}
