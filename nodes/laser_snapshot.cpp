#include <ros/ros.h>
#include <laser_assembler/AssembleScans.h>
#include <sensor_msgs/PointCloud.h>

using namespace laser_assembler;
int main(int argc, char **argv) {
	ros::init(argc, argv, "laser_snapshot");
	ros::NodeHandle n;

	// Wait for laser assembler
	ros::service::waitForService("assemble_scans");
	// Set update frequency (in HZ) to publish point cloud data
	ros::Rate r(.5);

	// Create publisher
	ros::Publisher pub = n.advertise<sensor_msgs::PointCloud>("cloud", 5);


	while (ros::ok()) {
		// Server
		AssembleScans srv;
		srv.request.begin = ros::Time(0,0);
		srv.request.end = ros::Time::now();
		// Client
		ros::ServiceClient client = n.serviceClient<AssembleScans>("assemble_scans");

		// Publish point cloud to topic
		if (client.call(srv)) {
			printf("Got cloud with %lu points\n", srv.response.cloud.points.size());
			pub.publish(srv.response.cloud);
		} else {
			printf("Service call failed\n");
		}
		r.sleep();
	}
	return 0;
}