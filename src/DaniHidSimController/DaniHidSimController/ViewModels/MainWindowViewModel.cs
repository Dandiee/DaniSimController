using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Input;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using DaniHidSimController.Services.Sim;
using Microsoft.Extensions.Options;

namespace DaniHidSimController.ViewModels
{
    public sealed class UsbStateWrittenEvent : PubSubEvent<UsbWriteState> { }
    public sealed class MainWindowViewModel : BindableBase
    {
        private const float Epsilon = 0.001f;

        private static readonly IReadOnlyDictionary<SimVars, Action<SimVarRequest, UsbWriteState>> Callbacks =
            new Dictionary<SimVars, Action<SimVarRequest, UsbWriteState>>
            {
                [SimVars.AUTOPILOT_MASTER] = (request, state)
                    => state.IsAutopilotMasterEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_HEADING_LOCK] = (request, state)
                    => state.IsAutopilotHeadingEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_ALTITUDE_LOCK] = (request, state)
                    => state.IsAutopilotAltitudeEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_AIRSPEED_HOLD] = (request, state)
                    => state.IsAutopilotAirspeedEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_VERTICAL_HOLD] = (request, state)
                    => state.IsAutopilotVerticalSpeedEnabled = (bool)request.Get(),

                [SimVars.FLAPS_HANDLE_PERCENT] = (request, state)
                    => state.IsFlapNonZero = (float)request.Get() > Epsilon,

                [SimVars.BRAKE_LEFT_POSITION] = (request, state)
                    => state.IsBrakeNonZero = (float)request.Get() > Epsilon,

                [SimVars.BRAKE_PARKING_INDICATOR] = (request, state)
                    => state.IsParkingBrakeEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_THROTTLE_ARM] = (request, state)
                    => state.IsAutothtottleEnabled = (bool)request.Get(),

                [SimVars.AUTOPILOT_YAW_DAMPER] = (request, state)
                    => state.IsAutopilotYawDamperEnabled = (bool)request.Get(),

                [SimVars.GEAR_CENTER_POSITION] = (request, state) =>
                {
                    var value = (float)request.Get();
                    state.IsCenterGearOut = Math.Abs(value - 1f) < Epsilon;
                    state.IsCenterGearMoving = value > 0 && value < 1f;
                },

                [SimVars.GEAR_LEFT_POSITION] = (request, state) =>
                {
                    var value = (float)request.Get();
                    state.IsLeftGearOut = Math.Abs(value - 1f) < Epsilon;
                    state.IsLeftGearMoving = value > 0 && value < 1f;
                },

                [SimVars.GEAR_RIGHT_POSITION] = (request, state) =>
                {
                    var value = (float)request.Get();
                    state.IsRightGearOut = Math.Abs(value - 1f) < Epsilon;
                    state.IsRightGearMoving = value > 0 && value < 1f;
                },
            };

        private readonly IUsbService _usbService;
        private readonly UsbWriteState _lastSentState;

        public ICommand OpenMapCommand { get; }
        public ICommand ClosingCommand { get; }

        public MainWindowViewModel(
            IUsbService usbService,
            IOptions<SimOptions> options,
            IEventAggregator eventAggregator)
        {
            _usbService = usbService;

            if (string.IsNullOrEmpty(options.Value.BingMapCredentialsProvider))
                throw new ArgumentException();

            _lastSentState = new UsbWriteState();

            eventAggregator.GetEvent<SimVarReceivedEvent>().Subscribe(OnSimVarReceived);
            eventAggregator.GetEvent<SimConnectConnectionChangedEvent>().Subscribe(OnSimConnectConnectionChanged);
            eventAggregator.GetEvent<UsbConnectionChangedEvent>().Subscribe(OnUsbConnectionChanged);

            ClosingCommand = new DelegateCommand(Closing);
            OpenMapCommand = new DelegateCommand(OpenMap);
        }

        private void Closing()
            => WriteDelta(state =>
            {
                state.IsSimConnectConnected = false;
                state.IsDaniClientConnected = false;
            });

        private void OnSimConnectConnectionChanged(bool isConnected)
            => WriteDelta(state => { state.IsSimConnectConnected = isConnected; });

        private void OnUsbConnectionChanged(bool isConnected)
            => WriteDelta(state => { state.IsDaniClientConnected = isConnected; });

        private void OnSimVarReceived(SimVarRequest request)
        {
            if (Callbacks.TryGetValue(request.SimVar, out var callback))
            {
                WriteDelta(state => callback(request, state));
            }
        }

        private void WriteDelta(Action<UsbWriteState> changes)
        {
            changes(_lastSentState);
            _usbService.Write(_lastSentState);
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
    }
}
