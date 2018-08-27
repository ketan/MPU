/* MPU/6000/6050 library common header code
by: Kris Winer
date: May 1, 2014
updated: 2018 Simon D. Levy
license: Beerware - Use this code however you'd like. If you
find it useful you can buy me a beer some time.
*/

#include <stdint.h>
#include <stdbool.h>

#include "MPU.h"


class MPU60x0 {

    private:

        const uint8_t XGOFFS_TC        		= 0x00; // Bit 7 PWR_MODE, bits 6:1 XG_OFFS_TC, bit 0 OTP_BNK_VLD;
        const uint8_t YGOFFS_TC        		= 0x01;
        const uint8_t ZGOFFS_TC        		= 0x02;
        const uint8_t X_FINE_GAIN      		= 0x03; // [7:0] fine gain
        const uint8_t Y_FINE_GAIN      		= 0x04;
        const uint8_t Z_FINE_GAIN      		= 0x05;
        const uint8_t XA_OFFSET_H      		= 0x06; // User-defined trim values for accelerometer
        const uint8_t XA_OFFSET_L_TC   		= 0x07;
        const uint8_t YA_OFFSET_H      		= 0x08;
        const uint8_t YA_OFFSET_L_TC   		= 0x09;
        const uint8_t ZA_OFFSET_H      		= 0x0A;
        const uint8_t ZA_OFFSET_L_TC   		= 0x0B;
        const uint8_t SELF_TEST_X      		= 0x0D;
        const uint8_t SELF_TEST_Y      		= 0x0E;
        const uint8_t SELF_TEST_Z      		= 0x0F;
        const uint8_t SELF_TEST_A      		= 0x10;
        const uint8_t XG_OFFS_USRH     		= 0x13;  // User-defined trim values for gyroscope; supported in MPU-60x0?
        const uint8_t XG_OFFS_USRL     		= 0x14;
        const uint8_t YG_OFFS_USRH     		= 0x15;
        const uint8_t YG_OFFS_USRL     		= 0x16;
        const uint8_t ZG_OFFS_USRH     		= 0x17;
        const uint8_t ZG_OFFS_USRL     		= 0x18;
        const uint8_t SMPLRT_DIV       		= 0x19;
        const uint8_t CONFIG           		= 0x1A;
        const uint8_t GYRO_CONFIG      		= 0x1B;
        const uint8_t ACCEL_CONFIG     		= 0x1C;
        const uint8_t FF_THR           		= 0x1D;  // Free-fall
        const uint8_t FF_DUR           		= 0x1E;  // Free-fall
        const uint8_t MOT_THR          		= 0x1F;  // Motion detection threshold bits [7:0]
        const uint8_t MOT_DUR          		= 0x20;  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
        const uint8_t ZMOT_THR         		= 0x21;  // Zero-motion detection threshold bits [7:0]
        const uint8_t ZRMOT_DUR        		= 0x22;  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms
        const uint8_t FIFO_EN          		= 0x23;
        const uint8_t I2C_MST_CTRL     		= 0x24;
        const uint8_t I2C_SLV0_ADDR    		= 0x25;
        const uint8_t I2C_SLV0_REG     		= 0x26;
        const uint8_t I2C_SLV0_CTRL    		= 0x27;
        const uint8_t I2C_SLV1_ADDR    		= 0x28;
        const uint8_t I2C_SLV1_REG     		= 0x29;
        const uint8_t I2C_SLV1_CTRL    		= 0x2A;
        const uint8_t I2C_SLV2_ADDR    		= 0x2B;
        const uint8_t I2C_SLV2_REG     		= 0x2C;
        const uint8_t I2C_SLV2_CTRL    		= 0x2D;
        const uint8_t I2C_SLV3_ADDR    		= 0x2E;
        const uint8_t I2C_SLV3_REG     		= 0x2F;
        const uint8_t I2C_SLV3_CTRL    		= 0x30;
        const uint8_t I2C_SLV4_ADDR    		= 0x31;
        const uint8_t I2C_SLV4_REG     		= 0x32;
        const uint8_t I2C_SLV4_DO      		= 0x33;
        const uint8_t I2C_SLV4_CTRL    		= 0x34;
        const uint8_t I2C_SLV4_DI      		= 0x35;
        const uint8_t I2C_MST_STATUS   		= 0x36;
        const uint8_t INT_PIN_CFG      		= 0x37;
        const uint8_t INT_ENABLE       		= 0x38;
        const uint8_t DMP_INT_STATUS   		= 0x39;  // Check DMP interrupt
        const uint8_t INT_STATUS       		= 0x3A;
        const uint8_t ACCEL_XOUT_H     		= 0x3B;
        const uint8_t ACCEL_XOUT_L     		= 0x3C;
        const uint8_t ACCEL_YOUT_H     		= 0x3D;
        const uint8_t ACCEL_YOUT_L     		= 0x3E;
        const uint8_t ACCEL_ZOUT_H     		= 0x3F;
        const uint8_t ACCEL_ZOUT_L     		= 0x40;
        const uint8_t TEMP_OUT_H       		= 0x41;
        const uint8_t TEMP_OUT_L       		= 0x42;
        const uint8_t GYRO_XOUT_H      		= 0x43;
        const uint8_t GYRO_XOUT_L      		= 0x44;
        const uint8_t GYRO_YOUT_H      		= 0x45;
        const uint8_t GYRO_YOUT_L      		= 0x46;
        const uint8_t GYRO_ZOUT_H      		= 0x47;
        const uint8_t GYRO_ZOUT_L      		= 0x48;
        const uint8_t EXT_SENS_DATA_00 		= 0x49;
        const uint8_t EXT_SENS_DATA_01 		= 0x4A;
        const uint8_t EXT_SENS_DATA_02 		= 0x4B;
        const uint8_t EXT_SENS_DATA_03 		= 0x4C;
        const uint8_t EXT_SENS_DATA_04 		= 0x4D;
        const uint8_t EXT_SENS_DATA_05 		= 0x4E;
        const uint8_t EXT_SENS_DATA_06 		= 0x4F;
        const uint8_t EXT_SENS_DATA_07 		= 0x50;
        const uint8_t EXT_SENS_DATA_08 		= 0x51;
        const uint8_t EXT_SENS_DATA_09 		= 0x52;
        const uint8_t EXT_SENS_DATA_10 		= 0x53;
        const uint8_t EXT_SENS_DATA_11 		= 0x54;
        const uint8_t EXT_SENS_DATA_12 		= 0x55;
        const uint8_t EXT_SENS_DATA_13 		= 0x56;
        const uint8_t EXT_SENS_DATA_14 		= 0x57;
        const uint8_t EXT_SENS_DATA_15 		= 0x58;
        const uint8_t EXT_SENS_DATA_16      = 0x59;
        const uint8_t EXT_SENS_DATA_17      = 0x5A;
        const uint8_t EXT_SENS_DATA_18      = 0x5B;
        const uint8_t EXT_SENS_DATA_19      = 0x5C;
        const uint8_t EXT_SENS_DATA_20      = 0x5D;
        const uint8_t EXT_SENS_DATA_21      = 0x5E;
        const uint8_t EXT_SENS_DATA_22      = 0x5F;
        const uint8_t EXT_SENS_DATA_23      = 0x60;
        const uint8_t MOT_DETECT_STATUS     = 0x61;
        const uint8_t I2C_SLV0_DO           = 0x63;
        const uint8_t I2C_SLV1_DO           = 0x64;
        const uint8_t I2C_SLV2_DO           = 0x65;
        const uint8_t I2C_SLV3_DO           = 0x66;
        const uint8_t I2C_MST_DELAY_CTRL    = 0x67;
        const uint8_t SIGNAL_PATH_RESET     = 0x68;
        const uint8_t MOT_DETECT_CTRL       = 0x69;
        const uint8_t USER_CTRL             = 0x6A;  // Bit 7 enable DMP, bit 3 reset DMP
        const uint8_t PWR_MGMT_1            = 0x6B; // Device defaults to the SLEEP mode
        const uint8_t PWR_MGMT_2            = 0x6C;
        const uint8_t DMP_BANK              = 0x6D;  // Activates a specific bank in the DMP
        const uint8_t DMP_RW_PNT            = 0x6E;  // Set read/write pointer to a specific start address in specified DMP bank
        const uint8_t DMP_REG               = 0x6F;  // Register in DMP from which to read or to which to write
        const uint8_t DMP_REG_1             = 0x70;
        const uint8_t DMP_REG_2             = 0x71;
        const uint8_t FIFO_COUNTH           = 0x72;
        const uint8_t FIFO_COUNTL           = 0x73;
        const uint8_t FIFO_R_W              = 0x74;
        const uint8_t WHO_AM_I              = 0x75; // Should return = 0x68

        Ascale_t _aScale;
        Gscale_t _gScale;
        float    _aRes;
        float    _gRes;
        float    _accelBias[3];
        float    _gyroBias[3];

        void     calibrate(float accelBias[3], float gyroBias[3]);
        void     init(Ascale_t ascale, Gscale_t gscale);
        float    getGres(Gscale_t gscale);
        float    getAres(Ascale_t ascale);
        uint8_t  getId(void);
        void     selfTest(float * destination);

        uint8_t  readRegister(uint8_t subAddress);

        // Cross-platform support
        virtual void     writeRegister(uint8_t subAddress, uint8_t data) = 0;
        virtual void     readRegisters(uint8_t subAddress, uint8_t count, uint8_t * dest) = 0;

    protected:

        const uint8_t MPU60x0_ADDRESS = 0x68;

        MPU60x0(Ascale_t ascale, Gscale_t gscale);

        MPU_Error_t begin(void);

    public:

        bool        checkNewData(void);
        void        lowPowerAccelOnly(void);

        void        readAccelerometer(float & ax, float & ay, float & az);
        void        readGyrometer(float & gx, float & gy, float & gz);
        float       readTemperature(void);

}; // class MPU60x0