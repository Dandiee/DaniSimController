using System;
using System.Buffers.Binary;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Documents;
using System.Windows.Interop;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniHidSimController.Services
{
    public interface ISimConnectService
    {
        void Connect(HwndSource hwndSource);
        IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled);
        void TransmitEvent(SimEvents simEvent, uint value);
        public event EventHandler<SimVarRequest> OnSimVarsChanged;
    }

    public sealed class SimConnectService : ISimConnectService
    {
        public event EventHandler<SimVarRequest> OnSimVarsChanged;

        private SimConnect _simConnect;
        private bool _isInitialized;

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
        };

        public SimConnectService()
        {
            _simVarsByRequests = _simVars.ToDictionary(kvp => kvp.SimVar, kvp => kvp);
        }

        public void Connect(HwndSource hwndSource)
        {
            _simConnect = new SimConnect("DaniConnect", hwndSource.Handle, 0x0402, null, 0);
            _simConnect.OnRecvOpen += (_, __) => OnConnected();
            _simConnect.OnRecvSimobjectData += (_, data) => OnSimVarReceived(data);
            _simConnect.OnRecvException +=
                (_, ex) => throw new Exception(((SIMCONNECT_EXCEPTION) ex.dwException).ToString());
        }

        private void OnSimVarReceived(SIMCONNECT_RECV_SIMOBJECT_DATA data)
        {
            var request = _simVarsByRequests[(SimVars)data.dwRequestID];
            request.Set(data.dwData[0]);

            if (!_isInitialized)
            {
                _isInitialized = _simVars.All(s => s.IsInitialized);
            }
            else
            {
                OnSimVarsChanged?.Invoke(this, request);
            }
        }

        private void OnConnected()
        {
            foreach (var simEvent in Enum.GetValues(typeof(SimEvents)).Cast<SimEvents>())
            {
                _simConnect.MapClientEventToSimEvent(simEvent, simEvent.ToString());
            }


            var methodInfo = typeof(SimConnect).GetMethod(nameof(SimConnect.RegisterDataDefineStruct));
            if (methodInfo == default) throw new NotSupportedException("Not okay");

            foreach (var simVar in _simVars)
            {
                _simConnect.AddToDataDefinition(
                    simVar.SimVar,
                    simVar.Name,
                    string.Empty,
                    simVar.SimConnectType,
                    0,
                    SimConnect.SIMCONNECT_UNUSED);
                methodInfo.MakeGenericMethod(simVar.ClrType).Invoke(_simConnect, new object[] {simVar.SimVar});
                _simConnect.RequestDataOnSimObject(
                    simVar.SimVar,
                    simVar.SimVar, 
                    SimConnect.SIMCONNECT_OBJECT_ID_USER, 
                    SIMCONNECT_PERIOD.VISUAL_FRAME, SIMCONNECT_DATA_REQUEST_FLAG.CHANGED, 0, 0, 0);
            }
        }

        public void TransmitEvent(SimEvents simEvent, uint value)
            => _simConnect?.TransmitClientEvent(0, simEvent, value,
                MyGroups.FirstGroup, SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);


        public IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled)
        {
            if (iMsg == 0x0402)
            {
                _simConnect.ReceiveMessage();
            }

            return IntPtr.Zero;
        }
    }

    public abstract class SimVarRequest
    {
        private static readonly IReadOnlyDictionary<Type, SIMCONNECT_DATATYPE> SimConnectDataTypes =
            new Dictionary<Type, SIMCONNECT_DATATYPE>
            {
                [typeof(bool)] = SIMCONNECT_DATATYPE.INT32,
                [typeof(float)] = SIMCONNECT_DATATYPE.FLOAT32,
            };

        public SimVars SimVar { get; }
        public Type ClrType { get; }
        public SIMCONNECT_DATATYPE SimConnectType { get; }
        public string Name { get; }

        public abstract void Set(object obj);
        public abstract object Get();

        public bool IsInitialized { get; protected set; }

        protected SimVarRequest(SimVars simVar, Type clrType)
        {
            SimVar = simVar;
            ClrType = clrType;
            SimConnectType = SimConnectDataTypes[clrType];
            Name = simVar.ToString().Replace("_", " ");
        }
    }
    public sealed class SimVarRequest<T> : SimVarRequest
    {
        public T Value { get; set; }

        public SimVarRequest(SimVars simVar) : base(simVar, typeof(T)) { }

        public override void Set(object obj)
        {
            Value = (T) obj;
            IsInitialized = true;
        }

        public override object Get() => Value;
    }

    public enum SimEvents
    {
        AP_ALT_VAR_INC,
        AP_ALT_VAR_DEC,

        AP_VS_VAR_INC,
        AP_VS_VAR_DEC,

        AP_SPD_VAR_INC,
        AP_SPD_VAR_DEC,

        HEADING_BUG_INC,
        HEADING_BUG_DEC
    }

    public enum SimVars
    {
        AUTOPILOT_MASTER,
        AUTOPILOT_HEADING_LOCK,
        AUTOPILOT_ALTITUDE_LOCK,
        AUTOPILOT_ATTITUDE_HOLD,
        AUTOPILOT_AIRSPEED_HOLD,
        AUTOPILOT_VERTICAL_HOLD,

        GEAR_LEFT_POSITION,
        GEAR_RIGHT_POSITION,
        GEAR_CENTER_POSITION
    }

    public enum MyGroups : uint
    {
        FirstGroup = 0
    }
}
