using DaniSimController.Models;

namespace DaniSimController.ViewModels
{
    public sealed class SelectedSimVarViewModel : BindableBase
    {
        public SimVar SimVar { get; }
        public Numbers Number { get; }

        private string _value;
        public string Value
        {
            get => _value;
            set => SetProperty(ref _value, value);
        }

        private bool _isPending;
        public bool IsPending
        {
            get => _isPending;
            set => SetProperty(ref _isPending, value);
        }

        public SelectedSimVarViewModel(SimVar simVar, Numbers number)
        {
            IsPending = false;
            SimVar = simVar;
            Number = number;
        }
    }
}