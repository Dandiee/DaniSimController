using System;
using System.Runtime.CompilerServices;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class DeviceStateViewModel : BindableBase
    {
        private readonly ISimConnectService _simConnectService;
        public readonly byte[] WriteBuffer;

        public EventHandler<byte[]> OnWriteBufferChanged;

        public DeviceStateViewModel(ISimConnectService simConnectService)
        {
            _simConnectService = simConnectService;

            Analog5 = new EncoderValueViewModel(simConnectService, SimEvents.AP_ALT_VAR_INC, SimEvents.AP_ALT_VAR_DEC);
            Analog6 = new EncoderValueViewModel(simConnectService, SimEvents.HEADING_BUG_INC, SimEvents.HEADING_BUG_DEC);
            Analog7 = new EncoderValueViewModel(simConnectService, SimEvents.AP_SPD_VAR_INC, SimEvents.AP_SPD_VAR_DEC);
            Analog8 = new EncoderValueViewModel(simConnectService, SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC);
            Analog9 = new EncoderValueViewModel(simConnectService, SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC);
            Analog10 = new EncoderValueViewModel(simConnectService, SimEvents.AP_VS_VAR_INC, SimEvents.AP_VS_VAR_DEC);

            WriteBuffer = new byte[16];
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

        private bool _led1;
        public bool Led1
        {
            get => _led1;
            set => Write(ref _led1, value, 0);
        }

        private bool _led2;
        public bool Led2
        {
            get => _led2;
            set => Write(ref _led2, value, 0);
        }

        private bool _led3;
        public bool Led3
        {
            get => _led3;
            set => Write(ref _led3, value, 0);
        }

        private bool _led4;
        public bool Led4
        {
            get => _led4;
            set => Write(ref _led4, value, 0);
        }

        private bool _led5;
        public bool Led5
        {
            get => _led5;
            set => Write(ref _led5, value, 0);
        }

        private bool _led6;
        public bool Led6
        {
            get => _led6;
            set => Write(ref _led6, value, 0);
        }

        private bool _led7;
        public bool Led7
        {
            get => _led7;
            set => Write(ref _led7, value, 0);
        }

        private bool Write(ref bool led, bool value, int index, [CallerMemberName]string propertyName = null)
        {
            if (SetProperty(ref led, value, propertyName))
            {
                var leds = (byte) (byte.MinValue |
                                   (Led1 ? 1 : 0) |
                                   (Led2 ? 2 : 0) |
                                   (Led3 ? 4 : 0) |
                                   (Led4 ? 8 : 0) |
                                   (Led5 ? 16 : 0) |
                                   (Led6 ? 32 : 0) |
                                   (Led7 ? 64 : 0));

                var bytes = new byte[]
                {
                    leds
                };

                OnWriteBufferChanged?.Invoke(this, bytes);

                return true;
            }

            return false;
        }


        public EncoderValueViewModel Analog5 { get; }
        public EncoderValueViewModel Analog6 { get; }

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

            Analog5.RawValue = state.Analog5;
            Analog6.RawValue = state.Analog6;
            Analog7.RawValue = state.Analog7;
            Analog8.RawValue = state.Analog8;
            Analog9.RawValue = state.Analog9;
            Analog10.RawValue = state.Analog10;
        }
    }

    public sealed class LedValueViewModel : BindableBase
    {
        private readonly DeviceStateViewModel _device;
        public int Index { get; }

        public LedValueViewModel(int index, DeviceStateViewModel device)
        {
            _device = device;
            Index = index;
        }

        private bool _isActive;
        public bool IsActive
        {
            get => _isActive;
            set
            {
                if(SetProperty(ref _isActive, value))
                {
                    //_device.WriteBuffer[Index] = _isActive;
                }
            }

        }
    }
}
