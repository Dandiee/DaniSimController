using System.Windows;

namespace DaniHidSimController.Views
{
    public partial class SliderView
    {
        public SliderView()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty KnobWidthProperty = DependencyProperty.Register(
            nameof(KnobWidth), typeof(double), typeof(SliderView), new PropertyMetadata(default(double)));
        public double KnobWidth
        {
            get => (double) GetValue(KnobWidthProperty);
            set => SetValue(KnobWidthProperty, value);
        }

        public static readonly DependencyProperty KnobHeightProperty = DependencyProperty.Register(
            nameof(KnobHeight), typeof(double), typeof(SliderView), new PropertyMetadata(default(double)));
        public double KnobHeight
        {
            get => (double) GetValue(KnobHeightProperty);
            set => SetValue(KnobHeightProperty, value);
        }

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
            nameof(Value), typeof(double), typeof(SliderView), new PropertyMetadata(default(double), PropertyValueChanged));
        
        private static void PropertyValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is SliderView sliderView)
            {
                var value = (double) e.NewValue;
                var range = sliderView.GrfxContainer.ActualHeight - sliderView.KnobHeight;

                sliderView.VerticalOffset = range -
                    Map(value, short.MinValue, short.MaxValue, 0, range);
                sliderView.KnobMargin = new Thickness(0, sliderView.VerticalOffset, 0, 0);
            }
        }

        public double Value
        {
            get => (double) GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public static readonly DependencyProperty VerticalOffsetProperty = DependencyProperty.Register(
            nameof(VerticalOffset), typeof(double), typeof(SliderView), new PropertyMetadata(default(double)));
        public double VerticalOffset
        {
            get => (double) GetValue(VerticalOffsetProperty);
            set => SetValue(VerticalOffsetProperty, value);
        }

        public static readonly DependencyProperty KnobMarginProperty = DependencyProperty.Register(
            nameof(KnobMargin), typeof(Thickness), typeof(SliderView), new PropertyMetadata(default(Thickness)));
        public Thickness KnobMargin
        {
            get => (Thickness) GetValue(KnobMarginProperty);
            set => SetValue(KnobMarginProperty, value);
        }

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title), typeof(string), typeof(SliderView), new PropertyMetadata(default(string)));
        public string Title
        {
            get => (string) GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }
        private static double Map(double x, double in_min, double in_max, double out_min, double out_max)
        {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }
    }
}
