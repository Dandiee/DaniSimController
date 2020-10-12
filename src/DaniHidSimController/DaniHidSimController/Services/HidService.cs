using System;
using System.Runtime.InteropServices;
using DaniHidSimController.Mvvm;

namespace DaniHidSimController.Services
{
    public interface IHidService
    {
        IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled);
    }

    public sealed class HidStateReceivedEvent : PubSubEvent<DaniDeviceState> { }
    public sealed class HidService : IHidService
    {
        private readonly IEventAggregator _eventAggregator;

        public HidService(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;
        }

        public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == 0x00FF)
            {
                var newState = GetDeviceState(lParam, out _);
                if (newState.ReportId == 6)
                {
                    _eventAggregator.GetEvent<HidStateReceivedEvent>().Publish(newState);
                }

                handled = true;
            }

            return IntPtr.Zero;
        }

        private DaniDeviceState GetDeviceState(IntPtr rawInputHandle, out byte[] bytes)
        {
            var skipper = 24u;
            var dwSize = 0u;
            var nativeBuffer = IntPtr.Zero;
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                skipper);

            nativeBuffer = Marshal.AllocHGlobal((int)dwSize);
            WinApi.GetRawInputData(rawInputHandle, 268435459, nativeBuffer, ref dwSize,
                skipper);

            bytes = new byte[dwSize];
            Marshal.Copy(nativeBuffer, bytes, 0, (int)dwSize);

            return Marshal.PtrToStructure<DaniDeviceState>(IntPtr.Add(nativeBuffer, (int)(dwSize - 26)));
        }

    }
}
