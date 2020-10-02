namespace DaniHidSimController.Models
{
    public enum SimEvents
    {
        AP_ALT_VAR_SET_ENGLISH,
        AP_SPD_VAR_SET,
        HEADING_BUG_SET,
        AP_VS_VAR_SET_METRIC,

        AP_ALT_VAR_INC,
        AP_ALT_VAR_DEC,

        AP_VS_VAR_INC,
        AP_VS_VAR_DEC,

        AP_SPD_VAR_INC,
        AP_SPD_VAR_DEC,

        HEADING_BUG_INC,
        HEADING_BUG_DEC
    }

    public sealed class SimEvent
    {
        public SimEvents Event { get; }
    }
}
