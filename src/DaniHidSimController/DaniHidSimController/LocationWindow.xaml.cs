using DaniHidSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniHidSimController
{
    public partial class LocationWindow
    {
        public LocationWindow()
        {
            DataContext = App.ServiceProvider.GetService<LocationWindowViewModel>();
            InitializeComponent();
        }
    }
}
