using System;
using System.Windows;
using System.Windows.Interop;
using DaniSimController.ViewModels;
using Microsoft.Extensions.DependencyInjection;

namespace DaniSimController
{
    public partial class MainWindow
    {
        private readonly MainWindowViewModel _viewModel;

        public MainWindow()
        {
            InitializeComponent();

            _viewModel = App.ServiceProvider.GetService<MainWindowViewModel>();
            DataContext = _viewModel;
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            _viewModel.SetHwndSourceCommand.Execute(PresentationSource.FromVisual(this) as HwndSource);
            base.OnSourceInitialized(e);
        }
    }
}
