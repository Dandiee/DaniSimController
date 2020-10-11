using System;
using System.Linq.Expressions;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class EncoderViewModel : BindableBase
    {
        private readonly ISimConnectService _simConnectService;
        private readonly Func<DaniDeviceState, short> _getValue;
        
        public EncoderViewModel(
            ISimConnectService simConnectService,
            string name,
            Expression<Func<DaniDeviceState, short>> getValueExpression,
            int buttonIndex,
            SimEvents increaseEvent,
            SimEvents decreaseEvent)
        {
            _simConnectService = simConnectService;
            _getValue = getValueExpression.Compile();
            
            IncreaseEvent = increaseEvent;
            DecreaseEvent = decreaseEvent;
            InputName = (getValueExpression.Body as MemberExpression).Member.Name;
            ButtonIndex = buttonIndex;
            Name = name;
        }

        public SimEvents IncreaseEvent { get; }
        public SimEvents DecreaseEvent { get; }
        public int ButtonIndex { get; }
        public string Name { get; }
        public string InputName { get; }


        private bool _isInitialized;
        public bool IsInitialized
        {
            get => _isInitialized;
            private set => SetProperty(ref _isInitialized, value);
        }

        private bool _isPressed;
        public bool IsPressed
        {
            get => _isPressed;
            private set => SetProperty(ref _isPressed, value);
        }

        private short _rawValue;
        public short RawValue
        {
            get => _rawValue;
            private set
            {
                var originalValue = _rawValue;

                if (SetProperty(ref _rawValue, value))
                {
                    if (IsInitialized)
                    {
                        var delta = value - originalValue;
                        if (delta > 0)
                        {
                            _simConnectService.TransmitEvent(IncreaseEvent, 0);
                        }
                        else
                        {
                            _simConnectService.TransmitEvent(DecreaseEvent, 0);
                        }
                    }
                    else
                    {
                        IsInitialized = true;
                    }

                    _rawValue = value;
                }
            }
        }

        public void Update(DaniDeviceState state)
        {
            RawValue = _getValue(state);
            var pow = (int)Math.Pow(2, ButtonIndex);
            IsPressed = (state.ButtonStates & pow) == pow;
        }
    }
}
