using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class DeviceStateViewModel : BindableBase
    {
        public DeviceStateViewModel(
            ISimConnectService simConnectService,
            IEventAggregator eventAggregator)
        {
            Analog6 = new EncoderViewModel(simConnectService, "HDG", state => state.Analog6, 0,
                SimEvents.HEADING_BUG_INC, SimEvents.HEADING_BUG_DEC);
            Analog7 = new EncoderViewModel(simConnectService, "ALT", state => state.Analog7, 1,
                SimEvents.AP_ALT_VAR_INC, SimEvents.AP_ALT_VAR_DEC);
            Analog8 = new EncoderViewModel(simConnectService, "SPD", state => state.Analog8, 2,
                SimEvents.AP_SPD_VAR_INC, SimEvents.AP_SPD_VAR_DEC);
            Analog9 = new EncoderViewModel(simConnectService, "VSP", state => state.Analog9, 3,
                SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC);

            Analog1 = new PotentiometerViewModel(state => state.Analog1);
            Analog2 = new PotentiometerViewModel(state => state.Analog2);
            Analog3 = new PotentiometerViewModel(state => state.Analog3);
            Analog4 = new PotentiometerViewModel(state => state.Analog4);
            Analog5 = new PotentiometerViewModel(state => state.Analog5);

            Button5  = new ButtonViewModel(4);
            Button6  = new ButtonViewModel(5);
            Button7  = new ButtonViewModel(6);
            Button8  = new ButtonViewModel(7);
            Button9  = new ButtonViewModel(8);
            Button10 = new ButtonViewModel(9);
            Button11 = new ButtonViewModel(10);
            Button12 = new ButtonViewModel(11);

            eventAggregator.GetEvent<UsbStateWrittenEvent>().Subscribe(state =>
            {     
                Led1 = state.IsAutopilotMasterEnabled;
                Led2 = state.IsAutopilotHeadingEnabled;
                Led3 = state.IsAutopilotAltitudeEnabled;
                Led4 = state.IsAutopilotAirspeedEnabled;
                Led5 = state.IsAutopilotVerticalSpeedEnabled;
            });
        }

        public ButtonViewModel Button5 { get; }
        public ButtonViewModel Button6 { get; }
        public ButtonViewModel Button7 { get; }
        public ButtonViewModel Button8 { get; }
        public ButtonViewModel Button9 { get; }
        public ButtonViewModel Button10 { get; }
        public ButtonViewModel Button11 { get; }
        public ButtonViewModel Button12 { get; }

        public PotentiometerViewModel Analog1 { get; }
        public PotentiometerViewModel Analog2 { get; }
        public PotentiometerViewModel Analog3 { get; }
        public PotentiometerViewModel Analog4 { get; }
        public PotentiometerViewModel Analog5 { get; }

        public EncoderViewModel Analog6 { get; }
        public EncoderViewModel Analog7 { get; }
        public EncoderViewModel Analog8 { get; }
        public EncoderViewModel Analog9 { get; }

        private bool _led1;
        public bool Led1
        {
            get => _led1;
            set => SetProperty(ref _led1, value);
        }

        private bool _led2;
        public bool Led2
        {
            get => _led2;
            set => SetProperty(ref _led2, value);
        }

        private bool _led3;
        public bool Led3
        {
            get => _led3;
            set => SetProperty(ref _led3, value);
        }

        private bool _led4;
        public bool Led4
        {
            get => _led4;
            set => SetProperty(ref _led4, value);
        }

        private bool _led5;
        public bool Led5
        {
            get => _led5;
            set => SetProperty(ref _led5, value);
        }

        private bool _led6;
        public bool Led6
        {
            get => _led6;
            set => SetProperty(ref _led6, value);
        }

        private bool _led7;
        public bool Led7
        {
            get => _led7;
            set => SetProperty(ref _led7, value);
        }

        

        public void Apply(DaniDeviceState state)
        {
            Button5.Update(state);
            Button6.Update(state);
            Button7.Update(state);
            Button8.Update(state);
            Button9.Update(state);
            Button10.Update(state);
            Button11.Update(state);
            Button12.Update(state);

            Analog1.Update(state);
            Analog2.Update(state);
            Analog3.Update(state);
            Analog4.Update(state);
            Analog5.Update(state);

            Analog6.Update(state);
            Analog7.Update(state);
            Analog8.Update(state);
            Analog9.Update(state);
        }
    }
}
