using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;

namespace DaniSimController.ViewModels
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

    public sealed class DelegateCommand : ICommand
    {
        private readonly Action _callback;

        public DelegateCommand(Action action)
        {
            _callback = action;
        }

        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter) => _callback?.Invoke();

        public event EventHandler CanExecuteChanged;
    }

    public sealed class DelegateCommand<T> : ICommand
    {
        private readonly Action<T> _callback;

        public DelegateCommand(Action<T> action)
        {
            _callback = action;
        }

        public bool CanExecute(object parameter) => true;

        public void Execute(object parameter) => _callback?.Invoke((T)parameter);

        public event EventHandler CanExecuteChanged;
    }
}
