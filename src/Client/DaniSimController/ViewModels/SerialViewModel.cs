using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using DaniSimController.Extensions;
using DaniSimController.Mvvm;

namespace DaniSimController.ViewModels
{
    public sealed class WriteToSerialEvent : PubSubEvent<string> { }

    public sealed class SerialViewModel : BindableBase
    {
        private readonly IEventAggregator _eventAggregator;

        private static readonly IReadOnlyCollection<int> SupportedBaudRates = new[]
            {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 74800, 115200, 230400, 250000, 500000, 10000000};

        private static readonly IReadOnlyCollection<Parity> SupportedParities = EnumExtensions.GetEnums<Parity>();
        private static readonly IReadOnlyCollection<StopBits> SupportedStopBits = EnumExtensions.GetEnums<StopBits>();

        public ICommand ConnectDisconnectCommand { get; }

        public IReadOnlyCollection<int> BaudRateOptions => SupportedBaudRates;
        public IReadOnlyCollection<Parity> ParityOptions => SupportedParities;
        public IReadOnlyCollection<StopBits> StopBitsOptions => SupportedStopBits;
        
        private readonly SerialPort _serialPort;

        public SerialViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
            ConnectDisconnectCommand = new DelegateCommand(ConnectDisconnect);

            _eventAggregator.GetEvent<WriteToSerialEvent>().Subscribe(Write);

            _serialPort = new SerialPort();
            Refresh();
        }

        private void Write(string line)
        {
            if (IsOpened)
            {
                _serialPort.WriteLine(line);
            }
        }

        private void Refresh()
        {
            PortNames = SerialPort.GetPortNames();
            PortName = PortNames.LastOrDefault();
            CanConnect = !string.IsNullOrEmpty(PortName);
        }

        private void ConnectDisconnect()
        {
            if (IsOpened)
            {
                _serialPort.Close();
                IsOpened = false;
            }
            else
            {
                Connect();
            }
        }

        private void Connect()
        {
            try
            {
                _serialPort.PortName = PortName;
                _serialPort.BaudRate = BaudRate;
                _serialPort.DataBits = DataBits;
                _serialPort.StopBits = StopBits;
                _serialPort.RtsEnable = RtsEnabled;
                _serialPort.DtrEnable = DtrEnabled;
                _serialPort.Parity = Parity;
                _serialPort.Open();
                IsOpened = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Can't connect :/\r\n" + ex.ToString());
            }
        }

        private bool _isOpened;
        public bool IsOpened
        {
            get => _isOpened;
            set => SetProperty(ref _isOpened, value);
        }

        private bool _rtsEnabled = true;
        public bool RtsEnabled
        {
            get => _rtsEnabled;
            set => SetProperty(ref _rtsEnabled, value);
        }

        private bool _dtrEnabled = true;
        public bool DtrEnabled
        {
            get => _dtrEnabled;
            set => SetProperty(ref _dtrEnabled, value);
        }

        private IReadOnlyCollection<string> _portNames;
        public IReadOnlyCollection<string> PortNames
        {
            get => _portNames;
            set => SetProperty(ref _portNames, value);
        }

        private string _portName;
        public string PortName
        {
            get => _portName;
            set => SetProperty(ref _portName, value);
        }

        private int _baudRate = 9600;
        public int BaudRate
        {
            get => _baudRate;
            set => SetProperty(ref _baudRate, value);
        }


        private Parity _parity = Parity.Even;
        public Parity Parity
        {
            get => _parity;
            set => SetProperty(ref _parity, value);
        }

        private StopBits _stopBits = StopBits.One;
        public StopBits StopBits
        {
            get => _stopBits;
            set => SetProperty(ref _stopBits, value);
        }

        private int _dataBits = 8;
        public int DataBits
        {
            get => _dataBits;
            set => SetProperty(ref _dataBits, value);
        }

        private bool _canConnect;
        public bool CanConnect
        {
            get => _canConnect;
            set => SetProperty(ref _canConnect, value);
        }

    }
}
