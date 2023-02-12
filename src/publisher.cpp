// including libraries
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "assignment_2/info_robot.h"
#include <sstream>
#include <iostream>
#include <unistd.h>

// initializing variables
double posx, posy, velx, velz;
ros::Publisher pub;

// callback function for the subscription to the topic /odom
void myCallback(const nav_msgs::Odometry::ConstPtr& msg) {
	
	// we take the information we need from the topic
	posx = msg->pose.pose.position.x;
	posy = msg->pose.pose.position.y;
	velx = msg->twist.twist.linear.x;
	velz = msg->twist.twist.angular.z;
	ROS_INFO("[%f,%f,%f,%f]",posx,posy,velx,velz);
	assignment_2::info_robot info;
	
	// we publish the information obtained on the custom topic /info, using custom messages
	info.x = posx;
	info.y = posy;
	info.velx = velx;
	info.velz = velz;
	pub.publish(info);
}

// main function
int main (int argc, char **argv) {
	
	// initializing the node
	ros::init(argc,argv,"publisher");
	ros::NodeHandle nh;
	
	// initializing the publisher and the subscriber
	pub = nh.advertise<assignment_2::info_robot>("/my_info",1);
	ros::Subscriber sub = nh.subscribe("odom",1,myCallback);
	
	// we set the 'spin rate'
	ros::Rate rate(1);
	
	assignment_2::info_robot info;
	rate.sleep();
	ros::spin();
}
