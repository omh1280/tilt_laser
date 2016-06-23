#include <ros/ros.h>
#include <laser_assembler/AssembleScans2.h>
#include <sensor_msgs/PointCloud2.h>

using namespace laser_assembler;
int main(int argc, char **argv) {
	ros::init(argc, argv, "laser_snapshot");
	ros::NodeHandle n;

	// Wait for laser assembler
	ros::service::waitForService("assemble_scans2");
	
	// Set update frequency (in HZ) to publish point cloud data
	ros::Rate r(1.5);

	// Create publisher
	ros::Publisher pub = n.advertise<sensor_msgs::PointCloud2>("cloud", 2);

	while (ros::ok()) {
		// Server
		AssembleScans2 srv;
		srv.request.begin = ros::Time(0,0);
		srv.request.end = ros::Time::now();
		// Client
		ros::ServiceClient client = n.serviceClient<AssembleScans2>("assemble_scans2");

		// Publish point cloud to topic
		if (client.call(srv)) {
			printf("Got cloud with %u points\n", srv.response.cloud.height*srv.response.cloud.width);
			pub.publish(srv.response.cloud);
		} else {
			printf("Service call failed\n");
		}
		r.sleep();
	}
	return 0;
}