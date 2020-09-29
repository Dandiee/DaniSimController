using System;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly IHidService _hidService;
        public DeviceStateViewModel State { get; }

        public MainWindowViewModel(IHidService hidService)
        {
            _hidService = hidService;
            State = new DeviceStateViewModel();
        }

        public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == 0x00FF)
            {
                var newState = _hidService.GetDeviceState(lParam);
                State.Apply(newState);
                handled = true;
            }

            return IntPtr.Zero;
        }
    }
}
