namespace DaniHidSimController.Models
{
    public enum SimEvents
    {
        AP_ALT_VAR_SET_ENGLISH,
        AP_SPD_VAR_SET,
        HEADING_BUG_SET,
        AP_VS_VAR_SET_METRIC
    }

    public sealed class SimEvent
    {
        public SimEvents Event { get; }
    }
}
