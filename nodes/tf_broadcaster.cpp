#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/Quaternion.h"
#include "tf/transform_datatypes.h"
#include "sensor_msgs/Imu.h"

void handle_imu_rotation(const sensor_msgs::Imu &msg) {
	static tf::TransformBroadcaster br;
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
	tf::Quaternion q;
	geometry_msgs::Quaternion quat = msg.orientation;
	quaternionMsgToTF(quat, q);
	transform.setRotation(q);
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "laser", "world"));
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "tf_broadcaster");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("/imu/data", 3, &handle_imu_rotation);

	ros::spin();
	return 0;
}