using System;

namespace DaniHidSimController.ViewModels.IoComponents
{
    public struct Pin
    {
        public int Index { get; }
        public bool IsGpio { get; }

        public Pin(int index, bool isGpio)
        {
            Index = index;
            IsGpio = isGpio;
        }

        public bool Equals(Pin other)
        {
            return Index == other.Index && IsGpio == other.IsGpio;
        }

        public override bool Equals(object obj)
        {
            return obj is Pin other && Equals(other);
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(Index, IsGpio);
        }

        public static readonly Pin Gpio0 = new Pin(0, true);
        public static readonly Pin Gpio1 = new Pin(1, true);
        public static readonly Pin Gpio2 = new Pin(2, true);
        public static readonly Pin Gpio3 = new Pin(3, true);
        public static readonly Pin Gpio4 = new Pin(4, true);
        public static readonly Pin Gpio5 = new Pin(5, true);
        public static readonly Pin Gpio6 = new Pin(6, true);
        public static readonly Pin Gpio7 = new Pin(7, true);
        public static readonly Pin Gpio8 = new Pin(8, true);
        public static readonly Pin Gpio9 = new Pin(9, true);
        public static readonly Pin Gpio10 = new Pin(10, true);
        public static readonly Pin Gpio11 = new Pin(11, true);
        public static readonly Pin Gpio12 = new Pin(12, true);
        public static readonly Pin Gpio13 = new Pin(13, true);
        public static readonly Pin Gpio14 = new Pin(14, true);
        public static readonly Pin Gpio15 = new Pin(15, true);


        public static readonly Pin BuiltIn0 = new Pin(0, false);
        public static readonly Pin BuiltIn1 = new Pin(1, false);
        public static readonly Pin BuiltIn2 = new Pin(2, false);
        public static readonly Pin BuiltIn3 = new Pin(3, false);
        public static readonly Pin BuiltIn4 = new Pin(4, false);
        public static readonly Pin BuiltIn5 = new Pin(5, false);
        public static readonly Pin BuiltIn6 = new Pin(6, false);
        public static readonly Pin BuiltIn7 = new Pin(7, false);
        public static readonly Pin BuiltIn8 = new Pin(8, false);
        public static readonly Pin BuiltIn9 = new Pin(9, false);
        public static readonly Pin BuiltIn10 = new Pin(10, false);
        public static readonly Pin BuiltIn11 = new Pin(11, false);
        public static readonly Pin BuiltIn12 = new Pin(12, false);
        public static readonly Pin BuiltIn13 = new Pin(13, false);
        public static readonly Pin BuiltIn14 = new Pin(14, false);
        public static readonly Pin BuiltIn15 = new Pin(15, false);

    }
}
