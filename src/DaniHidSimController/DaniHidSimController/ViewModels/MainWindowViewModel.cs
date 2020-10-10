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

    public class DevState
    {
        public bool IsAutopilotMasterEnabled { get; set; }
        public bool IsAutopilotHeadingEnabled { get; set; }
        public bool IsAutopilotAltitudeEnabled { get; set; }
        public bool IsAutopilotAirspeedEnabled { get; set; }
        public bool IsAutopilotVerticalSpeedEnabled { get; set; }

        public bool IsLeftGearMoving { get; set; }
        public bool IsCenterGearMoving { get; set; }
        public bool IsRightGearMoving { get; set; }

        public bool IsLeftGearOut { get; set; }
        public bool IsCenterGearOut { get; set; }
        public bool IsRightGearOut { get; set; }

        public bool IsFlapNonZero { get; set; }
        public bool IsBrakeNonZero { get; set; }

        public bool IsParkingBrakeEnabled { get; set; }


        private byte byte1;
        private byte byte2;
        private byte byte3;
        private byte byte4;

        private byte[] bytes = new byte[4];

        public byte[] GetState()
        {
            var b1 = 0;

            var b2 = 0
                     | ((IsAutopilotMasterEnabled ? 1 : 0) << 0)
                     | ((IsAutopilotHeadingEnabled ? 1 : 0) << 1)
                     | ((IsAutopilotAltitudeEnabled ? 1 : 0) << 2)
                     | ((IsAutopilotAirspeedEnabled ? 1 : 0) << 3)
                     | ((IsAutopilotVerticalSpeedEnabled ? 1 : 0) << 4);

            var b3 = 0
                     | ((IsLeftGearMoving ? 1 : 0) << 0)
                     | ((IsCenterGearMoving ? 1 : 0) << 1)
                     | ((IsRightGearMoving ? 1 : 0) << 2)

                     | ((IsLeftGearOut ? 1 : 0) << 3)
                     | ((IsCenterGearOut ? 1 : 0) << 4)
                     | ((IsRightGearOut ? 1 : 0) << 5)

                     | ((IsFlapNonZero? 1 : 0) << 6)
                     | ((IsBrakeNonZero ? 1 : 0) << 7);

            var b4 = 0
                     | ((IsParkingBrakeEnabled ? 1 : 0) << 0);

            bytes[0] = (byte)b1;
            bytes[1] = (byte)b2;
            bytes[2] = (byte)b3;
            bytes[3] = (byte)b4;

            return bytes;
        }
    }

    public sealed class UsbStateWrittenEvent : PubSubEvent<DevState> { }

    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly IHidService _hidService;
        private readonly IUsbService _usbService;
        private readonly IEventAggregator _eventAggregator;

        private DevState _lastSentState;
        public DeviceStateViewModel State { get; }

        private long _lastSent = -1;

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
            _lastSentState = new DevState();

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

                case SimVars.GEAR_CENTER_POSITION:
                    var vCenter = (float)request.Get();
                    _lastSentState.IsCenterGearOut = vCenter == 1f;
                    _lastSentState.IsCenterGearMoving = vCenter > 0 && vCenter < 1f;
                    break;

                case SimVars.GEAR_LEFT_POSITION:
                    var vLeft = (float)request.Get();
                    _lastSentState.IsLeftGearOut = vLeft == 1f;
                    _lastSentState.IsLeftGearMoving = vLeft > 0 && vLeft < 1f;
                    break;

                case SimVars.GEAR_RIGHT_POSITION:
                    var vRight = (float)request.Get();
                    _lastSentState.IsRightGearOut = vRight == 1f;
                    _lastSentState.IsRightGearMoving = vRight > 0 && vRight < 1f;
                    break;


                case SimVars.FLAPS_HANDLE_PERCENT:
                    var vFlaps = (float)request.Get();
                    _lastSentState.IsFlapNonZero = vFlaps > 0;
                    break;

                case SimVars.BRAKE_LEFT_POSITION:
                    var vBrake = (float)request.Get();
                    _lastSentState.IsBrakeNonZero = vBrake > 0.01;
                    break;

                case SimVars.BRAKE_PARKING_INDICATOR:
                    _lastSentState.IsParkingBrakeEnabled = (bool) request.Get();
                    break;
            }

            var newBytes = _lastSentState.GetState();
            if (true)
            {
                _usbService.Write(newBytes);
                _eventAggregator.GetEvent<UsbStateWrittenEvent>().Publish(_lastSentState);
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
