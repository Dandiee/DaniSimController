using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;

namespace DaniSimController.Services
{
    public interface ISerialComponent
    {

    }

    public sealed class SerialComponent : ISerialComponent, IDisposable
    {
        private readonly SerialPort _serialPort;

        public SerialComponent()
        {
            _serialPort = new SerialPort();
        }

        public void asd()
        {
            
        }

        public void Dispose()
        {
            _serialPort?.Dispose();
        }
    }
}
