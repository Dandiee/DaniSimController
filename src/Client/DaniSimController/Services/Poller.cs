using System;
using System.Timers;

namespace DaniSimController.Services
{
    public interface IPoller
    {
        void Start();
        void Stop();
        void SetInterval(int intervalInMs);

        bool IsPolling { get; }
        event EventHandler Tick;
        event EventHandler OnStateChanged;
    }

    public sealed class Poller : IPoller, IDisposable
    {
        private readonly Timer _timer;

        public Poller()
        {
            _timer = new Timer();
            _timer.Elapsed += (_, args) => Tick?.Invoke(this, args);
        }

        public void Start()
        {
            _timer.Enabled = true;
            OnStateChanged?.Invoke(this, null);
        }

        public void Stop()
        {
            _timer.Enabled = false;
            OnStateChanged?.Invoke(this, null);
        }

        public void SetInterval(int intervalInMs) => _timer.Interval = intervalInMs;
        public bool IsPolling => _timer.Enabled;

        public event EventHandler Tick;
        public event EventHandler OnStateChanged;

        public void Dispose()
        {
            _timer?.Dispose();
        }
    }
}
