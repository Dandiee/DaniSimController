using System;
using System.Runtime.CompilerServices;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class DeviceStateViewModel : BindableBase
    {
        private readonly ISimConnectService _simConnectService;

        public DeviceStateViewModel(ISimConnectService simConnectService)
        {
            _simConnectService = simConnectService;

            Analog7 = new EncoderValueViewModel(simConnectService, SimEvents.AP_ALT_VAR_INC, SimEvents.AP_ALT_VAR_DEC);
            Analog8 = new EncoderValueViewModel(simConnectService, SimEvents.HEADING_BUG_INC, SimEvents.HEADING_BUG_DEC);
            Analog9 = new EncoderValueViewModel(simConnectService, SimEvents.AP_SPD_VAR_INC, SimEvents.AP_SPD_VAR_DEC);
            Analog10 = new EncoderValueViewModel(simConnectService, SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC);
        }

        private bool SetAndTransmitEvent<T>(ref T field, T value, Func<T, uint> parameterFactory, SimEvents simEvent,
            [CallerMemberName] string propertyName = null)
        {
            if (SetProperty(ref field, value, propertyName))
            {
                _simConnectService.TransmitEvent(simEvent, parameterFactory(value));

                return true;
            }

            return false;
        }

        private string _bytesText;
        public string BytesText
        {
            get => _bytesText;
            set => SetProperty(ref _bytesText, value);
        }

        private bool _button1;
        public bool Button1
        {
            get => _button1;
            set => SetProperty(ref _button1, value);
        }

        private bool _button2;
        public bool Button2
        {
            get => _button2;
            set => SetProperty(ref _button2, value);
        }

        private bool _button3;
        public bool Button3
        {
            get => _button3;
            set => SetProperty(ref _button3, value);
        }

        private bool _button4;
        public bool Button4
        {
            get => _button4;
            set => SetProperty(ref _button4, value);
        }

        private bool _button5;
        public bool Button5
        {
            get => _button5;
            set => SetProperty(ref _button5, value);
        }

        private bool _button6;
        public bool Button6
        {
            get => _button6;
            set => SetProperty(ref _button6, value);
        }

        private short _analog1;
        public short Analog1
        {
            get => _analog1;
            set => SetProperty(ref _analog1, value);
        }

        private short _analog2;
        public short Analog2
        {
            get => _analog2;
            set => SetProperty(ref _analog2, value);
        }

        private short _analog3;
        public short Analog3
        {
            get => _analog3;
            set => SetProperty(ref _analog3, value);
        }

        private short _analog4;
        public short Analog4
        {
            get => _analog4;
            set => SetProperty(ref _analog4, value);
        }

        private short _analog5;
        public short Analog5
        {
            get => _analog5;
            set => SetProperty(ref _analog5, value);
        }

        private short _analog6;
        public short Analog6
        {
            get => _analog6;
            set => SetProperty(ref _analog6, value);
        }

        public EncoderValueViewModel Analog7 { get; }
        public EncoderValueViewModel Analog8 { get; }
        public EncoderValueViewModel Analog9 { get; }
        public EncoderValueViewModel Analog10 { get; }

        public void Apply(DaniDeviceState state)
        {
            Button1 = (state.ButtonStates & 1) == 1;
            Button2 = (state.ButtonStates & 2) == 2;
            Button3 = (state.ButtonStates & 4) == 4;
            Button4 = (state.ButtonStates & 8) == 8;
            Button5 = (state.ButtonStates & 16) == 16;
            Button6 = (state.ButtonStates & 32) == 32;

            Analog1 = state.Analog1;

            Analog2 = state.Analog2;
            Analog3 = state.Analog3;
            Analog4 = state.Analog4;
            Analog5 = state.Analog5;
            Analog6 = state.Analog6;

            Analog7.RawValue = state.Analog7;
            Analog8.RawValue = state.Analog8;
            Analog9.RawValue = state.Analog9;
            Analog10.RawValue = state.Analog10;
        }
    }
}
