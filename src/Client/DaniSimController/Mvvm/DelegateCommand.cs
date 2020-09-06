using System;
using System.Windows.Input;

namespace DaniSimController.ViewModels
{
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
}