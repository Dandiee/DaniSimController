using System;
using System.Linq;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly IHidService _hidService;
        public DeviceStateViewModel State { get; }

        public MainWindowViewModel(
            IHidService hidService, 
            DeviceStateViewModel deviceStateViewModel)
        {
            _hidService = hidService;
            State = deviceStateViewModel;
        }

        public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == 0x00FF)
            {
                var newState = _hidService.GetDeviceState(lParam, out var bytes);
                if (newState.ReportId == 6)
                {
                    State.Apply(newState);
                }
                // State.BytesText = string.Join("\r\n", bytes.Select(s => Convert.ToString(s, 2).PadLeft(8, '0')));

                handled = true;
            }

            return IntPtr.Zero;
        }
    }
}
