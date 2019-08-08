#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
//내가 만든 msg 파일 
#include <arduino_ros/arduino.h>
#include <arduino_ros/button.h>

ros::NodeHandle nh1,nh2;
arduino_ros::arduino sensor_msg;
arduino_ros::button button_msg;

ros::Publisher sensor("adc", &sensor_msg);
ros::Publisher button("pushed", &button_msg);

const int button0=52,button1=50,button2=48,button3=46,button4=44;
const int dust_led_pin=40;

bool last_reading0,last_reading1,last_reading2,last_reading3,last_reading4;
long last_debounce_time=0;
long debounce_delay=50;
bool published = true;

void setup()
{ 
  nh1.initNode(); 
  nh2.initNode(); 
  nh1.advertise(sensor);
  nh1.advertise(button); 
  
  pinMode(dust_led_pin, OUTPUT);
  pinMode(button0,INPUT);
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  
  digitalWrite(button0,HIGH);
  digitalWrite(button1,HIGH);
  digitalWrite(button2,HIGH);
  digitalWrite(button3,HIGH);
  digitalWrite(button4,HIGH);
  
  last_reading0 != digitalRead(button0);
  last_reading1 != digitalRead(button1);
  last_reading2!= digitalRead(button2);
  last_reading3 != digitalRead(button3);
  last_reading4 != digitalRead(button4);
  
}

//Only use dust sensor 
int averageAnalog_dust(int pin){
    double v;
    digitalWrite(dust_led_pin,LOW);
    delayMicroseconds(280);
    v = analogRead(pin);
    digitalWrite(dust_led_pin,HIGH);
    delayMicroseconds(40);
    
    return v;
}

//Get sensor data
int averageAnalog(int pin){
    double v;
    v= analogRead(pin);
    return v;
}

void push_button()
{
  bool reading0=!digitalRead(button0);
  bool reading1=!digitalRead(button1);
  bool reading2=!digitalRead(button2);
  bool reading3=!digitalRead(button3);
  bool reading4=!digitalRead(button4);

  if (last_reading0 != reading0 || last_reading1 != reading1 || last_reading2 != reading2 || last_reading3 != reading3 || last_reading4 != reading4)
  {
      last_debounce_time = millis();
      published = false;
  }
  //if the button value has not changed for the debounce delay, we know its stable
  if (!published && (millis() - last_debounce_time)  > debounce_delay)
  {
      button_msg.button0=reading0;
      button_msg.button1=reading1;
      button_msg.button2=reading2;
      button_msg.button3=reading3;
      button_msg.button4=reading4;
      button.publish(&button_msg);
      published = true;
  }

    last_reading0 = reading0;
    last_reading1 = reading1;
    last_reading2 = reading2;
    last_reading3 = reading3;
    last_reading4 = reading4;
}

void loop()
{
  //sensor
  sensor_msg.adc0 = averageAnalog_dust(0);//dust sensor
  //arduino_msg.adc1 = averageAnalog(1);//sensor
  
  //button
  push_button();

  sensor.publish(&sensor_msg);
  
  nh1.spinOnce();
  nh2.spinOnce();
}
