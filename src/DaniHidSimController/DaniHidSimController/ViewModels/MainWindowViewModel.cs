using System;
using System.Runtime.InteropServices;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    [StructLayout(LayoutKind.Explicit, Size = 16)]
    public struct DeviceWriteState
    {
        [FieldOffset(0)]
        public unsafe fixed byte ByteArray[2];

        [FieldOffset(0)]
        public bool IsAutopilotMasterEnabled;

        [FieldOffset(1)]
        public bool IsAutopilotHeadingEnabled;

        [FieldOffset(2)]
        public bool IsAutopilotAltitudeEnabled;

        [FieldOffset(3)]
        public bool IsAutopilotAirspeedEnabled;

        [FieldOffset(4)]
        public bool IsAutopilotVerticalSpeedEnabled;
    }

    public sealed class UsbStateWrittenEvent : PubSubEvent<DeviceWriteState> { }

    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly IHidService _hidService;
        private readonly IUsbService _usbService;
        private readonly IEventAggregator _eventAggregator;

        private DeviceWriteState _lastSentState;
        public DeviceStateViewModel State { get; }

        public MainWindowViewModel(
            IHidService hidService,
            DeviceStateViewModel deviceStateViewModel,
            IUsbService usbService,
            IEventAggregator eventAggregator)
        {
            _hidService = hidService;
            _usbService = usbService;
            _eventAggregator = eventAggregator;

            State = deviceStateViewModel;
            _lastSentState = new DeviceWriteState();

            eventAggregator.GetEvent<SimVarReceivedEvent>().Subscribe(OnSimVarReceived);
        }

        private unsafe void OnSimVarReceived(SimVarRequest request)
        {
            switch (request.SimVar)
            {
                case SimVars.AUTOPILOT_MASTER:
                    _lastSentState.IsAutopilotMasterEnabled = (bool)request.Get();
                    break;

                case SimVars.AUTOPILOT_HEADING_LOCK:
                    _lastSentState.IsAutopilotHeadingEnabled = (bool)request.Get();
                    break;

                case SimVars.AUTOPILOT_AIRSPEED_HOLD:
                    _lastSentState.IsAutopilotAirspeedEnabled = (bool)request.Get();
                    break;

                case SimVars.AUTOPILOT_VERTICAL_HOLD:
                    _lastSentState.IsAutopilotVerticalSpeedEnabled = (bool)request.Get();
                    break;
            }

            var bytes = new byte[2];

            fixed (byte* buffer = _lastSentState.ByteArray)
            {
                int index = 0;
                for (byte* i = buffer; *i != 0; i++)
                {
                    bytes[index++] = *i;
                }
            }

            bytes[0] = (byte) (
                (_lastSentState.IsAutopilotMasterEnabled ? 1 : 0) << 0 |
                (_lastSentState.IsAutopilotHeadingEnabled ? 1 : 0) << 1 |
                (_lastSentState.IsAutopilotAltitudeEnabled ? 1 : 0) << 2 |
                (_lastSentState.IsAutopilotAirspeedEnabled ? 1 : 0) << 3 |
                (_lastSentState.IsAutopilotVerticalSpeedEnabled ? 1 : 0) << 4);

            _usbService.Write(bytes);
            _eventAggregator.GetEvent<UsbStateWrittenEvent>().Publish(_lastSentState);
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
