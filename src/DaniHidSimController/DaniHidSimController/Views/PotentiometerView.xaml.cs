using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class PotentiometerView
    {
        public PotentiometerView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty DiameterProperty = DependencyProperty.Register(
            nameof(Diameter), typeof(double), typeof(PotentiometerView), new PropertyMetadata(default(double), OnDiameterPropertyChanged));

        public static readonly DependencyProperty RadiusProperty = DependencyProperty.Register(
            nameof(Radius), typeof(double), typeof(PotentiometerView), new PropertyMetadata(default(double)));
        public double Radius
        {
            get => (double) GetValue(RadiusProperty);
            set => SetValue(RadiusProperty, value);
        }

        private static void OnDiameterPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is PotentiometerView potentiometerView)
            {
                potentiometerView.Radius = (double) e.NewValue / 2;
            }
        }

        public double Diameter
        {
            get => (double) GetValue(DiameterProperty);
            set => SetValue(DiameterProperty, value);
        }

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            nameof(Value), typeof(short), typeof(PotentiometerView), new PropertyMetadata(default(short), OnValuePropertyChanged));

        private static void OnValuePropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is PotentiometerView potentiometerView)
            {
                var value = (short) e.NewValue;
                var ratio = value / (float) short.MaxValue;
                potentiometerView.Angle = ratio * 170;
            }
        }

        public static readonly DependencyProperty AngleProperty = DependencyProperty.Register(
            nameof(Angle), typeof(double), typeof(PotentiometerView), new PropertyMetadata(default(double)));

        public double Angle
        {
            get => (double) GetValue(AngleProperty);
            set => SetValue(AngleProperty, value);
        }

        public short Value
        {
            get => (short) GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title), typeof(string), typeof(PotentiometerView), new PropertyMetadata(default(string)));
        public string Title
        {
            get => (string) GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }
    }
}
