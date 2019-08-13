#!/usr/bin/env python
#import slam
import roslib; roslib.load_manifest('arduino_ros')
import subprocess
import rospy
import roslaunch
from arduino_ros.msg import button

count0=0
count1=0
def callback(data):
        num0=int(data.button0)
        num1=int(data.button1)
        if num0==1:
                global count0
                if count0==0:
                        subprocess.call(['python','/home/duckbe/catkin_ws/src/arduino_ros/src/slam.py'])
                        count0=1
                else:
                        count0=0

        if num1==1:
                global count1
                if count1==0:
                        rospy.loginfo("1")
                        count1=1
                                
                elif count1==1:
                        rospy.loginfo("2")  
                        count1=2
                                
                elif count1==2:
                        rospy.loginfo("3")
                        count1=3
                                
                elif count1==3:
                        rospy.loginfo("AUTO MODE")
                        count1=4
        
                elif count1==4:
                        rospy.loginfo("OFF")
                        count1=0
def command():
        rospy.init_node("button",anonymous=True)
        rospy.Subscriber("pushed",button,callback)
        rospy.spin()
if __name__ == "__main__":
        try:
                command()

        except rospy.ROSInterruptException:
                pass
