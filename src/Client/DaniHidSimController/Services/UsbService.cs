using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Threading.Tasks;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using DaniHidSimController.ViewModels;
using Microsoft.Extensions.Options;

namespace DaniHidSimController.Services
{
    public interface IUsbService
    {
        bool IsConnected { get; }
        void Write(UsbWriteState state);
    }

    public sealed class UsbConnectionChangedEvent : PubSubEvent<bool> { }
    public sealed class UsbService : IUsbService
    {
        private readonly IEventAggregator _eventAggregator;
        private readonly SimOptions _options;
        private readonly string _searchQuery;
        private long _lastWrittenState;

        public bool IsConnected { get; private set; }
        private SerialPort _serialPort;

        public UsbService(
            IEventAggregator eventAggregator,
            IOptions<SimOptions> options)
        {
            _eventAggregator = eventAggregator;
            _options = options.Value;
            _searchQuery = $"SELECT * FROM WIN32_SerialPort WHERE Description = '{options.Value.DeviceName}'";
            StartConnect();
        }

        public void Write(UsbWriteState state)
        {
            var newState = state.GetAsUlong();
            if (IsConnected && _lastWrittenState != newState)
            {
                try
                {
                    var data = state.GetState();
                    _serialPort.Write(data, 0, data.Length);
                    _lastWrittenState = newState;
                    _eventAggregator.GetEvent<UsbStateWrittenEvent>().Publish(state);
                }
                catch
                {
                    IsConnected = false;
                    _serialPort.Dispose();
                    _eventAggregator.GetEvent<UsbConnectionChangedEvent>().Publish(false);
                    StartConnect();
                }
            }
        }

        private void StartConnect() => Task.Run(async () => await TryConnect());

        private async Task TryConnect()
        {
            if (TryGetDeviceId(out var deviceId))
            {
                try
                {
                    Connect(deviceId);
                    return;
                }
                catch
                {
                    Debug.WriteLine("USB Connection faield.");
                }
            }

            await Task.Delay(_options.UsbConnectionIntervalInMs);
            await TryConnect();
        }

        private void Connect(string deviceId)
        {
            _serialPort = new SerialPort(deviceId, 115200);
            _serialPort.Open();
            IsConnected = true;
            _eventAggregator.GetEvent<UsbConnectionChangedEvent>().Publish(true);
        }

        private bool TryGetDeviceId(out string deviceId)
        {
            using (var searcher = new ManagementObjectSearcher(_searchQuery))
            {
                var device = searcher.Get().Cast<ManagementBaseObject>().SingleOrDefault();
                if (device != null)
                {
                    deviceId = device.Properties["DeviceID"].Value.ToString();
                    return true;
                }
            }

            deviceId = null;
            return false;
        }
    }
}
