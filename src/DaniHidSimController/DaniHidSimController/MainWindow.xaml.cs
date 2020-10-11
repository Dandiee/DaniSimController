using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using DaniHidSimController.Services;
using DaniHidSimController.ViewModels;

namespace DaniHidSimController
{
    public partial class MainWindow
    {
        private readonly IHidService _hidService;
        private readonly ISimConnectService _simConnectService;

        public MainWindow(
            MainWindowViewModel viewModel,
            IHidService hidService,
            ISimConnectService simConnectService)
        {
            _hidService = hidService;
            _simConnectService = simConnectService;
            DataContext = viewModel;

            InitializeComponent();
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            var handle = new WindowInteropHelper(this).Handle;
            var source = HwndSource.FromHwnd(handle);

            source.AddHook(_hidService.WndProc);
            source.AddHook(_simConnectService.WndProc);

            _simConnectService.SetHandle(source);

            WinApi.RegisterRawInputDevices(new[]
            {
                new RAWINPUTDEVICE
                {
                    usUsagePage = 1,
                    dwFlags = (RawInputDeviceFlags) uint.Parse("00000100", System.Globalization.NumberStyles.HexNumber),
                    hwndTarget = source.Handle,
                    usUsage = 4,
                }
            }, 1, (uint)Marshal.SizeOf(typeof(RAWINPUTDEVICE)));

            base.OnSourceInitialized(e);
        }
    }
}
