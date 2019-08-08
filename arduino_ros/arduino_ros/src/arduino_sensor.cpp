#include "arduino_ros/arduino_sensor.h"


Arduino_sensor::Arduino_sensor(){}

Arduino_sensor::~Arduino_sensor(){}

void Arduino_sensor::SensorCallback(const arduino_ros::arduino::ConstPtr& msg)
{
  //ROS_INFO("1: %d, 2: %d, 3: %d, 4: %d, 5: %d, 6: %d",msg->adc0,msg->adc1,msg->adc2,msg->adc3,msg->adc4,msg->adc5);
  double dust_filter = averageFilter(msg->adc0);
  double dust_umg = dust(dust_filter);
  ROS_INFO("%d, %f, %f",msg->adc0,dust_filter,dust_umg);
}

double Arduino_sensor::averageFilter(int data)
{
   static double m_rawData[MAF_MAX]={0,};
   static double m_sumData=0;
   static double m_mafData=0;
   static int m_mafCNT=0;
    
    m_mafCNT = (m_mafCNT+1)%MAF_MAX;
    m_rawData[m_mafCNT] = data;
    m_sumData =0;
    for(int i=0; i<MAF_MAX; i++){
         m_sumData+= m_rawData[i];
    }
     
    return m_sumData/MAF_MAX;
}

double Arduino_sensor::dust(double data)
{
    double x=(data/bit10)*Vcc;
    double y=0;
    if(x<0.7)
        y=0;
    else
        y=0.0702*pow((double)x,6)-0.8472*pow((double)x,5)+4.0421*pow((double)x,4)-9.6606*pow((double)x,3)+12.03*pow((double)x,2)-7.1036*(double)x+1.557;

    return y;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "arduino_sensor");
    Arduino_sensor sensor;
    sensor.arduino_sensor_sub = sensor.nh.subscribe("adc",1,&Arduino_sensor::SensorCallback,&sensor);
    // 콜백함수 호출을 위한 함수로써, 메시지가 수신되기를 대기,
    // 수신되었을 경우 콜백함수를 실행한다
    ros::spin();
    return 0;
}