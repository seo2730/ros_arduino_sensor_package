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

//dust sensor 변수 
const int dust_led_pin=40;

//button 변수 
const int button0=52,button1=50,button2=48,button3=46,button4=44;
bool last_reading0,last_reading1,last_reading2,last_reading3,last_reading4,published = true;
long last_debounce_time=0,debounce_delay=50;
static int count=0;

//motor 변수
const int motor_cw = 22;
const int motor_ccw = 24;
const int pan_pwm = 2;
static int ran;

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
  pinMode(pan_pwm,OUTPUT);
  pinMode(motor_cw,OUTPUT);
  pinMode(motor_ccw,OUTPUT);
  
  digitalWrite(button0,HIGH);
  digitalWrite(button1,HIGH);
  digitalWrite(button2,HIGH);
  digitalWrite(button3,HIGH);
  digitalWrite(button4,HIGH);
  digitalWrite(motor_cw,HIGH);
  digitalWrite(motor_ccw,LOW);
  
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
    delayMicroseconds(40);
    digitalWrite(dust_led_pin,HIGH);
    delayMicroseconds(9680);
    
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
  bool reading0=!digitalRead(button0),reading1=!digitalRead(button1),reading2=!digitalRead(button2),
       reading3=!digitalRead(button3),reading4=!digitalRead(button4);

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
      if(button_msg.button1==1)
      {
        switch(count)
        {
          case 0:analogWrite(pan_pwm,85);count++;break;
          case 1:analogWrite(pan_pwm,170);count++;break;
          case 2:analogWrite(pan_pwm,255);count++;break;
          case 3:analogWrite(pan_pwm,ran);count++;break;
          case 4:analogWrite(pan_pwm,0);count=0;break;
        }
      }
      button.publish(&button_msg);
      published = true;
  }
    if(count==4)
      analogWrite(pan_pwm,ran);
      
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
  
  sensor.publish(&sensor_msg);
  //button
  push_button();
  ran = random(0,255);

  nh1.spinOnce();
  nh2.spinOnce();
}
