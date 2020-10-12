using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace DaniHidSimController.Mvvm
{
    public abstract class BindableBase : INotifyPropertyChanged
    {
        public bool SetProperty<T>(ref T field, T value, [CallerMemberName] string propertyName = default)
        {
            if (EqualityComparer<T>.Default.Equals(value, field))
            {
                return false;
            }

            field = value;
            OnPropertyChanged(propertyName);
            return true;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
