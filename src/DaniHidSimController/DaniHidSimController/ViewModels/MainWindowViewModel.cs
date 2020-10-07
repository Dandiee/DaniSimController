using System;
using System.Collections.Generic;
using System.Linq;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly IHidService _hidService;
        private readonly IUsbService _usbService;
        private readonly ISimConnectService _simConnectService;
        public DeviceStateViewModel State { get; }
        
        public MainWindowViewModel(
            IHidService hidService, 
            DeviceStateViewModel deviceStateViewModel,
            IUsbService usbService,
            ISimConnectService simConnectService)
        {
            _hidService = hidService;
            _usbService = usbService;
            _simConnectService = simConnectService;

            State = deviceStateViewModel;

            _simConnectService.OnSimVarsChanged += SimConnectServiceOnOnSimVarsChanged;
            State.OnWriteBufferChanged += OnWriteBufferChanged;
        }

        private void OnWriteBufferChanged(object? sender, byte[] e)
        {
            _usbService.Write(e);
        }

        private void SimConnectServiceOnOnSimVarsChanged(object sender, SimVarRequest request)
        {
            if (request.SimVar == SimVars.AUTOPILOT_MASTER)
            {
                State.Led1 = (bool) request.Get();
            }
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
