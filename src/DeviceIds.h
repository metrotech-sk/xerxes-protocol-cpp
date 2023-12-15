#ifndef DEVIDS_H
#define DEVIDS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    typedef uint8_t devid_t;

    /**
     * @brief Device IDs for the different sensors
     *
     */
    enum DeviceId : devid_t
    {
        /** temperature sensor ds18b20 */
        DEVID_TEMP_DS18B20 = 0x02,

        /** pressure sensor range 0-600mbar, output in Pa, 2 external temperature sensors -50/150°C output: mK */
        DEVID_PRESSURE_600MBAR = 0x03,

        /** pressure sensor range 0-60mbar, output in Pa, 2 external temperature sensors -50/150°C output: mK */
        DEVID_PRESSURE_60MBAR = 0x04,

        /** strain-gauge sensor range 0-2^24, 2 external temperature sensors -50/150°C output: mK */
        DEVID_STRAIN_24BIT = 0x11,

        /** I/O device, 8DI/8DO (8xDigital Input, 8xDigital 0utput) */
        DEVID_IO_8DI_8DO = 0x20,

        /** I/O device, 4DI/4DO (4xDigital Input, 4xDigital 0utput) */
        DEVID_IO_4DI_4DO = 0x21,

        /** I/O device, 4AI (4xAnalog Input) */
        DEVID_IO_4AI = 0x22,

        /** 3x Analog input, discrete 16bit */
        DEVID_IO_3AI = 0x23,

        /** Encoder reader */
        DEVID_ENC_1000PPR = 0x2A,

        /** Cutter */
        DEVID_CUTTER = 0x2B,

        /** Welder */
        DEVID_WELDER = 0x2C,

        /** Inclinometer SCL3300 */
        DEVID_ANGLE_XY_90 = 0x30,

        /** Inclinometer SCL3400 */
        DEVID_ANGLE_XY_30 = 0x31,

        /** SCL3300 in accelerometer mode*/
        DEVID_ACCEL_XYZ = 0x32,

        /** Distance sensor 0-22mm, resistive, linear*/
        DEVID_DIST_22MM = 0x40,

        /** Distance sensor 0-225mm, resistive, linear*/
        DEVID_DIST_225MM = 0x41,

        /** Air polution sensor*/
        DEVID_AIR_POL_CO_NOX_VOC = 0x50,
        DEVID_AIR_POL_PM = 0x51,
        DEVID_AIR_POL_CO_NOX_VOC_PM = 0x52,
        DEVID_AIR_POL_CO_NOX_VOC_PM_GPS = 0x53,

        DEVID_LIGHT_SOUND_POLLUTION = 0x60,
    };

#ifdef __cplusplus
}
#endif

#endif /* DEVIDS_H */
