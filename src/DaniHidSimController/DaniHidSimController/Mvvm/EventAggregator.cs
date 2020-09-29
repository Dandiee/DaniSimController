using System;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace DaniHidSimController.Mvvm
{
    public interface IEventAggregator
    {
        TEventType GetEvent<TEventType>() where TEventType : EventBase, new();
    }

    public sealed class EventAggregator : IEventAggregator
    {
        private readonly ConcurrentDictionary<Type, EventBase> _events;

        public EventAggregator()
        {
            _events = new ConcurrentDictionary<Type, EventBase>();
        }

        public TEventType GetEvent<TEventType>() 
            where TEventType : EventBase, new()
        {
            var type = typeof(TEventType);
            return (TEventType) _events.GetOrAdd(type, t => (EventBase) Activator.CreateInstance(t));
        }
    }

    public abstract class EventBase { }

    public abstract class PubSubEvent<TArgs> : EventBase
    {
        private readonly List<Action<TArgs>> _callbacks;

        protected PubSubEvent()
        {
            _callbacks = new List<Action<TArgs>>();
        }

        public void Publish(TArgs args)
        {
            foreach (var callback in _callbacks)
            {
                callback.Invoke(args);
            }
        }

        public void Subscribe(Action<TArgs> callback)
            => _callbacks.Add(callback);

        public void Unsubscribe(Action<TArgs> callback)
            => _callbacks.Remove(callback);
    }
}
