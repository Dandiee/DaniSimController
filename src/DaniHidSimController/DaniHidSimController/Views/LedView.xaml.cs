using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class LedView
    {
        public LedView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty IsActiveProperty = DependencyProperty.Register(
            nameof(IsActive), typeof(bool), typeof(LedView), new PropertyMetadata(default(bool)));
        public bool IsActive
        {
            get => (bool) GetValue(IsActiveProperty);
            set => SetValue(IsActiveProperty, value);
        }

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title), typeof(string), typeof(LedView), new PropertyMetadata(default(string)));

        public string Title
        {
            get => (string) GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }
    }
}
