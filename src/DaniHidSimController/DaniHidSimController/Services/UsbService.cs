using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Threading.Tasks;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using Microsoft.Extensions.Options;

namespace DaniHidSimController.Services
{
    public interface IUsbService
    {
        void Write(byte[] data);
    }

    public sealed class UsbConnectionChangedEvent : PubSubEvent<bool> { }
    public sealed class UsbService : IUsbService
    {
        private readonly IEventAggregator _eventAggregator;
        private readonly SimOptions _options;

        private bool _isConnected;
        private SerialPort _serialPort;

        public UsbService(
            IEventAggregator eventAggregator,
            IOptions<SimOptions> options)
        {
            _eventAggregator = eventAggregator;
            _options = options.Value;

            StartConnect();
        }

        public void Write(byte[] data)
        {
            if (_isConnected)
            {
                try
                {
                    _serialPort.Write(data, 0, data.Length);
                }
                catch
                {
                    _isConnected = false;
                    _serialPort.Dispose();
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
            _isConnected = true;
            _eventAggregator.GetEvent<UsbConnectionChangedEvent>().Publish(true);
        }

        private bool TryGetDeviceId(out string deviceId)
        {
            using (var searcher = new ManagementObjectSearcher
                ("SELECT * FROM WIN32_SerialPort WHERE Description = 'Arduino Leonardo'"))
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
