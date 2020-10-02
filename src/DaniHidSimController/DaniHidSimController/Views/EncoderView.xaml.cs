using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class EncoderView
    {
        public EncoderView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty RadiusProperty = DependencyProperty.Register(
            nameof(Radius), typeof(double), typeof(EncoderView), new PropertyMetadata(default(double)));
        public double Radius
        {
            get => (double)GetValue(RadiusProperty);
            set => SetValue(RadiusProperty, value);
        }

        private static void OnDiameterPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is EncoderView encoderView)
            {
                encoderView.Radius = (double)e.NewValue / 2;
            }
        }

        public static readonly DependencyProperty DiameterProperty = DependencyProperty.Register(
            nameof(Diameter), typeof(double), typeof(EncoderView), new PropertyMetadata(default(double), OnDiameterPropertyChanged));
        public double Diameter
        {
            get => (double)GetValue(DiameterProperty);
            set => SetValue(DiameterProperty, value);
        }

        private static void OnValuePropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is EncoderView encoderView)
            {
                var value = (short)e.NewValue;
                var ratio = value / (float)short.MaxValue;
                encoderView.Angle = value * (360.0 / encoderView.StepCount);
            }
        }

        public static readonly DependencyProperty AngleProperty = DependencyProperty.Register(
            nameof(Angle), typeof(double), typeof(EncoderView), new PropertyMetadata(default(double)));
        public double Angle
        {
            get => (double)GetValue(AngleProperty);
            set => SetValue(AngleProperty, value);
        }

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            nameof(Value), typeof(short), typeof(EncoderView), new PropertyMetadata(default(short), OnValuePropertyChanged));
        public short Value
        {
            get => (short)GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title), typeof(string), typeof(EncoderView), new PropertyMetadata(default(string)));
        public string Title
        {
            get => (string)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }

        public static readonly DependencyProperty StepCountProperty = DependencyProperty.Register(
            nameof(StepCount), typeof(int), typeof(EncoderView), new PropertyMetadata(default(int)));
        public int StepCount
        {
            get => (int) GetValue(StepCountProperty);
            set => SetValue(StepCountProperty, value);
        }

        public static readonly DependencyProperty IsPressedProperty = DependencyProperty.Register(
            nameof(IsPressed), typeof(bool), typeof(EncoderView), new PropertyMetadata(default(bool)));
        public bool IsPressed
        {
            get => (bool) GetValue(IsPressedProperty);
            set => SetValue(IsPressedProperty, value);
        }

        public static readonly DependencyProperty MappedValueProperty = DependencyProperty.Register(
            nameof(MappedValue), typeof(uint), typeof(EncoderView), new PropertyMetadata(default(uint)));
        public uint MappedValue
        {
            get => (uint) GetValue(MappedValueProperty);
            set => SetValue(MappedValueProperty, value);
        }
    }
}
