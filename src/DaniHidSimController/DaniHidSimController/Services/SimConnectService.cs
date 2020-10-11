using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows.Interop;
using DaniHidSimController.Models;
using DaniHidSimController.Mvvm;
using Microsoft.Extensions.Options;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniHidSimController.Services
{
    public interface ISimConnectService
    {
        void SetHandle(HwndSource hwndSource);
        IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled);
        void TransmitEvent(SimEvents simEvent, uint value);
    }

    public sealed class SimVarReceivedEvent : PubSubEvent<SimVarRequest> { }
    public sealed class SimConnectConnectionChangedEvent : PubSubEvent<bool> { }

    public sealed class SimConnectService : ISimConnectService
    {
        private readonly IEventAggregator _eventAggregator;
        private readonly SimOptions _options;
        private readonly MethodInfo _registerDataDefineStructMethodInfo;
        private readonly IReadOnlyDictionary<SimVars, SimVarRequest> _simVarsByRequests;
        private readonly IReadOnlyCollection<SimVarRequest> _simVars = new SimVarRequest[]
        {
            new SimVarRequest<bool>(SimVars.AUTOPILOT_MASTER),

            new SimVarRequest<bool>(SimVars.AUTOPILOT_HEADING_LOCK),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_ALTITUDE_LOCK),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_ATTITUDE_HOLD),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_AIRSPEED_HOLD),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_VERTICAL_HOLD),

            new SimVarRequest<float>(SimVars.GEAR_LEFT_POSITION),
            new SimVarRequest<float>(SimVars.GEAR_RIGHT_POSITION),
            new SimVarRequest<float>(SimVars.GEAR_CENTER_POSITION),

            new SimVarRequest<float>(SimVars.FLAPS_HANDLE_PERCENT),
            new SimVarRequest<float>(SimVars.BRAKE_LEFT_POSITION),

            new SimVarRequest<bool>(SimVars.BRAKE_PARKING_INDICATOR),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_THROTTLE_ARM),
            new SimVarRequest<bool>(SimVars.AUTOPILOT_YAW_DAMPER),
            new SimVarRequest<float>(SimVars.GPS_POSITION_LAT),
            new SimVarRequest<float>(SimVars.GPS_POSITION_LON),
        };

        private HwndSource _hwndSource;
        private SimConnect _simConnect;

        public SimConnectService(
            IEventAggregator eventAggregator,
            IOptions<SimOptions> options)
        {
            _eventAggregator = eventAggregator;
            _options = options.Value;
            _simVarsByRequests = _simVars.ToDictionary(kvp => kvp.SimVar, kvp => kvp);
            _registerDataDefineStructMethodInfo =
                typeof(SimConnect).GetMethod(nameof(SimConnect.RegisterDataDefineStruct));
        }

        public void SetHandle(HwndSource hwndSource)
        {
            if (_hwndSource == null)
            {
                _hwndSource = hwndSource;
                StartConnect();
            }
        }

        public IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled)
        {
            if (iMsg == 0x0402)
            {
                _simConnect?.ReceiveMessage();
            }

            return IntPtr.Zero;
        }

        private void StartConnect() => Task.Run(async () => await TryConnect());

        private async Task TryConnect()
        {
            if (_simConnect == null && IsFlightSimulatorRunning())
            {
                try
                {
                    Connect();
                    return;
                }
                catch
                {
                    Debug.WriteLine("Connection failed.");
                }
            }

            await Task.Delay(_options.FlightSimulatorConnectionIntervalInMs);
            await TryConnect();
        }

        private void Connect()
        {
            _simConnect = new SimConnect("DaniConnect", _hwndSource.Handle, 0x0402, null, 0);
            _simConnect.OnRecvOpen += OnConnected;
            _simConnect.OnRecvSimobjectData += OnSimVarReceived;
            _simConnect.OnRecvQuit += OnQuit;
            _simConnect.OnRecvException += OnException;

            _eventAggregator.GetEvent<SimConnectConnectionChangedEvent>().Publish(true);
        }

        private void OnException(SimConnect sender, SIMCONNECT_RECV_EXCEPTION data)
            => throw new Exception(((SIMCONNECT_EXCEPTION)data.dwException).ToString());

        private void OnQuit(SimConnect sender, SIMCONNECT_RECV data)
        {
            _eventAggregator.GetEvent<SimConnectConnectionChangedEvent>().Publish(false);

            _simConnect.OnRecvOpen -= OnConnected;
            _simConnect.OnRecvSimobjectData -= OnSimVarReceived;
            _simConnect.OnRecvQuit -= OnQuit;
            _simConnect.OnRecvException -= OnException;
            _simConnect.Dispose();
            _simConnect = null;

            StartConnect();
        }

        private void OnSimVarReceived(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA data)
        {
            var request = _simVarsByRequests[(SimVars)data.dwRequestID];
            request.Set(data.dwData[0]);
            _eventAggregator.GetEvent<SimVarReceivedEvent>().Publish(request);
        }

        private void OnConnected(SimConnect sender, SIMCONNECT_RECV_OPEN data)
        {
            foreach (var simEvent in Enum.GetValues(typeof(SimEvents)).Cast<SimEvents>())
            {
                _simConnect.MapClientEventToSimEvent(simEvent, simEvent.ToString());
            }

            foreach (var simVar in _simVars)
            {
                _simConnect.AddToDataDefinition(
                    simVar.SimVar,
                    simVar.Name,
                    string.Empty,
                    simVar.SimConnectType,
                    0,
                    SimConnect.SIMCONNECT_UNUSED);

                _registerDataDefineStructMethodInfo.MakeGenericMethod(simVar.ClrType).Invoke(_simConnect, new object[] { simVar.SimVar });

                _simConnect.RequestDataOnSimObject(
                    simVar.SimVar,
                    simVar.SimVar,
                    SimConnect.SIMCONNECT_OBJECT_ID_USER,
                    SIMCONNECT_PERIOD.VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG.CHANGED, 0, 0, 0);
            }
        }

        private bool IsFlightSimulatorRunning()
            => Process.GetProcesses().Any(proc =>
                proc.ProcessName.Equals(_options.FlightSimulatorProcessName,
                    StringComparison.InvariantCultureIgnoreCase));


        public void TransmitEvent(SimEvents simEvent, uint value)
            => _simConnect?.TransmitClientEvent(0, simEvent, value,
                MyGroups.FirstGroup, SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);
    }
}
