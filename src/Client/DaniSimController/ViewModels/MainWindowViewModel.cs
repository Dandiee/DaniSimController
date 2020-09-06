using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Timers;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Interop;
using DaniSimController.Models;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniSimController.ViewModels
{
    public sealed class MainWindowViewModel : BindableBase, IDisposable
    {
        public ICommand AddSimVarCommand { get; }
        public ICommand PausePlayTimerCommand { get; }
        public ICommand SetHwndSourceCommand { get; }
        public ICommand ClearSimVarTypeFilter { get; }
        public ICommand RemoveSimVarCommand { get; }

        public ObservableCollection<SelectedSimVarViewModel> SelectedSimVars { get; }
        public ObservableCollection<SimVar> AvailableSimVars { get; }
        public ICollectionView AvailableSimVarCollection { get; }


        private HwndSource _hwndSource;
        private SimConnect _simConnect;

        private readonly Timer _pollingTimer;
        private readonly Timer _systemTimer;

        private readonly IDictionary<Numbers, SelectedSimVarViewModel> _requests;

        private string _availableSimVarFilter;
        public string AvailableSimVarFilter
        {
            get => _availableSimVarFilter;
            set
            {
                if (SetProperty(ref _availableSimVarFilter, value))
                {
                    AvailableSimVarCollection.Refresh();
                }
            }
        }

        private SimVarType _selectedAvailableSimVarType;
        public SimVarType SelectedAvailableSimVarType
        {
            get => _selectedAvailableSimVarType;
            set
            {
                if (SetProperty(ref _selectedAvailableSimVarType, value))
                {
                    AvailableSimVarCollection.Refresh();
                }
            }
        }

        private int _pollIntervalInMs = 300;
        public int PollIntervalInMs
        {
            get => _pollIntervalInMs;
            set
            {
                if (SetProperty(ref _pollIntervalInMs, value))
                {
                    _pollingTimer.Interval = value;
                }
            }
        }

        private bool _isPolling;
        public bool IsPolling
        {
            get => _isPolling;
            set => SetProperty(ref _isPolling, value);
        }

        private bool _isSimConnected;
        public bool IsSimConnected
        {
            get => _isSimConnected;
            set => SetProperty(ref _isSimConnected, value);
        }

        public MainWindowViewModel()
        {
            _pollingTimer = new Timer(PollIntervalInMs)
            {
                Enabled = false,
            };
            _pollingTimer.Elapsed += PollTick;

            _systemTimer = new Timer(100)
            {
                Enabled = true
            };
            _systemTimer.Elapsed += SystemTick;

            AddSimVarCommand = new DelegateCommand<SimVar>(AddSimVar);
            PausePlayTimerCommand = new DelegateCommand(PausePlayTimer);
            SetHwndSourceCommand = new DelegateCommand<HwndSource>(hwndSource => _hwndSource = hwndSource);
            ClearSimVarTypeFilter = new DelegateCommand(() => SelectedAvailableSimVarType = null);
            RemoveSimVarCommand = new DelegateCommand<SelectedSimVarViewModel>(RemoveSimVar);

            // _serialPort = new SerialPort("COM10", baudRate: 115200, Parity.None, dataBits: 8, StopBits.One)
            // {
            //     RtsEnable = true,
            //     DtrEnable = true
            // };
            // _serialPort.Open();
            // _serialPort.DataReceived += (a, b) =>
            // {
            //     var asd = 42;
            // };

            SelectedSimVars = new ObservableCollection<SelectedSimVarViewModel>(
                new[]
                {
                   //new SelectedSimVarViewModel(SimVar.GearCenterPosition, Numbers.N1),
                   //new SelectedSimVarViewModel(SimVar.IndicatedAltitude, Numbers.N2),
                   new SelectedSimVarViewModel(SimVar.NavName, Numbers.N3),
                }
            );

            _requests = SelectedSimVars.ToDictionary(kvp => kvp.Number);

            AvailableSimVars = new ObservableCollection<SimVar>(SimVar.All.Where(v => SelectedSimVars.All(sm => sm.SimVar != v)));
            AvailableSimVarCollection = new ListCollectionView(AvailableSimVars)
            {
                Filter = obj =>
                    (obj is SimVar simVar && 
                     (string.IsNullOrEmpty(AvailableSimVarFilter) || simVar.FriendlyName.ToLowerInvariant().Contains(AvailableSimVarFilter.ToLowerInvariant())) &&
                     (SelectedAvailableSimVarType == null || simVar.Type == SelectedAvailableSimVarType))
            };
        }

        private void RemoveSimVar(SelectedSimVarViewModel simVar)
        {
            SelectedSimVars.Remove(simVar);
            _simConnect.ClearDataDefinition(simVar.Number);
            AvailableSimVars.Add(simVar.SimVar);
            _requests.Remove(simVar.Number);
        }

        private void PausePlayTimer()
        {
            _pollingTimer.Enabled = !_pollingTimer.Enabled;
            IsPolling = _pollingTimer.Enabled;
        }

        private void PollTick(object sender, ElapsedEventArgs e)
        {
            foreach (var selectedSimVar in SelectedSimVars.Where(s => !s.IsPending))
            {
                _simConnect.RequestDataOnSimObjectType(
                    selectedSimVar.Number,
                    selectedSimVar.Number, 0,
                    SIMCONNECT_SIMOBJECT_TYPE.USER);
                selectedSimVar.IsPending = true;

            }
        }

        private void SystemTick(object sender, ElapsedEventArgs e)
        {
            if (_hwndSource != null)
            {
                if (_simConnect == null)
                {
                    IsSimConnected = TryConnect();
                }
            }
        }

        private void OnReceiveSimObjectDataByType(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            var request = _requests[(Numbers)data.dwRequestID];
            request.Value = data.dwData[0].ToString();
            request.IsPending = false;
        }

        private void RegisterRequest(SelectedSimVarViewModel request)
        {
            _simConnect.AddToDataDefinition(
                request.Number,
                request.SimVar.Name,
                request.SimVar.Type.Name,
                SIMCONNECT_DATATYPE.FLOAT64,
                0,
                SimConnect.SIMCONNECT_UNUSED);

            _simConnect.RegisterDataDefineStruct<double>(request.Number);
        }

        private bool TryConnect()
        {
            try
            {
                _simConnect = new SimConnect("Dani", _hwndSource.Handle, 0x0402, null, 0);
                _simConnect.OnRecvOpen += (_, __) =>
                {
                    foreach (var simVar in SelectedSimVars)
                    {
                        RegisterRequest(simVar);
                    }

                    _pollingTimer.Enabled = true;
                };

                _simConnect.OnRecvSimobjectDataBytype += OnReceiveSimObjectDataByType;

                _hwndSource.AddHook(WindowProc);

                return true;
            }
            catch
            {
                return false;
            }
        }

        private IntPtr WindowProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled)
        {
            if (iMsg == 0x0402)
            {
                _simConnect.ReceiveMessage();
            }

            return IntPtr.Zero;
        }

        private void AddSimVar(SimVar simVar)
        {
            var nextAvailableNumber = Enumerable.Range(0, 64).Cast<Numbers>()
                .First(n => SelectedSimVars.All(m => m.Number != n));

            var request = new SelectedSimVarViewModel(simVar, nextAvailableNumber);
            SelectedSimVars.Add(request);
            RegisterRequest(request);
            AvailableSimVars.Remove(simVar);
            _requests[request.Number] = request;
        }

        public void Dispose()
        {
            _pollingTimer?.Dispose();
            _systemTimer?.Dispose();
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct DaniString
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        [FieldOffset(0)]
        public string Value;

        public override string ToString() => Value;
    }

    public enum Numbers
    {
        N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N17, N18, N19, N20, N21, N22, N23, N24, N25, N26, N27, N28, N29, N30, N31, N32, N33, N34, N35, N36, N37, N38, N39, N40, N41, N42, N43, N44, N45, N46, N47, N48, N49, N50, N51, N52, N53, N54, N55, N56, N57, N58, N59, N60, N61, N62, N63, N64,
    }
}
