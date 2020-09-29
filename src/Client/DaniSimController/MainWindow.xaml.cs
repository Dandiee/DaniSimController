using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;
using RawInput;

namespace DaniSimController
{
    public partial class MainWindow
    {
        private readonly MainWindowViewModel _viewModel;

        public MainWindow()
        {
            InitializeComponent();

            _viewModel = App.ServiceProvider.GetService<MainWindowViewModel>();
            DataContext = _viewModel;
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            _viewModel.SetHwndSourceCommand.Execute(PresentationSource.FromVisual(this) as HwndSource);


            WindowInteropHelper helper = new WindowInteropHelper(this);
            HwndSource source = HwndSource.FromHwnd(helper.Handle);
            RAWINPUTDEVICE[] rid = new RAWINPUTDEVICE[1];

            rid[0].usUsagePage = 0x01;
            rid[0].usUsage = 0x04;
            rid[0].dwFlags = RIDEV_INPUTSINK;
            rid[0].hwndTarget = source.Handle;
            RegisterRawInputDevices(rid, (uint)rid.Length, (uint)Marshal.SizeOf(rid[0]));

            source.AddHook(WndProc);

            base.OnSourceInitialized(e);
        }

        private const int WM_INPUT = 0x00FF;
        public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == WM_INPUT)
            {
                // TODO - figure out why this doesn't halt further processing of this handled event
                handled = true;
            }
            return IntPtr.Zero;
        }

        [StructLayout(LayoutKind.Sequential)]
        internal struct RAWINPUTDEVICE
        {
            [MarshalAs(UnmanagedType.U2)]
            public ushort usUsagePage;
            [MarshalAs(UnmanagedType.U2)]
            public ushort usUsage;
            [MarshalAs(UnmanagedType.U4)]
            public int dwFlags;
            public IntPtr hwndTarget;
        }

        private const int RIDEV_INPUTSINK = 0x00000100;

        [DllImport("User32.dll")]
        extern static bool RegisterRawInputDevices(RAWINPUTDEVICE[] pRawInputDevice, uint uiNumDevices, uint cbSize);

    }
}
