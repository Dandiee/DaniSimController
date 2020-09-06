using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController.Views
{
    public partial class PollingView
    {
        private readonly PollingViewModel _viewModel;

        public PollingView()
        {
            _viewModel = App.ServiceProvider.GetService<PollingViewModel>();
            DataContext = _viewModel;

            InitializeComponent();
        }
    }
}
