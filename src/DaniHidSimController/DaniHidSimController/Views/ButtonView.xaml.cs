using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class ButtonView
    {
        public ButtonView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty IsPressedProperty = DependencyProperty.Register(
            nameof(IsPressed), typeof(bool), typeof(ButtonView), new PropertyMetadata(default(bool)));
        public bool IsPressed
        {
            get => (bool) GetValue(IsPressedProperty);
            set => SetValue(IsPressedProperty, value);
        }

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title), typeof(string), typeof(ButtonView), new PropertyMetadata(default(string)));

        public string Title
        {
            get => (string) GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }
    }
}
