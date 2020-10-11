using System;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class ButtonViewModel : BindableBase, IInputComponentViewModel
    {
        public int ButtonIndex { get; }

        public ButtonViewModel(int buttonIndex)
        {
            ButtonIndex = buttonIndex;
        }

        private bool _isPressed;
        public bool IsPressed
        {
            get => _isPressed;
            private set => SetProperty(ref _isPressed, value);
        }
        
        public void Update(DaniDeviceState state)
        {
            var pow = (int)Math.Pow(2, ButtonIndex);
            IsPressed = (state.ButtonStates & pow) == pow;
        }
    }
}
