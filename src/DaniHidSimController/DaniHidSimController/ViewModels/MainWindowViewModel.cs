using System;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Input;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using Microsoft.Extensions.Options;
using Microsoft.Maps.MapControl.WPF;
using Microsoft.Maps.MapControl.WPF.Core;

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
        public bool IsDaniClientConnected { get; set; }
        public bool IsSimConnectConnected { get; set; }

        public bool IsAutopilotMasterEnabled { get; set; }
        public bool IsAutopilotHeadingEnabled { get; set; }
        public bool IsAutopilotAltitudeEnabled { get; set; }
        public bool IsAutopilotAirspeedEnabled { get; set; }
        public bool IsAutopilotVerticalSpeedEnabled { get; set; }
        public bool IsAutopilotYawDamperEnabled { get; set; }

        public bool IsLeftGearMoving { get; set; }
        public bool IsCenterGearMoving { get; set; }
        public bool IsRightGearMoving { get; set; }

        public bool IsLeftGearOut { get; set; }
        public bool IsCenterGearOut { get; set; }
        public bool IsRightGearOut { get; set; }

        public bool IsFlapNonZero { get; set; }
        public bool IsBrakeNonZero { get; set; }

        public bool IsParkingBrakeEnabled { get; set; }

        public bool IsAutothtottleEnabled { get; set; }


        private byte byte1;
        private byte byte2;
        private byte byte3;
        private byte byte4;

        private byte[] bytes = new byte[4];

        public byte[] GetState()
        {
            var b1 = 0
                     | ((IsDaniClientConnected ? 1 : 0) << 0)
                     | ((IsSimConnectConnected ? 1 : 0) << 1);

            var b2 = 0
                     | ((IsAutopilotMasterEnabled ? 1 : 0) << 0)
                     | ((IsAutopilotHeadingEnabled ? 1 : 0) << 1)
                     | ((IsAutopilotAltitudeEnabled ? 1 : 0) << 2)
                     | ((IsAutopilotAirspeedEnabled ? 1 : 0) << 3)
                     | ((IsAutopilotVerticalSpeedEnabled ? 1 : 0) << 4)
                     | ((IsAutopilotYawDamperEnabled ? 1 : 0) << 5);

            var b3 = 0
                     | ((IsLeftGearMoving ? 1 : 0) << 0)
                     | ((IsCenterGearMoving ? 1 : 0) << 1)
                     | ((IsRightGearMoving ? 1 : 0) << 2)

                     | ((IsLeftGearOut ? 1 : 0) << 3)
                     | ((IsCenterGearOut ? 1 : 0) << 4)
                     | ((IsRightGearOut ? 1 : 0) << 5)

                     | ((IsFlapNonZero ? 1 : 0) << 6)
                     | ((IsBrakeNonZero ? 1 : 0) << 7);

            var b4 = 0
                     | ((IsParkingBrakeEnabled ? 1 : 0) << 0)
                     | ((IsAutothtottleEnabled ? 1 : 0) << 1);

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
        private readonly IOptions<SimOptions> _options;
        private readonly IEventAggregator _eventAggregator;

        private DevState _lastSentState;

        public ICommand OpenMapCommand { get; }
        public ICommand ClosingCommand { get; }

        private long _lastSent = -1;

        public MainWindowViewModel(
            IHidService hidService,
            DeviceViewModel deviceViewModel,
            IUsbService usbService,
            IOptions<SimOptions> options,
            IEventAggregator eventAggregator)
        {
            _hidService = hidService;
            _usbService = usbService;
            _options = options;
            _eventAggregator = eventAggregator;

            if (string.IsNullOrEmpty(options.Value.BingMapCredentialsProvider))
                throw new ArgumentException();

            _lastSentState = new DevState();

            eventAggregator.GetEvent<SimVarReceivedEvent>().Subscribe(OnSimVarReceived);
            eventAggregator.GetEvent<SimConnectConnectionChangedEvent>().Subscribe(OnSimConnectConnectionChanged);
            eventAggregator.GetEvent<UsbConnectionChangedEvent>().Subscribe(OnUsbConnectionChanged);

            ClosingCommand = new DelegateCommand(Closing);
            OpenMapCommand = new DelegateCommand(OpenMap);
        }

        private void OpenMap()
        {
            var window = System.Windows.Application.Current.Windows.OfType<LocationWindow>().SingleOrDefault();
            if (window == null)
            {
                window = new LocationWindow();
                window.Show();
            }

            window.Activate();
        }

        private void Closing()
        {
            _lastSentState.IsSimConnectConnected = false;
            _lastSentState.IsDaniClientConnected = false;
            WriteState();
        }

        private void OnSimConnectConnectionChanged(bool isConnected)
        {
            _lastSentState.IsSimConnectConnected = isConnected;
            WriteState();
        }

        private void OnUsbConnectionChanged(bool isConnected)
        {
            _lastSentState.IsDaniClientConnected = isConnected;
            WriteState();
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

                case SimVars.AUTOPILOT_ALTITUDE_LOCK:
                    _lastSentState.IsAutopilotAltitudeEnabled = (bool)request.Get();
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
                    _lastSentState.IsParkingBrakeEnabled = (bool)request.Get();
                    break;

                case SimVars.AUTOPILOT_THROTTLE_ARM:
                    _lastSentState.IsAutothtottleEnabled = (bool)request.Get();
                    break;

                case SimVars.AUTOPILOT_YAW_DAMPER:
                    _lastSentState.IsAutopilotYawDamperEnabled = (bool)request.Get();
                    break;
            }

            WriteState();

        }

        private void WriteState()
        {
            var newBytes = _lastSentState.GetState();
            _usbService.Write(newBytes);
            _eventAggregator.GetEvent<UsbStateWrittenEvent>().Publish(_lastSentState);
        }

    }
}
