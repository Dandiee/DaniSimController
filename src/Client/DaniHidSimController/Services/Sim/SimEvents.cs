﻿using System.Diagnostics.CodeAnalysis;

namespace DaniHidSimController.Services.Sim
{
    [SuppressMessage("ReSharper", "InconsistentNaming", Justification = "SimConnect defined names")]
    public enum SimEvents
    {
        AP_ALT_VAR_INC,
        AP_ALT_VAR_DEC,

        AP_VS_VAR_INC,
        AP_VS_VAR_DEC,

        AP_SPD_VAR_INC,
        AP_SPD_VAR_DEC,

        HEADING_BUG_INC,
        HEADING_BUG_DEC
    }
}