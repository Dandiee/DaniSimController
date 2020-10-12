using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace DaniHidSimController.Services
{
    public static class WinApi
    {
        [DllImport("User32.dll", SetLastError = true)]
        public static extern uint GetRawInputData(
            IntPtr hRawInput,
            uint uiCommand,
            IntPtr pData,
            ref uint pcbSize,
            uint cbSizeHeader);


        [DllImport("User32.dll", SetLastError = true)]
        public static extern bool RegisterRawInputDevices(
            RawInputDevice[] pRawInputDevice,
            uint uiNumDevices,
            uint cbSize);
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RawInputHeader
    {
        [MarshalAs(UnmanagedType.U4)] public RawInputDeviceType dwType;
        [MarshalAs(UnmanagedType.U4)] public int dwSize;
        public IntPtr hDevice;
        [MarshalAs(UnmanagedType.U4)] public int wParam;
    }

    [StructLayout(LayoutKind.Explicit, Pack = 1)]
    public struct RawInput
    {
        [FieldOffset(00)] public RawInputHeader header;
        [FieldOffset(16)] public RawHid hid;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RawHid
    {
        [MarshalAs(UnmanagedType.U4)] public uint dwSizeHid;
        [MarshalAs(UnmanagedType.U4)] public uint dwCount;
    }

    [SuppressMessage("ReSharper", "InconsistentNaming", Justification = "WIN API defined names")]
    public enum RawInputDeviceType : uint
    {
        RIM_TYPEMOUSE,
        RIM_TYPEKEYBOARD,
        RIM_TYPEHID,
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RawInputDevice
    {
        [MarshalAs(UnmanagedType.U2)]
        public ushort usUsagePage;
        [MarshalAs(UnmanagedType.U2)]
        public ushort usUsage;
        [MarshalAs(UnmanagedType.U4)]
        public RawInputDeviceFlags dwFlags;
        public IntPtr hwndTarget;
    }


    [Flags]
    [SuppressMessage("ReSharper", "InconsistentNaming", Justification = "WIN API defined names")]
    public enum RawInputDeviceFlags : uint
    {
        RIDEV_APPKEYS = 1024, // 0x00000400
        RIDEV_CAPTUREMOUSE = 512, // 0x00000200
        RIDEV_DEVNOTIFY = 8192, // 0x00002000
        RIDEV_EXCLUDE = 16, // 0x00000010
        RIDEV_EXINPUTSINK = 4096, // 0x00001000
        RIDEV_INPUTSINK = 256, // 0x00000100
        RIDEV_NOHOTKEYS = RIDEV_CAPTUREMOUSE, // 0x00000200
        RIDEV_NOLEGACY = 48, // 0x00000030
        RIDEV_PAGEONLY = 32, // 0x00000020
        RIDEV_REMOVE = 1,
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct DaniDeviceState
    {
        [FieldOffset(00)] public byte ReportId;
        
        [FieldOffset(01)] public int ButtonStates;

        [FieldOffset(05)] public short Analog1;
        [FieldOffset(07)] public short Analog2;
        [FieldOffset(09)] public short Analog3;
        [FieldOffset(11)] public short Analog4;
        [FieldOffset(13)] public short Analog5;
        [FieldOffset(15)] public short Analog6;
        [FieldOffset(17)] public short Analog7;
        [FieldOffset(19)] public short Analog8;
        [FieldOffset(21)] public short Analog9;
        [FieldOffset(23)] public short Analog10;

        [FieldOffset(25)] public byte DPads;
    }
}
