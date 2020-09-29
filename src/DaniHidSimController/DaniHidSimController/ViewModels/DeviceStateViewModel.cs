using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class DeviceStateViewModel : BindableBase
    {
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

        private short _rz;
        public short Rz
        {
            get => _rz;
            set => SetProperty(ref _rz, value);
        }

        private short _slider;
        public short Slider
        {
            get => _slider;
            set => SetProperty(ref _slider, value);
        }
        private short _wheel;
        public short Wheel
        {
            get => _wheel;
            set => SetProperty(ref _wheel, value);
        }

        private short _dial;
        public short Dial
        {
            get => _dial;
            set => SetProperty(ref _dial, value);
        }

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
            Rz = state.Rz;
            Slider = state.Slider;
            Wheel = state.Wheel;
            Dial = state.Dial;
        }
    }
}
