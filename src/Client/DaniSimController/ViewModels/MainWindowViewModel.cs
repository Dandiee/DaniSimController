using System.Windows.Input;
using System.Windows.Interop;
using DaniSimController.Models;
using DaniSimController.Mvvm;
using DaniSimController.Services;

namespace DaniSimController.ViewModels
{
    public sealed class MainWindowViewModel : BindableBase
    {
        private readonly ISimVarComponent _simVarComponent;
        private readonly IEventAggregator _eventAggregator;
        
        public ICommand SetHwndSourceCommand { get; }

        public MainWindowViewModel(
            ISimVarComponent simVarComponent,
            IEventAggregator eventAggregator)
        {
            _simVarComponent = simVarComponent;
            _eventAggregator = eventAggregator;

            SetHwndSourceCommand = new DelegateCommand<HwndSource>(hwndSource => _simVarComponent.Connect(hwndSource));
        }
    }
}
