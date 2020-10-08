using System;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Timers;

namespace DaniHidSimController.Services
{
    public interface IUsbService
    {
        void Write(byte[] data);
    }

    public sealed class UsbService : IUsbService
    {
        public const int PollInterval = 1000;
        private byte[] _lastSent;

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
                    _serialPort.Dispose();
                    _isConnected = false;
                    _timer.Start();
                }
            }

            _lastSent = data;
        }

        private readonly Timer _timer;
        private bool _isConnected;
        private SerialPort _serialPort;

        public UsbService()
        {
            _timer = new Timer(PollInterval);
            _timer.Elapsed += OnTick;
            _timer.Start();
        }

        private void OnTick(object sender, ElapsedEventArgs e)
        {
            if (TryGetDeviceId(out var deviceId))
            {
                _timer.Stop();
                _serialPort = new SerialPort(deviceId, 115200);
                _serialPort.Open();
                Write(_lastSent);
                _isConnected = true;
            }
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
