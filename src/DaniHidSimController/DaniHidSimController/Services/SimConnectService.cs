using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;
using System.Windows.Controls;
using System.Windows.Interop;
using DaniHidSimController.Models;
using DaniSimController.Models;
using DaniSimController.Services;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniHidSimController.Services
{
    public interface ISimConnectService
    {
        void Connect(HwndSource hwndSource);
        IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled);
        void TransmitEvent(SimEvents simEvent, uint value);
    }

    public sealed class SimConnectService : ISimConnectService
    {
        private SimConnect _simConnect;
        private readonly IDictionary<Numbers, SimVarRequest> _requests;

        public SimConnectService()
        {
            _requests = new Dictionary<Numbers, SimVarRequest>();
        }

        public void Connect(HwndSource hwndSource)
        {
            _simConnect = new SimConnect("DaniConnect", hwndSource.Handle, 0x0402, null, 0);
            _simConnect.OnRecvOpen += (_, __) => OnConnected();
            _simConnect.OnRecvSimobjectDataBytype += (_, data) => OnReceived(data);
            _simConnect.OnRecvException += (_, ex) => OnError(ex);
        }

        private void AddRequest(SimVar simVar)
        {
            var requestId = GetNextNumber();

            _simConnect.AddToDataDefinition(
                requestId,
                simVar.Name,
                simVar.Type.Name,
                SIMCONNECT_DATATYPE.FLOAT64,
                0,
                SimConnect.SIMCONNECT_UNUSED);

            _simConnect.RegisterDataDefineStruct<double>(requestId);
            var request = new SimVarRequest(simVar, requestId);
            _requests[requestId] = request;

            Poll(request);
        }

        private void OnReceived(SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            if (_requests.TryGetValue((Numbers)data.dwRequestID, out var request))
            {
                request.IsPending = false;
                Poll(request);
                request.IsPending = true;
            }


        }

        private void Poll(SimVarRequest request)
            => _simConnect.RequestDataOnSimObjectType(
                request.RequestId,
                request.RequestId,
                0,
                SIMCONNECT_SIMOBJECT_TYPE.USER);

      
        private void OnConnected()
        {
            foreach (SimEvents simEvent in Enum.GetValues(typeof(SimEvents)).Cast<SimEvents>())
            {
                _simConnect.MapClientEventToSimEvent(simEvent, simEvent.ToString());
                /*_simConnect.AddClientEventToNotificationGroup(MyGroups.FirstGroup, simEvent, false);
                _simConnect.SetNotificationGroupPriority(MyGroups.FirstGroup,
                    (uint) GroupIds.SIMCONNECT_GROUP_PRIORITY_HIGHEST);*/
            }

        }

        public void TransmitEvent(SimEvents simEvent, uint value)
            => _simConnect?.TransmitClientEvent(0, simEvent, value,
                MyGroups.FirstGroup, SIMCONNECT_EVENT_FLAG.GROUPID_IS_PRIORITY);



        private void OnError(SIMCONNECT_RECV_EXCEPTION ex)
        {
            throw new Exception();
        }

        public IntPtr WndProc(IntPtr hWnd, int iMsg, IntPtr hWParam, IntPtr hLParam, ref bool bHandled)
        {
            if (iMsg == 0x0402)
            {
                _simConnect.ReceiveMessage();
            }

            return IntPtr.Zero;
        }

        private Numbers GetNextNumber() =>
            Enumerable.Range(0, 64).Cast<Numbers>().First(n => !_requests.ContainsKey(n));
    }

    enum EVENT_CTRL
    {
        PAUSE, //set pause
        ABORT, //Quit without message
        CLOCK, //Hours of Local Time
    }

    enum GROUP_IDS
    {
        GROUP_1,
    }

    public sealed class SimVarRequest
    {
        public SimVar SimVar { get; }
        public Numbers RequestId { get; }
        public bool IsPending { get; set; }

        public SimVarRequest(SimVar simVar, Numbers requestId)
        {
            SimVar = simVar;
            RequestId = requestId;
        }
    }

    public enum MyGroups : uint
    {
        FirstGroup = 0
    }

    public enum GroupIds : uint
    {
        SIMCONNECT_GROUP_PRIORITY_LOWEST = 4000000000,
        SIMCONNECT_GROUP_PRIORITY_DEFAULT = 2000000000,
        SIMCONNECT_GROUP_PRIORITY_STANDARD = 1900000000,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST_MASKABLE = 10000000,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST = 1
    }
}
