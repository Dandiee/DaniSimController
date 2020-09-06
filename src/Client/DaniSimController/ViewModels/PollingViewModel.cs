using System.Windows.Input;
using DaniSimController.Mvvm;
using DaniSimController.Services;

namespace DaniSimController.ViewModels
{
    public sealed class PollingViewModel : BindableBase
    {
        private readonly IPoller _poller;
        private readonly IEventAggregator _eventAggregator;

        public ICommand PausePlayCommand { get; }

        public PollingViewModel(
            IPoller poller,
            IEventAggregator eventAggregator)
        {
            _poller = poller;
            _eventAggregator = eventAggregator;

            _poller.OnStateChanged += (_, __) => IsPolling = _poller.IsPolling;

            PausePlayCommand = new DelegateCommand(() =>
            {
                if (_poller.IsPolling)
                {
                    _poller.Stop();
                }
                else _poller.Start();

                IsPolling = _poller.IsPolling;
            });
        }

        private int _intervalInMs = 300;
        public int IntervalInMs
        {
            get => _intervalInMs;
            set
            {
                if (SetProperty(ref _intervalInMs, value))
                {
                    _poller.SetInterval(value);
                }
            }
        }

        private bool _isPolling;
        public bool IsPolling
        {
            get => _isPolling;
            set => SetProperty(ref _isPolling, value);
        }
    }
}
