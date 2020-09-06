using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Data;
using System.Windows.Input;
using DaniSimController.Models;
using DaniSimController.Mvvm;
using DaniSimController.Services;

namespace DaniSimController.ViewModels
{
    public sealed class SimVarBrowserViewModel : BindableBase
    {
        public ICommand ClearTypeFilterCommand { get; }
        public ICommand SelectSimVarCommand { get; }

        public ObservableCollection<SimVar> SimVars { get; }
        public ICollectionView SimVarsCollection { get; }

        public SimVarBrowserViewModel(IEventAggregator eventAggregator)
        {
            SelectSimVarCommand =
                new DelegateCommand<SimVar>(simVar =>
                    eventAggregator.GetEvent<AddSimVarRequestEvent>().Publish(simVar));

            ClearTypeFilterCommand = new DelegateCommand(() => TypeFilter = null);

            eventAggregator.GetEvent<SimVarRequestRemovedEvent>().Subscribe(SimVarRequestRemoved);
            eventAggregator.GetEvent<SimVarRequestAddedEvent>().Subscribe(SimVarRequestAdded);

            SimVars = new ObservableCollection<SimVar>(SimVar.All);
            SimVarsCollection = new ListCollectionView(SimVars)
            {
                Filter = Filter,
            };
        }
        
        private SimVarType _typeFilter;
        public SimVarType TypeFilter
        {
            get => _typeFilter;
            set
            {
                if (SetProperty(ref _typeFilter, value))
                {
                    SimVarsCollection.Refresh();
                }
            }
        }

        private string _nameFilter;
        public string NameFilter
        {
            get => _nameFilter;
            set
            {
                if (SetProperty(ref _nameFilter, value))
                {
                    SimVarsCollection.Refresh();
                }
            }
        }

        private void SimVarRequestAdded(SimVarRequest request) => SimVars.Remove(request.SimVar);
        
        private void SimVarRequestRemoved(SimVarRequest request) => SimVars.Add(request.SimVar);
        
        private bool Filter(object obj)
            => (obj is SimVar simVar &&
                (string.IsNullOrEmpty(NameFilter) || simVar.FriendlyName.ToLowerInvariant()
                    .Contains(NameFilter.ToLowerInvariant())) &&
                (TypeFilter == null || simVar.Type == TypeFilter));
    }
}
