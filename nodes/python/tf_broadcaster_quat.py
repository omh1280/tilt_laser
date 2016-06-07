#!/usr/bin/env python
import roslib
roslib.load_manifest('tilt_laser')
import rospy

import tf
from sensor_msgs.msg import Imu
import geometry_msgs.msg

def handle_imu_rotation(msg):
	br = tf.TransformBroadcaster()
	rotation = msg.orientation
	br.sendTransform((0,0,0), (rotation.x, rotation.y, rotation.z, rotation.w), rospy.Time.now(), "laser", "world")

if __name__ == '__main__':
	rospy.init_node('tf_broadcaster')
	rospy.Subscriber('/imu/data', Imu, handle_imu_rotation)
	rospy.spin()
