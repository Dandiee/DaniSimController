namespace SimVarGenerator
{
    public sealed class SimVarType
    {

        public static readonly SimVarType Bool = new SimVarType("Bool", "Bool");
        public static readonly SimVarType FeetPerSecond = new SimVarType("Feet per second", "Feet Per Second");
        public static readonly SimVarType PercentOver100 = new SimVarType("Percent over 100", "Percent Over 100");
        public static readonly SimVarType Number = new SimVarType("Number", "Number");
        public static readonly SimVarType Gallons = new SimVarType("Gallons", "Gallons");
        public static readonly SimVarType String = new SimVarType("String", "String");
        public static readonly SimVarType Boolperstring = new SimVarType("Bool/String", "Boolperstring");
        public static readonly SimVarType Feet = new SimVarType("Feet", "Feet");
        public static readonly SimVarType SimconnectDataXyz = new SimVarType("SIMCONNECT_DATA_XYZ", "Simconnect Data Xyz");
        public static readonly SimVarType Mask = new SimVarType("Mask", "Mask");
        public static readonly SimVarType Knots = new SimVarType("Knots", "Knots");
        public static readonly SimVarType SimconnectDataWaypoint = new SimVarType("SIMCONNECT_DATA_WAYPOINT", "Simconnect Data Waypoint");
        public static readonly SimVarType Degrees = new SimVarType("Degrees", "Degrees");
        public static readonly SimVarType Seconds = new SimVarType("Seconds", "Seconds");
        public static readonly SimVarType Boolean = new SimVarType("Boolean", "Boolean");
        public static readonly SimVarType SimconnectDataLatlonalt = new SimVarType("SIMCONNECT_DATA_LATLONALT", "Simconnect Data Latlonalt");
        public static readonly SimVarType Percent = new SimVarType("Percent", "Percent");
        public static readonly SimVarType Enum = new SimVarType("Enum", "Enum");
        public static readonly SimVarType Rpm = new SimVarType("Rpm", "Rpm");
        public static readonly SimVarType Rankine = new SimVarType("Rankine", "Rankine");
        public static readonly SimVarType Psi = new SimVarType("Psi", "Psi");
        public static readonly SimVarType Hours = new SimVarType("Hours", "Hours");
        public static readonly SimVarType Position = new SimVarType("Position", "Position");
        public static readonly SimVarType FtLbPerSecond = new SimVarType("ft lb per second", "Ft Lb Per Second");
        public static readonly SimVarType FootPound = new SimVarType("Foot pound", "Foot Pound");
        public static readonly SimVarType Celsius = new SimVarType("Celsius", "Celsius");
        public static readonly SimVarType PoundsPerHour = new SimVarType("Pounds per hour", "Pounds Per Hour");
        public static readonly SimVarType Ratio = new SimVarType("Ratio", "Ratio");
        public static readonly SimVarType Pounds = new SimVarType("Pounds", "Pounds");
        public static readonly SimVarType Radians = new SimVarType("Radians", "Radians");
        public static readonly SimVarType Ratio016384 = new SimVarType("Ratio (0-16384)", "Ratio (0 16384)");
        public static readonly SimVarType PoundForcePerSquareInch = new SimVarType("pound-force per square inch", "Pound Force Per Square Inch");
        public static readonly SimVarType Inhg = new SimVarType("inHg", "Inhg");
        public static readonly SimVarType Percentage = new SimVarType("Percentage", "Percentage");
        public static readonly SimVarType FeetPerSecondSquared = new SimVarType("Feet per second squared", "Feet Per Second Squared");
        public static readonly SimVarType Meters = new SimVarType("Meters", "Meters");
        public static readonly SimVarType Mach = new SimVarType("Mach", "Mach");
        public static readonly SimVarType Millibars = new SimVarType("Millibars", "Millibars");
        public static readonly SimVarType RadiansPerSecond = new SimVarType("Radians per second", "Radians Per Second");
        public static readonly SimVarType Gforce = new SimVarType("Gforce", "Gforce");
        public static readonly SimVarType FrequencyBcd16 = new SimVarType("Frequency BCD16", "Frequency Bcd16");
        public static readonly SimVarType Mhz = new SimVarType("MHz", "Mhz");
        public static readonly SimVarType NauticalMiles = new SimVarType("Nautical miles", "Nautical Miles");
        public static readonly SimVarType FrequencyAdfBcd32 = new SimVarType("Frequency ADF BCD32", "Frequency Adf Bcd32");
        public static readonly SimVarType Hz = new SimVarType("Hz", "Hz");
        public static readonly SimVarType Bco16 = new SimVarType("BCO16", "Bco16");
        public static readonly SimVarType MetersPerSecond = new SimVarType("Meters per second", "Meters Per Second");
        public static readonly SimVarType Flags = new SimVarType("Flags", "Flags");
        public static readonly SimVarType Psf = new SimVarType("psf", "Psf");
        public static readonly SimVarType Feetperminute = new SimVarType("Feet/minute", "Feetperminute");
        public static readonly SimVarType SlugsPerCubicFeet = new SimVarType("Slugs per cubic feet", "Slugs Per Cubic Feet");
        public static readonly SimVarType Amperes = new SimVarType("Amperes", "Amperes");
        public static readonly SimVarType Volts = new SimVarType("Volts", "Volts");
        public static readonly SimVarType PoundForcePerSquareFoot = new SimVarType("Pound force per square foot", "Pound Force Per Square Foot");
        public static readonly SimVarType FeetPerMinute = new SimVarType("Feet per minute", "Feet Per Minute");
        public static readonly SimVarType PoundsPerSquareFoot = new SimVarType("Pounds per square foot", "Pounds Per Square Foot");
        public static readonly SimVarType SquareFeet = new SimVarType("Square feet", "Square Feet");
        public static readonly SimVarType PerRadian = new SimVarType("Per radian", "Per Radian");
        public static readonly SimVarType Machs = new SimVarType("Machs", "Machs");
        public static readonly SimVarType SlugFeetSquared = new SimVarType("slug feet squared", "Slug Feet Squared");
        public static readonly SimVarType Amps = new SimVarType("Amps", "Amps");
        public static readonly SimVarType PerSecond = new SimVarType("Per second", "Per Second");
        public static readonly SimVarType String64 = new SimVarType("String64", "String64");
        public static readonly SimVarType String8 = new SimVarType("String8", "String8");
        public static readonly SimVarType VariableLengthString = new SimVarType("Variable length string", "Variable Length String");

        public string Name { get; }
        public string FriendlyName { get; }

        private SimVarType(string name, string friendlyName)
        {
            Name = name;
            FriendlyName = friendlyName;
        }
    }
}