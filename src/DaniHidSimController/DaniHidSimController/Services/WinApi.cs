using System;
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
            RAWINPUTDEVICE[] pRawInputDevice,
            uint uiNumDevices,
            uint cbSize);
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RAWINPUTHEADER
    {
        [MarshalAs(UnmanagedType.U4)] public RawInputDeviceType dwType;
        [MarshalAs(UnmanagedType.U4)] public int dwSize;
        public IntPtr hDevice;
        [MarshalAs(UnmanagedType.U4)] public int wParam;
    }

    [StructLayout(LayoutKind.Explicit, Pack = 1)]
    public struct RAWINPUT
    {
        [FieldOffset(00)] public RAWINPUTHEADER header;
        [FieldOffset(16)] public RAWHID hid;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RAWHID
    {
        [MarshalAs(UnmanagedType.U4)] public uint dwSizeHid;
        [MarshalAs(UnmanagedType.U4)] public uint dwCount;
    }

    public enum RawInputDeviceType : uint
    {
        RIM_TYPEMOUSE,
        RIM_TYPEKEYBOARD,
        RIM_TYPEHID,
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RAWINPUTDEVICE
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
        
        [FieldOffset(01)] public int ButtonStates;     // 4 byte

        [FieldOffset(05)] public short Analog1; // 2 byte
        [FieldOffset(07)] public short Analog2; // 2 byte
        [FieldOffset(09)] public short Analog3; // 2 byte
        [FieldOffset(11)] public short Analog4; // 2 byte
        [FieldOffset(13)] public short Analog5; // 2 byte
        [FieldOffset(15)] public short Analog6; // 2 byte
        [FieldOffset(17)] public short Analog7; // 2 byte
        [FieldOffset(19)] public short Analog8; // 2 byte
        [FieldOffset(21)] public short Analog9; // 2 byte => 9*2 = 18 bytes
        [FieldOffset(23)] public short Analog10; // 2 byte => 9*2 = 18 bytes

        [FieldOffset(25)] public byte Dpads;          // 1 byte
    }
}
