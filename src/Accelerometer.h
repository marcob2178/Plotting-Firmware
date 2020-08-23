#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Adafruit_BNO055.h>

class Accelerometer
{
private:
    Adafruit_BNO055 *bno;
    imu::Vector<3> euler;
    imu::Vector<3> linaccel;

    double offset_y, offset_z, offset_x;

    //  x       y       z
    //  yaw     pitch   roll
    void setOffsets(double _offset_y, double _offset_z, double _offset_x)
    {
        offset_y = _offset_y;
        offset_z = _offset_z;
        offset_x = _offset_x;
    }

public:
    Accelerometer(int id, int address)
    {
        bno = new Adafruit_BNO055(id, address);
    };

    void calibrate()
    {
        euler = bno->getVector(Adafruit_BNO055::VECTOR_EULER);
        setOffsets(euler.y(), euler.z(), euler.x());
    }

    void begin()
    {
        if (!bno->begin())
        {
            Serial.print("Ooops, no BNO055 1 detected ... Check your wiring or I2C ADDR!");
            return;
            while (1)
                ;
        }
        //bno->setExtCrystalUse(false);
        delay(500);

        calibrate();
    }

    imu::Vector<3> getLinAccel()
    {
        return linaccel;
    }

    double getYaw()
    {
        return euler.x() - offset_x;
    }

    double getPitch()
    {
        return euler.y() - offset_y;
    }

    double getRoll()
    {
        return euler.z() - offset_z;
    }

    void update()
    {
        euler = bno->getVector(Adafruit_BNO055::VECTOR_EULER);
        linaccel = bno->getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
        // delayMicroseconds(500);
    }

    bool isCal()
    {
        return bno->isFullyCalibrated();
    }

    void printCalibration()
    {
        uint8_t system, gyro, accel, mag;
        system = gyro = accel = mag = 0;
        bno->getCalibration(&system, &gyro, &accel, &mag);

        /* The data should be ignored until the system calibration is > 0 */
        // Serial.print("\t");
        // if (!system)
        // {
        //     Serial.print("! ");
        // }

        /* Display the individual values */
        Serial.print("Sys:");
        Serial.print(system, DEC);
        Serial.print(" G:");
        Serial.print(gyro, DEC);
        Serial.print(" A:");
        Serial.print(accel, DEC);
        Serial.print(" M:");
        Serial.print(mag, DEC);
        Serial.print("\t");
    }
};
#endif