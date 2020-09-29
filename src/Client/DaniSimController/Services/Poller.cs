using System;
using System.Linq;
using System.Timers;
using HidSharp;
using HidSharp.Reports;
using SharpDX.Multimedia;
using SharpDX.RawInput;

namespace DaniSimController.Services
{
    public interface IPoller
    {
        void Start();
        void Stop();
        void SetInterval(int intervalInMs);

        bool IsPolling { get; }
        event EventHandler Tick;
        event EventHandler OnStateChanged;
    }

    public sealed class Poller : IPoller, IDisposable
    {
        private readonly Timer _timer;

        public Poller()
        {
            _timer = new Timer
            {
                Interval = 1
            };
            //var devices = SharpDX.RawInput.Device.GetDevices().Select(r => new SharpDX.RawInput.());


            var hids = HidSharp.DeviceList.Local.GetHidDevices().Where(s => s.ProductName == "Arduino Leonardo")
                .Single();
            HidStream stream;
            hids.TryOpen(out stream);
            var reportDescriptor = hids.GetReportDescriptor();
            var inputReceiver = reportDescriptor.CreateHidDeviceInputReceiver();
            var inputReportBuffer = new byte[hids.GetMaxInputReportLength()];
            var inputParser = reportDescriptor.DeviceItems[0].CreateDeviceItemInputParser();
            inputReceiver.Received += (sender, e) =>
            {
                Report report;
                while (inputReceiver.TryRead(inputReportBuffer, 0, out report))
                {
                    // Parse the report if possible.
                    // This will return false if (for example) the report applies to a different DeviceItem.
                    if (inputParser.TryParseReport(inputReportBuffer, 0, report))
                    {
                        // If you are using Windows Forms, you could call BeginInvoke here to marshal the results
                        // to your main thread.
                        while (inputParser.HasChanged)
                        {
                            int changedIndex = inputParser.GetNextChangedIndex();
                            var previousDataValue = inputParser.GetPreviousValue(changedIndex);
                            var dataValue = inputParser.GetValue(changedIndex);

                            Console.WriteLine(string.Format("  {0}: {1} -> {2}",
                                (Usage) dataValue.Usages.FirstOrDefault(), previousDataValue.GetPhysicalValue(),
                                dataValue.GetPhysicalValue()));
                        }

                    }
                }
            };

            stream.InterruptRequested += (sender, args) =>
            {

            };
            SharpDX.RawInput.Device.RegisterDevice(UsagePage.Generic, UsageId.GenericKeyboard, DeviceFlags.InputSink);
            SharpDX.RawInput.Device.RawInput += DeviceOnRawInput;


            
            
            _timer.Elapsed += (_, args) => Tick?.Invoke(this, args);
        }

        private void DeviceOnRawInput(object? sender, RawInputEventArgs e)
        {

        }

        public void Start()
        {
            _timer.Enabled = true;
            OnStateChanged?.Invoke(this, null);
        }

        public void Stop()
        {
            _timer.Enabled = false;
            OnStateChanged?.Invoke(this, null);
        }

        public void SetInterval(int intervalInMs) => _timer.Interval = intervalInMs;
        public bool IsPolling => _timer.Enabled;

        public event EventHandler Tick;
        public event EventHandler OnStateChanged;

        public void Dispose()
        {
            _timer?.Dispose();
        }
    }
}
