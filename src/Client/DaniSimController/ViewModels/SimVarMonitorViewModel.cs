using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using DaniSimController.Mvvm;
using DaniSimController.Services;

namespace DaniSimController.ViewModels
{
    public sealed class SimVarMonitorViewModel : BindableBase
    {
        private readonly IEventAggregator _eventAggregator;
        private readonly IDictionary<Numbers, SelectedSimVarViewModel> _simVars;

        public ICommand RemoveSimVarCommand { get; }

        public ObservableCollection<SelectedSimVarViewModel> SimVars { get; }

        public SimVarMonitorViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;

            _simVars = new Dictionary<Numbers, SelectedSimVarViewModel>();

            RemoveSimVarCommand = new DelegateCommand<SelectedSimVarViewModel>(vm =>
                _eventAggregator.GetEvent<RemoveSimVarRequestEvent>().Publish(vm.Request));

            _eventAggregator.GetEvent<SimVarRequestAddedEvent>().Subscribe(SimVarRequestAdded);
            _eventAggregator.GetEvent<SimVarRequestRemovedEvent>().Subscribe(SimVarRequestRemoved);
            _eventAggregator.GetEvent<SimVarReceivedEvent>().Subscribe(SimVarReceived);

            SimVars = new ObservableCollection<SelectedSimVarViewModel>();
        }

        private void SimVarReceived(SimVarReceivedEventArgs obj)
        {
            if (_simVars.TryGetValue(obj.RequestId, out var value))
            {
                value.Value = obj.Value.ToString();
            }
        }

        private void SimVarRequestAdded(SimVarRequest simVarRequest)
        {
            var simVar = new SelectedSimVarViewModel(simVarRequest);
            _simVars.Add(simVarRequest.RequestId, simVar);
            SimVars.Add(simVar);
        }

        private void SimVarRequestRemoved(SimVarRequest simVarRequest)
        {
            _simVars.Remove(simVarRequest.RequestId);
            SimVars.Remove(SimVars.Single(s => s.Request == simVarRequest));
        }
    }

    public sealed class SelectedSimVarViewModel : BindableBase
    {
        public SimVarRequest Request { get; }

        private string _value;
        public string Value
        {
            get => _value;
            set => SetProperty(ref _value, value);
        }

        public SelectedSimVarViewModel(SimVarRequest request)
        {
            Request = request;
        }
    }
}
