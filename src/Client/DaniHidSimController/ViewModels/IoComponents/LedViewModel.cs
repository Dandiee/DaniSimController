using DaniHidSimController.Mvvm;

namespace DaniHidSimController.ViewModels.IoComponents
{
    public enum LedState { Off, On, Blink }

    public sealed class LedViewModel : BindableBase
    {
        public Pin Pin { get; }

        public LedViewModel(bool isGpio, int index)
        {
            Pin = new Pin(index, isGpio);
        }

        private LedState _state;
        public LedState State
        {
            get => _state;
            set => SetProperty(ref _state, value);
        }
    }

    public static class LedStateExtensions
    {
        public static LedState ToLedState(this bool value) => value ? LedState.On : LedState.Off;
    }
}
