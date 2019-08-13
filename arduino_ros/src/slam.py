import roslib; roslib.load_manifest('arduino_ros')
import sys
import subprocess
import rospy
import roslaunch
from arduino_ros.msg import button

launch1=0
def mapping():
        uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
        roslaunch.configure_logging(uuid)

        global launch1
        launch1 = roslaunch.parent.ROSLaunchParent(uuid, ["/home/duckbe/catkin_ws/src/romillion/romillion_slam/launch/romillion_slam.launch"])
        launch1.start()
        try:
                launch1.spin()
        finally:
                launch1.shutdown()

def callback(data):
        num= int(data.button0)
        if num ==1:
                global launch1
                launch1.shutdown()

def slam():
        rospy.init_node("slam",anonymous=True)
        rospy.Subscriber("pushed",button,callback)
        mapping()

if __name__ == "__main__":
        slam()
