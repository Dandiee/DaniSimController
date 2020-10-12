namespace DaniHidSimController.Services
{
    public sealed class UsbWriteState
    {
        private readonly byte[] _bytes;
        public UsbWriteState()
        {
            _bytes = new byte[4];
        }

        public bool IsDaniClientConnected { get; set; }
        public bool IsSimConnectConnected { get; set; }

        public bool IsAutopilotMasterEnabled { get; set; }
        public bool IsAutopilotHeadingEnabled { get; set; }
        public bool IsAutopilotAltitudeEnabled { get; set; }
        public bool IsAutopilotAirspeedEnabled { get; set; }
        public bool IsAutopilotVerticalSpeedEnabled { get; set; }
        public bool IsAutopilotYawDamperEnabled { get; set; }

        public bool IsLeftGearMoving { get; set; }
        public bool IsCenterGearMoving { get; set; }
        public bool IsRightGearMoving { get; set; }

        public bool IsLeftGearOut { get; set; }
        public bool IsCenterGearOut { get; set; }
        public bool IsRightGearOut { get; set; }

        public bool IsFlapNonZero { get; set; }
        public bool IsBrakeNonZero { get; set; }

        public bool IsParkingBrakeEnabled { get; set; }

        public bool IsAutoThrottleEnabled { get; set; }


        public byte[] GetState()
        {
            var b1 = 0
                     | ((IsDaniClientConnected ? 1 : 0))
                     | ((IsSimConnectConnected ? 1 : 0) << 1);

            var b2 = 0
                     | ((IsAutopilotMasterEnabled ? 1 : 0))
                     | ((IsAutopilotHeadingEnabled ? 1 : 0) << 1)
                     | ((IsAutopilotAltitudeEnabled ? 1 : 0) << 2)
                     | ((IsAutopilotAirspeedEnabled ? 1 : 0) << 3)
                     | ((IsAutopilotVerticalSpeedEnabled ? 1 : 0) << 4)
                     | ((IsAutopilotYawDamperEnabled ? 1 : 0) << 5);

            var b3 = 0
                     | ((IsLeftGearMoving ? 1 : 0))
                     | ((IsCenterGearMoving ? 1 : 0) << 1)
                     | ((IsRightGearMoving ? 1 : 0) << 2)

                     | ((IsLeftGearOut ? 1 : 0) << 3)
                     | ((IsCenterGearOut ? 1 : 0) << 4)
                     | ((IsRightGearOut ? 1 : 0) << 5)

                     | ((IsFlapNonZero ? 1 : 0) << 6)
                     | ((IsBrakeNonZero ? 1 : 0) << 7);

            var b4 = 0
                     | ((IsParkingBrakeEnabled ? 1 : 0))
                     | ((IsAutoThrottleEnabled ? 1 : 0) << 1);

            _bytes[0] = (byte)b1;
            _bytes[1] = (byte)b2;
            _bytes[2] = (byte)b3;
            _bytes[3] = (byte)b4;

            return _bytes;
        }
    }
}