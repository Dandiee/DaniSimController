using System;
using System.Linq.Expressions;
using DaniHidSimController.Mvvm;
using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public sealed class PotentiometerViewModel : BindableBase
    {
        private readonly Func<DaniDeviceState, short> _getValue;
        
        public PotentiometerViewModel(Expression<Func<DaniDeviceState, short>> getValueExpression)
        {
            _getValue = getValueExpression.Compile();
            InputName = (getValueExpression.Body as MemberExpression).Member.Name;
        }

        public string InputName { get; }

        private short _rawValue;
        public short RawValue
        {
            get => _rawValue;
            private set
            {
                if (SetProperty(ref _rawValue, value))
                {
                    Percent = ((float)value / short.MaxValue) * 100;
                }
            }
        }

        private float _percent;
        public float Percent
        {
            get => _percent;
            private set => SetProperty(ref _percent, value);
        }

        public void Update(DaniDeviceState state)
        {
            RawValue = _getValue(state);
        }
    }
}
