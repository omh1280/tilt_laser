#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/Quaternion.h"
#include "tf/transform_datatypes.h"
#include "sensor_msgs/Imu.h"

void handle_imu_rotation(const sensor_msgs::Imu &msg) {
	// Creat tf broadcaster
	static tf::TransformBroadcaster br;
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
	tf::Quaternion q;
	geometry_msgs::Quaternion quat = msg.orientation;
	// Conversion function for quaternions
	quaternionMsgToTF(quat, q);
	// Below is crucial transformation -- I don't know why though
	tf::Vector3 axis = q.getAxis();
	axis.setY(-axis.y());
	tf::Quaternion q2(axis, -q.getAngle());
	// Set the transform
	transform.setRotation(q2.normalize());
	// Publish to tf
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "base_link"));
	//tf::Transform none;
	//none.setOrigin(tf::Vector3(0.0,0.0,0.0));
	//br.sendTransform(tf::StampedTransform(none, ros::Time::now(), "base_link", "laser"));
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "tf_broadcaster");
	ros::NodeHandle n;
	
	// Subscribe to Imu and set callback to publish tf data
	ros::Subscriber sub = n.subscribe("/imu/data", 4, &handle_imu_rotation);

	ros::spin();
	return 0;
}