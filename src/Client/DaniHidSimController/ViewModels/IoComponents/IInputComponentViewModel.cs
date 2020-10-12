using DaniHidSimController.Services;

namespace DaniHidSimController.ViewModels.IoComponents
{
    public interface IInputComponentViewModel
    {
        void Update(DaniDeviceState state);
    }
}
