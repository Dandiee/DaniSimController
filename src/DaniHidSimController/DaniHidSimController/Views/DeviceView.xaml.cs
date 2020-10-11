using DaniHidSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniHidSimController.Views
{
    public partial class DeviceView
    {
        public DeviceView()
        {
            DataContext = App.ServiceProvider.GetService<DeviceViewModel>();

            InitializeComponent();
        }
    }
}
