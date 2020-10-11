using System.Windows;
using System.Windows.Media;

namespace DaniHidSimController.Views
{
    public partial class EncoderView
    {
        private const double DefaultDiameter = 50;
        private const double DefaultRadius = DefaultDiameter / 2;
        private const int DefaultStepCount = 24;

        public EncoderView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty RadiusProperty = DependencyProperty.Register(
            nameof(Radius), typeof(double), typeof(EncoderView), new PropertyMetadata(DefaultRadius));
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
            nameof(Diameter), typeof(double), typeof(EncoderView), new PropertyMetadata(DefaultDiameter, OnDiameterPropertyChanged));
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

        public static readonly DependencyProperty StepCountProperty = DependencyProperty.Register(
            nameof(StepCount), typeof(int), typeof(EncoderView), new PropertyMetadata(DefaultStepCount));
        public int StepCount
        {
            get => (int) GetValue(StepCountProperty);
            set => SetValue(StepCountProperty, value);
        }

        public static readonly DependencyProperty KnobColorProperty = DependencyProperty.Register(
            nameof(KnobColor), typeof(Brush), typeof(EncoderView), new PropertyMetadata(default(Brush)));
        public Brush KnobColor
        {
            get => (Brush) GetValue(KnobColorProperty);
            set => SetValue(KnobColorProperty, value);
        }
    }
}
