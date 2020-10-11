using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels
{
    public interface IInputComponentViewModel
    {
        void Update(DaniDeviceState state);
    }
}
