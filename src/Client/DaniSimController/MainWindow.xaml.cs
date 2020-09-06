using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using DaniSimController.ViewModels;

namespace DaniSimController
{
    public partial class MainWindow
    {
        public MainWindowViewModel ViewModel { get; }

        public MainWindow()
        {
            InitializeComponent();

            ViewModel = new MainWindowViewModel();
            DataContext = ViewModel;
        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            ViewModel.SetHwndSourceCommand.Execute(PresentationSource.FromVisual(this) as HwndSource);
            base.OnSourceInitialized(e);
        }

        private void SimVarOptionsListDoubleClicked(object sender, MouseButtonEventArgs e)
        {
            var item = GetParent<ListBoxItem>((DependencyObject)e.OriginalSource, sender);
            if (item != null)
            {
                ViewModel.AddSimVarCommand.Execute(item.DataContext);
            }
        }

        private void SelectedSimVarListDoubleClicked(object sender, MouseButtonEventArgs e)
        {
            var item = GetParent<ListViewItem>((DependencyObject)e.OriginalSource, sender);
            if (item != null)
            {
                ViewModel.RemoveSimVarCommand.Execute(item.DataContext);
            }
        }

        private static T GetParent<T>(DependencyObject obj, object sender)
            where T : class
        {
            while (obj != null && obj != sender)
            {
                if (obj is T item)
                {
                    return item;
                }
                obj = VisualTreeHelper.GetParent(obj);
            }

            return null;
        }
    }
}
