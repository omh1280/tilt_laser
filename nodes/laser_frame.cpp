#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

int main(int argc, char **argv) {
	ros::init(argc, argv, "laser_frame");
	ros::NodeHandle n;
	
	tf::TransformBroadcaster br;
	tf::Transform transform;
	
	ros::Rate rate(100.0);

	while (n.ok()) {
		transform.setOrigin(tf::Vector3(0.2, 0.0, 0.0));
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "laser"));
		rate.sleep();
	}
	return 0;
}