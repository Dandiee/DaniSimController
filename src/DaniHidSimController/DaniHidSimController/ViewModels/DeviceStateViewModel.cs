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

            Wheel = new EncoderValueViewModel(simConnectService,
                (abs, delta) => (uint)Math.Clamp((int)(Wheel.MappedValue + delta * 100), 0, 50000), SimEvents.AP_ALT_VAR_SET_ENGLISH);
            Slider = new EncoderValueViewModel(simConnectService,
                (abs, delta) =>
                {
                    if (abs < 0)
                    {
                        return (uint)(360 + (abs % (-360)));
                    }

                    return (uint) (abs % 360);
                }, SimEvents.HEADING_BUG_SET);

            Dial = new EncoderValueViewModel(simConnectService,
                (abs, delta) => (uint) Math.Clamp((int) (Dial.MappedValue + delta), 100, 400), SimEvents.AP_SPD_VAR_SET);

            Rz = new EncoderValueViewModel(simConnectService,
                (abs, delta) => (uint)Math.Clamp((int)(Dial.MappedValue + delta), 100, 400), SimEvents.AP_VS_VAR_SET_METRIC);
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

        private short _x;
        public short X
        {
            get => _x;
            set => SetProperty(ref _x, value);
        }

        private short _y;
        public short Y
        {
            get => _y;
            set => SetProperty(ref _y, value);
        }

        private short _z;
        public short Z
        {
            get => _z;
            set => SetProperty(ref _z, value);
        }

        private short _rx;
        public short Rx
        {
            get => _rx;
            set => SetProperty(ref _rx, value);
        }

        private short _ry;
        public short Ry
        {
            get => _ry;
            set => SetProperty(ref _ry, value);
        }

        public EncoderValueViewModel Rz { get; }
        public EncoderValueViewModel Slider { get; }
        public EncoderValueViewModel Wheel { get; }
        public EncoderValueViewModel Dial { get; }

        public void Apply(DaniDeviceState state)
        {
            Button1 = (state.ButtonStates & 1) == 1;
            Button2 = (state.ButtonStates & 2) == 2;
            Button3 = (state.ButtonStates & 4) == 4;
            Button4 = (state.ButtonStates & 8) == 8;
            Button5 = (state.ButtonStates & 16) == 16;
            Button6 = (state.ButtonStates & 32) == 32;

            X = state.X;

            Y = state.Y;
            Z = state.Z;
            Rx = state.Rx;
            Ry = state.Ry;

            Rz.RawValue = state.Rz;
            Slider.RawValue = state.Slider;
            Wheel.RawValue = state.Wheel;
            Dial.RawValue = state.Dial;
        }
    }
}
