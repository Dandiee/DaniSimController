using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class LedView
    {
        private const double DefaultDiameter = 20d;

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

        public static readonly DependencyProperty DiameterProperty = DependencyProperty.Register(
            nameof(Diameter), typeof(double), typeof(LedView), new PropertyMetadata(DefaultDiameter));
        public double Diameter
        {
            get => (double) GetValue(DiameterProperty);
            set => SetValue(DiameterProperty, value);
        }
    }
}
