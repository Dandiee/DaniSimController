using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController.Views
{
    public partial class SerialView
    {
        private readonly SerialViewModel _viewModel;

        public SerialView()
        {
            _viewModel = App.ServiceProvider.GetService<SerialViewModel>();
            DataContext = _viewModel;

            InitializeComponent();
        }
    }
}
