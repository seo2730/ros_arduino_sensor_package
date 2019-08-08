#!/usr/bin/env python
#import slam
import roslib; roslib.load_manifest('arduino_ros')
import subprocess
import rospy
import roslaunch
from arduino_ros.msg import button

count=0
def callback(data):
        num= int(data.button0)
        rospy.loginfo("button : %d",num)
        if num ==1:
                global count
                if count==0:
                        subprocess.call(['python','/home/duckbe/catkin_ws/src/arduino_ros/src/slam.py'])
                        count=1
                else:
                        count=0
                        
def command():
        rospy.init_node("button",anonymous=True)
        rospy.Subscriber("pushed",button,callback)
        rospy.spin()
if __name__ == "__main__":
        try:
                command()

        except rospy.ROSInterruptException:
                pass
