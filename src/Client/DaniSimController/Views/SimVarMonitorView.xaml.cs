using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using DaniSimController.Extensions;
using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController.Views
{
    public partial class SimVarMonitorView
    {
        private readonly SimVarMonitorViewModel _viewModel;

        public SimVarMonitorView()
        {
            _viewModel = App.ServiceProvider.GetService<SimVarMonitorViewModel>();
            DataContext = _viewModel;
            InitializeComponent();
        }

        private void SelectedSimVarListDoubleClicked(object sender, MouseButtonEventArgs e)
        {
            var item = ((DependencyObject)e.OriginalSource).GetParent<ListViewItem>(sender);
            if (item != null)
            {
                _viewModel.RemoveSimVarCommand.Execute(item.DataContext);
            }
        }
    }
}
