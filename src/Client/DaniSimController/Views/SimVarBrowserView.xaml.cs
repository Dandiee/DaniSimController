using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using DaniSimController.Extensions;
using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController.Views
{
    public partial class SimVarBrowserView
    {
        private readonly SimVarBrowserViewModel _viewModel;

        public SimVarBrowserView()
        {
            _viewModel = App.ServiceProvider.GetService<SimVarBrowserViewModel>();
            DataContext = _viewModel;

            InitializeComponent();
        }

        private void SimVarOptionsListDoubleClicked(object sender, MouseButtonEventArgs e)
        {
            var item = ((DependencyObject) e.OriginalSource).GetParent<ListViewItem>(sender);
            if (item != null)
            {
                _viewModel.SelectSimVarCommand.Execute(item.DataContext);
            }
        }
    }
}
