namespace DaniHidSimController.Models
{
    public sealed class SimOptions
    {
        public string BingMapCredentialsProvider { get; set; }
        public string FlightSimulatorProcessName { get; set; }
        public int FlightSimulatorConnectionIntervalInMs { get; set; }
        public int UsbConnectionIntervalInMs { get; set; }
    }
}
