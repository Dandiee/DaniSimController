using System.Collections.Generic;
using System.Linq;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;
using DaniHidSimController.Services.Sim;
using DaniHidSimController.ViewModels.IoComponents;

namespace DaniHidSimController.ViewModels
{
    public sealed class DeviceViewModel : BindableBase
    {
        public IReadOnlyCollection<ButtonViewModel> Buttons { get; }
        public IReadOnlyCollection<EncoderViewModel> Encoders { get; }
        public IReadOnlyCollection<PotentiometerViewModel> Potentiometers { get; }
        public IReadOnlyCollection<LedViewModel> GpioLeds { get; }
        public IReadOnlyCollection<LedViewModel> BuiltInLeds { get; }


        public DeviceViewModel(
            ISimConnectService simConnectService,
            IUsbService usbService,
            IEventAggregator eventAggregator)
        {
            _isDisconnected = !usbService.IsConnected;

            Encoders = new[]
            {
                new EncoderViewModel(simConnectService, "HDG", state => state.Analog6, 0,
                    SimEvents.HEADING_BUG_INC, SimEvents.HEADING_BUG_DEC),
                new EncoderViewModel(simConnectService, "ALT", state => state.Analog7, 1,
                    SimEvents.AP_ALT_VAR_INC, SimEvents.AP_ALT_VAR_DEC),
                new EncoderViewModel(simConnectService, "SPD", state => state.Analog8, 2,
                    SimEvents.AP_SPD_VAR_INC, SimEvents.AP_SPD_VAR_DEC),
                new EncoderViewModel(simConnectService, "VSP", state => state.Analog9, 3,
                    SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC)
            };

            Potentiometers = new[]
            {
                new PotentiometerViewModel(state => state.Analog1),
                new PotentiometerViewModel(state => state.Analog2),
                new PotentiometerViewModel(state => state.Analog3),
                new PotentiometerViewModel(state => state.Analog4),
                new PotentiometerViewModel(state => state.Analog5),
            };

            Buttons = Enumerable.Range(4, 11).Select(i => new ButtonViewModel(i)).ToList();

            BuiltInLeds = Enumerable.Range(0, 16).Select(i => new LedViewModel(false, i)).ToList();
            GpioLeds = Enumerable.Range(0, 16).Select(i => new LedViewModel(true, i)).ToList();

            var leds = BuiltInLeds.Union(GpioLeds).ToDictionary(kvp => kvp.Pin);

            var inputComponents = Encoders.Cast<IInputComponentViewModel>()
                .Union(Potentiometers)
                .Union(Buttons)
                .ToList();

            eventAggregator.GetEvent<UsbStateWrittenEvent>().Subscribe(state =>
            {
                leds[Pin.Gpio15].State = LedState.On;
                leds[Pin.Gpio14].State = state.IsSimConnectConnected.ToLedState();
                
                leds[Pin.Gpio7].State = state.IsBrakeNonZero.ToLedState();
                leds[Pin.Gpio8].State = state.IsFlapNonZero.ToLedState();

                leds[Pin.Gpio9].State = state.IsAutopilotMasterEnabled.ToLedState();
                leds[Pin.Gpio10].State = state.IsAutopilotHeadingEnabled.ToLedState();
                leds[Pin.Gpio11].State = state.IsAutopilotAltitudeEnabled.ToLedState();
                leds[Pin.Gpio12].State = state.IsAutopilotAirspeedEnabled.ToLedState();
                leds[Pin.Gpio13].State = state.IsAutopilotVerticalSpeedEnabled.ToLedState();

                leds[Pin.Gpio0].State = state.IsParkingBrakeEnabled.ToLedState();

                leds[Pin.Gpio5].State = state.IsAutothtottleEnabled.ToLedState();
                leds[Pin.Gpio4].State = state.IsAutopilotYawDamperEnabled.ToLedState();

                leds[Pin.BuiltIn8].State = state.IsLeftGearMoving ? LedState.Blink : state.IsLeftGearOut.ToLedState();
                leds[Pin.BuiltIn9].State = state.IsCenterGearMoving ? LedState.Blink : state.IsCenterGearOut.ToLedState();
                leds[Pin.BuiltIn10].State = state.IsRightGearMoving ? LedState.Blink : state.IsRightGearOut.ToLedState();
            });

            eventAggregator.GetEvent<HidStateReceivedEvent>().Subscribe(state =>
            {
                foreach (var inputComponent in inputComponents)
                {
                    inputComponent.Update(state);
                }
            });

            eventAggregator.GetEvent<UsbConnectionChangedEvent>().Subscribe(isConnected =>
            {
                IsDisconnected = !isConnected;
            });
        }

        private bool _isDisconnected;
        public bool IsDisconnected
        {
            get => _isDisconnected;
            private set => SetProperty(ref _isDisconnected, value);
        }
    }
}
