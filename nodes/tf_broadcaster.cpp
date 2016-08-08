#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/Quaternion.h"
#include "tf/transform_datatypes.h"

void handle_imu_rotation(const sensor_msgs::Imu &msg) 
{
	// Creat tf broadcaster

	static tf::TransformBroadcaster br;
	
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
	
	tf::Quaternion q;
	
	geometry_msgs::Quaternion quat = msg.orientation;

	// Conversion function for quaternions
	quaternionMsgToTF(quat, q);
	tf::Matrix3x3 m(q);
	
	double roll, pitch, yaw;
	
	m.getRPY(roll, pitch, yaw);
	
	tf::Quaternion q2;
	q2.setRPY(roll, pitch, 0.0);
		
	transform.setRotation(q2.normalize());
	
	// Publish to tf
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "base_link"));
}

int main(int argc, char **argv) 
{
	ros::init(argc, argv, "tf_broadcaster");
	ros::NodeHandle n;
	
	// Subscribe to Imu and set callback to publish tf data
	ros::Subscriber sub = n.subscribe("/imu/data", 4, &handle_imu_rotation);

	ros::spin();
	return 0;
}
