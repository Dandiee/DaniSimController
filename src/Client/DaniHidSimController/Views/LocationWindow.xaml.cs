using DaniHidSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniHidSimController.Views
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
