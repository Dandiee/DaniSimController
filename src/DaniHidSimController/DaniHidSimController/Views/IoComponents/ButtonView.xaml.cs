using System.Windows;

namespace DaniHidSimController.Views.IoComponents
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
    }
}
