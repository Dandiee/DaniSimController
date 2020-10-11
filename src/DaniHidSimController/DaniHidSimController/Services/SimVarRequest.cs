using System;
using System.Collections.Generic;
using Microsoft.FlightSimulator.SimConnect;

namespace DaniHidSimController.Services
{
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
            Value = (T)obj;
            IsInitialized = true;
        }

        public override object Get() => Value;
    }
}