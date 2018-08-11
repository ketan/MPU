/* 07/6/2017 Copyright Tlera Corporation
 *  
 * Created by Kris Winer
 *
 * Adapted by Simon D. Levy April 2018
 *  
 * Demonstrate basic MPU-9250 functionality in pass-through mode including
 * parameterizing the register addresses, initializing the sensor, getting
 * properly scaled accelerometer, gyroscope, and magnetometer data out. 
 *
 * SDA and SCL have 4K7 pull-up resistors (to 3.3V).
 *
 * Library may be used freely and without limit with attribution.
 */

#if defined(__MK20DX256__)  
#include <i2c_t3.h>   
#else
#include <Wire.h>   
#endif

#include "MPU9250.h"

/*
   MPU9250 Configuration

   Specify sensor full scale

   Choices are:

Gscale: GFS_250 = 250 dps, GFS_500 = 500 dps, GFS_1000 = 1000 dps, GFS_2000DPS = 2000 degrees per second gyro full scale
Ascale: AFS_2G = 2 g, AFS_4G = 4 g, AFS_8G = 8 g, and AFS_16G = 16 g accelerometer full scale
Mscale: MFS_14BITS = 0.6 mG per LSB and MFS_16BITS = 0.15 mG per LSB
Mmode:  Mmode = M_8Hz for 8 Hz data rate or Mmode = M_100Hz for 100 Hz data rate
SAMPLE_RATE_DIVISOR: (1 + SAMPLE_RATE_DIVISOR) is a simple divisor of the fundamental 1000 kHz rate of the gyro and accel, so 
SAMPLE_RATE_DIVISOR = 0x00 means 1 kHz sample rate for both accel and gyro, 0x04 means 200 Hz, etc.
 */
static const Gscale_t GSCALE    = GFS_250DPS;
static const Ascale_t ASCALE    = AFS_2G;
static const Mscale_t MSCALE    = MFS_16BITS;
static const Mmode_t  MMODE     = M_100Hz;
static const uint8_t SAMPLE_RATE_DIVISOR = 0x04;         

// Pin definitions
static const uint8_t INTERRUPT_PIN = 8;   //  MPU9250 interrupt
static const uint8_t LED_PIN = 13; // red led

// Interrupt support 
static bool gotNewData = false;
static void myinthandler()
{
    gotNewData = true;
}

// Instantiate MPU9250 class in pass-through mode
static MPU9250_Passthru imu(ASCALE, GSCALE, MSCALE, MMODE, SAMPLE_RATE_DIVISOR);

static void error(const char * errmsg) 
{
    Serial.println(errmsg);
    while (true) ;
}

static void reportAcceleration(const char * dim, float val)
{
    Serial.print(dim);
    Serial.print("-acceleration: ");
    Serial.print(1000*val);
    Serial.print(" milliG  "); 
}

static void reportGyroRate(const char * dim, float val)
{
    Serial.print(dim);
    Serial.print("-gyro rate: ");
    Serial.print(val, 1);
    Serial.print(" degrees/sec  "); 
}

static void reportMagnetometer(const char * dim, float val)
{
    Serial.print(dim);
    Serial.print("-magnetometer: ");
    Serial.print(val);
    Serial.print(" milligauss  "); 
}



void setup()
{
    // Start serial comms
    Serial.begin(115200);
    delay(1000);

    // Start I^2C 
#if defined(__MK20DX256__)  
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 400000);
#else
    Wire.begin(); 
    Wire.setClock(400000); 
#endif

    delay(100);

    // Start the MPU9250
    switch (imu.begin()) {

        case MPU_ERROR_IMU_ID:
            error("Bad IMU device ID");
        case MPU_ERROR_MAG_ID:
            error("Bad magnetometer device ID");
        case MPU_ERROR_SELFTEST:
            error("Failed self-test");
        default:
            Serial.println("MPU6050 online!\n");
    }

    delay(100);

    // Set up the interrupt pin, it's set as active high, push-pull
    pinMode(INTERRUPT_PIN, INPUT);

    // Start with orange led on (active HIGH)
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); 

    // Comment out if using pre-measured, pre-stored offset magnetometer biases
    Serial.println("Mag Calibration: Wave device in a figure eight until done!");
    imu.calibrateMagnetometer();

    attachInterrupt(INTERRUPT_PIN, myinthandler, RISING);  // define interrupt for INTERRUPT_PIN output of MPU9250

    digitalWrite(LED_PIN, LOW); // turn off led when using flash memory
}

void loop()
{  
    static float ax, ay, az, gx, gy, gz, mx, my, mz, temperature;

    // If INTERRUPT_PIN goes high, either all data registers have new data
    // or the accel wake on motion threshold has been crossed
    if (true /*gotNewData*/) {   // On interrupt, read data

        gotNewData = false;     

        if (imu.checkNewAccelGyroData())  { 

            imu.readAccelerometer(ax, ay, az);

            imu.readGyrometer(gx, gy, gz);

            temperature = imu.readTemperature();

            if(imu.checkNewMagData()) { 

                imu.readMagnetometer(mx, my, mz);
            }
        }

        // Report at 1Hz
        static uint32_t msec_prev;
        uint32_t msec_curr = millis();

        if (msec_curr-msec_prev > 1000) {

            msec_prev = msec_curr;

            Serial.println();

            reportAcceleration("X", ax);
            reportAcceleration("Y", ay);
            reportAcceleration("Z", az);

            Serial.println();

            reportGyroRate("X", gx);
            reportGyroRate("Y", gy);
            reportGyroRate("Z", gz);

            Serial.println();;

            reportMagnetometer("X", mx);
            reportMagnetometer("Y", my);
            reportMagnetometer("Z", mz);

            Serial.println();;

            // Print temperature in degrees Centigrade      
            Serial.print("Gyro temperature is ");  
            Serial.print(temperature, 1);  
            Serial.println(" degrees C"); 

        }

    } // if got new data
}
