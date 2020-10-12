namespace DaniHidSimController.Models
{
    public sealed class SimOptions
    {
        public string BingMapCredentialsProvider { get; set; }
        public string FlightSimulatorProcessName { get; set; }
        public string DeviceName { get; set; }
        public int FlightSimulatorConnectionIntervalInMs { get; set; }
        public int UsbConnectionIntervalInMs { get; set; }
        public int BindMapLocationUpdateCooldownInMs { get; set; }
    }
}
