import roslib; roslib.load_manifest('arduino_ros')
import sys
import subprocess
import rospy
import roslaunch
from arduino_ros.msg import button

launch1=0
launch2=0
launch3=0

def mapping():
        uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
        roslaunch.configure_logging(uuid)

        global launch1,launch2,launch3
        launch1 = roslaunch.parent.ROSLaunchParent(uuid, ["/home/duckbe/catkin_ws/src/romillion/romillion_slam/launch/romillion_slam.launch"])
        launch2 = roslaunch.parent.ROSLaunchParent(uuid, ["/home/duckbe/catkin_ws/src/romillion_explore/explore/launch/explore.launch"])
        launch3 = roslaunch.parent.ROSLaunchParent(uuid, ["/home/duckbe/catkin_ws/src/romillion/romillion_navigation/launch/move_base.launch"])
        
        launch1.start()
        launch2.start()
        launch3.start()
        try:
                launch1.spin()
                launch2.spin()
                launch3.spin()
        finally:
                launch1.shutdown()
                launch2.shutdown()
                launch3.shutdown()


def callback(data):
        num= int(data.button0)
        rospy.loginfo("slam : %d",num)
        if num ==1:
                global launch1,launch2,launch3
                launch1.shutdown()
                launch2.shutdown()
                launch3.shutdown()

def slam():
        rospy.init_node("slam",anonymous=True)
        rospy.Subscriber("pushed",button,callback)
        mapping()

if __name__ == "__main__":
        slam()
