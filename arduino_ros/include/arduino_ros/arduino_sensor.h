#ifndef ARDUINO_SENSOR_H_
#define ARDUINO_SENSOR_H_

#include "ros/ros.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/Float32.h>
#include "arduino_ros/arduino.h"

#define bit10 1023.0
#define Vcc 5.0
#define MAF_MAX 30

class Arduino_sensor
{
 public:
   Arduino_sensor();
   ~Arduino_sensor();
   ros::NodeHandle nh;
   ros::Subscriber arduino_sensor_sub;
   void SensorCallback(const arduino_ros::arduino::ConstPtr& msg);
   double averageFilter(int data);
   double com_Filter(int data);
   double dust(double data);

  //double dust_sensor;
};

#endif //ARDUINO_SENSOR_H_