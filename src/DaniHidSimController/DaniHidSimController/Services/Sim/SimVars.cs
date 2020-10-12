using System.Diagnostics.CodeAnalysis;

namespace DaniHidSimController.Services.Sim
{
    [SuppressMessage("ReSharper", "InconsistentNaming", Justification = "SimConnect defined names")]
    public enum SimVars
    {
        AUTOPILOT_MASTER = 0,
        AUTOPILOT_HEADING_LOCK = 1,
        AUTOPILOT_ALTITUDE_LOCK = 2,
        AUTOPILOT_ATTITUDE_HOLD = 3,
        AUTOPILOT_AIRSPEED_HOLD = 4,
        AUTOPILOT_VERTICAL_HOLD = 5,

        GEAR_LEFT_POSITION = 6,
        GEAR_RIGHT_POSITION = 7,
        GEAR_CENTER_POSITION = 8,

        FLAPS_HANDLE_PERCENT = 9,
        BRAKE_LEFT_POSITION = 10,

        BRAKE_PARKING_INDICATOR = 11,
        AUTOPILOT_THROTTLE_ARM = 12,
        AUTOPILOT_YAW_DAMPER = 13,

        GPS_POSITION_LAT = 14,
        GPS_POSITION_LON = 15,
    }
}