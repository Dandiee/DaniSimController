using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using DaniHidSimController.Services;
using DaniHidSimController.ViewModels;

namespace DaniHidSimController
{
    public partial class MainWindow
    {
        private readonly MainWindowViewModel _viewModel;
        private readonly ISimConnectService _simConnectService;

        public MainWindow(
            MainWindowViewModel viewModel,
            ISimConnectService simConnectService)
        {
            _viewModel = viewModel;
            _simConnectService = simConnectService;
            DataContext = viewModel;
            
            InitializeComponent();
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            var handle = new WindowInteropHelper(this).Handle;
            var source = HwndSource.FromHwnd(handle);

            source.AddHook(_viewModel.WndProc);
            source.AddHook(_simConnectService.WndProc);

            try
            {
                _simConnectService.SetHandle(source);
            }
            catch (Exception exception)
            {
            }
            

            WinApi.RegisterRawInputDevices(new[]
            {
                new RAWINPUTDEVICE
                {
                    usUsagePage = 1,
                    dwFlags = (RawInputDeviceFlags) uint.Parse("00000100", System.Globalization.NumberStyles.HexNumber),
                    hwndTarget = source.Handle,
                    usUsage = 4,
                }
            }, 1, (uint) Marshal.SizeOf(typeof(RAWINPUTDEVICE)));

            

            base.OnSourceInitialized(e);
        }
    }
}
