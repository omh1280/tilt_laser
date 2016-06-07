#!/usr/bin/env python
import roslib
roslib.load_manifest('tilt_laser')
import rospy

import tf
import sensor_msgs.msg
import geometry_msgs.msg
import math
from tf.transformations import quaternion_multiply

x = 1
y = 0
z = 0
w = 0

def quat_from_angular_vel(vel):
	global x, y, z, w
	angle = math.sqrt(vel[0]**2 + vel[1]**2 + vel[2]**2)
	if angle > 0.0:
		ret = (vel[0] * math.sin(angle/2.0)/angle,
                       vel[1] * math.sin(angle/2.0)/angle,
                       vel[2] * math.sin(angle/2.0)/angle,
                       math.cos(angle/2.0))
	else:
		ret = (0.0, 0.0, 0.0, 1.0)
	ret = quaternion_multiply((x,y,z,w), ret)
	x = ret[0]
	y = ret[1]
	z = ret[2]
	w = ret[3]
	return ret
	
def handle_imu_rotation(msg):
	br = tf.TransformBroadcaster()
	#rotation = msg.orientation
	vel = msg.angular_velocity
	hz = .01
	br.sendTransform((0,0,0), quat_from_angular_vel((vel.x*hz, vel.y*hz, vel.z*hz)), rospy.Time.now(), "laser", "world")

if __name__ == '__main__':
	rospy.init_node('tf_broadcaster')
	rospy.Subscriber('/imu/data', sensor_msgs.msg.Imu, handle_imu_rotation)
	rospy.spin()
