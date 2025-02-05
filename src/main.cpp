#include <Arduino.h>
#include <Accelerometer.h>

//=========================================================
// CONFIGURATION
//=========================================================

#define PLOTTING_PERIOD 10

//=========================================================
// CONFIGURATION END
//=========================================================

void control_tick();
void plot_tick();

#define TYPE_NO_OUTPUT 0
#define TYPE_CALIBRATION_INFO 1
#define TYPE_LINEAR_SPEED 2
#define RIGHT_ACCEL_TWI_ADRESS 0x29
#define LEFT_ACCEL_TWI_ADRESS 0x28

int plottingRequired = TYPE_NO_OUTPUT;
Accelerometer rightFoot(1, RIGHT_ACCEL_TWI_ADRESS);
Accelerometer leftFoot(1, LEFT_ACCEL_TWI_ADRESS);

void setup()
{
  Serial.begin(2000000);
  delay(1000);

  rightFoot.begin();
  leftFoot.begin();

  Serial.println("Calibrating");
  unsigned long calibrationTimer = millis();
  while (millis() - calibrationTimer < 3000)
  {
    rightFoot.calibrate();
    leftFoot.calibrate();
    Serial.print(".");
    delay(50);
  }
  Serial.println("\nDone!");
}

void loop()
{
  control_tick();
  plot_tick();
}

unsigned long timer = millis();
void control_tick()
{
  if (Serial.available() > 0)
  {
    char mess = Serial.read();

    if (mess == 'c')
      plottingRequired = TYPE_CALIBRATION_INFO;

    if (mess == 'l')
      plottingRequired = TYPE_LINEAR_SPEED;

    if (mess == 'n')
      plottingRequired = TYPE_NO_OUTPUT;
  }
}

void plot_tick()
{
  if (millis() >= timer)
  {
    timer = millis() + PLOTTING_PERIOD;

    if (plottingRequired == TYPE_LINEAR_SPEED)
    {
      rightFoot.update();
      leftFoot.update();
      imu::Vector<3> rightData = rightFoot.getLinAccel();
      imu::Vector<3> leftData = leftFoot.getLinAccel();

      Serial.print(String(rightData.x()) + ",");
      Serial.print(String(rightData.y()) + ",");
      Serial.print(String(rightData.z()) + ",");

      Serial.print(String(leftData.x()) + ",");
      Serial.print(String(leftData.y()) + ",");
      Serial.println(String(leftData.z()) + ",");
    }

    if (plottingRequired == TYPE_CALIBRATION_INFO)
    {
      // Serial.print("right is\t" + rightFoot.isCal() ? "Calibrated well\t": "Not calibrated\t");
      // Serial.println("left is\t" + leftFoot.isCal() ? "Calibrated well\t": "Not calibrated\t");

      rightFoot.printCalibration();
      leftFoot.printCalibration();
      Serial.println();

    }
  }
}