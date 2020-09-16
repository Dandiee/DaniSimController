using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Interop;
using DaniSimController.Models;
using DaniSimController.Mvvm;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniSimController.Services
{
    public interface ISimVarComponent
    {
        void Connect(HwndSource hwndSource);
    }

    public sealed class SimVarComponent : ISimVarComponent
    {
        private SimConnect _simConnect;
        private readonly IPoller _poller;
        private readonly IEventAggregator _eventAggregator;
        private readonly IDictionary<Numbers, SimVarRequest> _requests;

        public SimVarComponent(
            IPoller poller,
            IEventAggregator eventAggregator)
        {
            _poller = poller;
            _eventAggregator = eventAggregator;

            _requests = new Dictionary<Numbers, SimVarRequest>();

            _eventAggregator.GetEvent<RemoveSimVarRequestEvent>().Subscribe(RemoveSimVar);
            _eventAggregator.GetEvent<AddSimVarRequestEvent>().Subscribe(AddSimVar);

            _poller.Tick += (_, __) => Tick();
        }


        private Numbers GetNextNumber() =>
            Enumerable.Range(0, 64).Cast<Numbers>().First(n => !_requests.ContainsKey(n));

        public void Connect(HwndSource hwndSource)
        {
            // Gracefully wait for simconnect to start
            _simConnect = new SimConnect("Dani", hwndSource.Handle, 0x0402, null, 0);
            _simConnect.OnRecvOpen += (_, __) =>
            {
                _poller.Start();
            };

            _simConnect.OnRecvSimobjectDataBytype += SimVarReceived;
            hwndSource.AddHook(WindowProc);
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
            _eventAggregator.GetEvent<SimVarRequestAddedEvent>().Publish(request);
        }

        private void RemoveSimVar(SimVarRequest request)
        {
            _simConnect.ClearDataDefinition(request.RequestId);
            _requests.Remove(request.RequestId);
            _eventAggregator.GetEvent<SimVarRequestRemovedEvent>().Publish(request);
        }

        private void SimVarReceived(SimConnect sender, SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE data)
        {
            if (_requests.TryGetValue((Numbers) data.dwRequestID, out var request))
            {
                request.IsPending = false;
                _eventAggregator.GetEvent<SimVarReceivedEvent>()
                    .Publish(new SimVarReceivedEventArgs(request.RequestId, data.dwData[0]));
            }
        }

        private void Tick()
        {
            foreach (var selectedSimVar in _requests.Values.Where(s => !s.IsPending))
            {
                _simConnect.RequestDataOnSimObjectType(
                    selectedSimVar.RequestId,
                    selectedSimVar.RequestId,
                    0,
                    SIMCONNECT_SIMOBJECT_TYPE.USER);
                selectedSimVar.IsPending = true;
            }
        }
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

    public sealed class SimVarReceivedEvent : PubSubEvent<SimVarReceivedEventArgs> { }
    public sealed class SimVarReceivedEventArgs
    {
        public Numbers RequestId { get; }
        public object Value { get; }

        public SimVarReceivedEventArgs(Numbers requestId, object value)
        {
            RequestId = requestId;
            Value = value;
        }
    }

    public sealed class SimVarRequestAddedEvent : PubSubEvent<SimVarRequest> { }
    public sealed class SimVarRequestRemovedEvent : PubSubEvent<SimVarRequest> { }
    public sealed class RemoveSimVarRequestEvent : PubSubEvent<SimVarRequest> { }
    public sealed class AddSimVarRequestEvent : PubSubEvent<SimVar> { }
}
