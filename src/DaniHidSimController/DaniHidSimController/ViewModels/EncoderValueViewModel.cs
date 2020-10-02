using System;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class EncoderValueViewModel : BindableBase
    {
        private readonly ISimConnectService _simConnectService;
        private readonly Func<short, int, uint> _mapFunction;
        private readonly SimEvents _simEvent;
        private bool _isInitialized;

        public EncoderValueViewModel(
            ISimConnectService simConnectService,
            Func<short, int, uint> mapFunction,
            SimEvents simEvent)
        {
            _simConnectService = simConnectService;
            _mapFunction = mapFunction;
            _simEvent = simEvent;
        }

        private uint _mappedValue;
        public uint MappedValue
        {
            get => _mappedValue;
            set
            {
                if (SetProperty(ref _mappedValue, value))
                {
                    _simConnectService.TransmitEvent(_simEvent, value);
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
                        MappedValue = _mapFunction(value, delta);
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
