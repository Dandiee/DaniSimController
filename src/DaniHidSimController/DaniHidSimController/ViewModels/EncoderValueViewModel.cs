using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class EncoderValueViewModel : BindableBase
    {
        private readonly ISimConnectService _simConnectService;
        private readonly SimEvents _increaseEvent;
        private readonly SimEvents _decreaseEvent;
        private bool _isInitialized;

        public EncoderValueViewModel(
            ISimConnectService simConnectService,
            SimEvents increaseEvent,
            SimEvents decreaseEvent)
        {
            _simConnectService = simConnectService;
            _increaseEvent = increaseEvent;
            _decreaseEvent = decreaseEvent;
        }

        private uint _mappedValue;
        public uint MappedValue
        {
            get => _mappedValue;
            set
            {
                if (SetProperty(ref _mappedValue, value))
                {
                    _simConnectService.TransmitEvent(_increaseEvent, value);
                }
            }
        }

        private short _rawValue;
        public short RawValue
        {
            get => _rawValue;
            set
            {
                var originalValue = _rawValue;

                if (SetProperty(ref _rawValue, value))
                {
                    if (_isInitialized)
                    {
                        var delta = value - originalValue;
                        if (delta > 0)
                        {
                            _simConnectService.TransmitEvent(_increaseEvent, 0);
                        }
                        else
                        {
                            _simConnectService.TransmitEvent(_decreaseEvent, 0);
                        }
                    }
                    else
                    {
                        _isInitialized = true;
                    }

                    _rawValue = value;
                }
            }
        }
    }
}
