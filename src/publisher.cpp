// HERE I WILL IMPLEMENT A NODE TO PUBLISH VELOCITIES (ON CUSTOM MSGS), TAKEN FROM TOPIC /odom
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "assignment_2/info_robot.h"
#include <sstream>
#include <iostream>
#include <unistd.h>

double posx, posy, velx, vely;
ros::Publisher pub;

void myCallback(const nav_msgs::Odometry::ConstPtr& msg) {
	posx = msg->pose.pose.position.x;
	posy = msg->pose.pose.position.y;
	velx = msg->twist.twist.linear.x;
	vely = msg->twist.twist.linear.y;
	ROS_INFO("[%f,%f,%f,%f]",posx,posy,velx,vely);
	assignment_2::info_robot info;
	info.x = posx;
	info.y = posy;
	info.velx = velx;
	info.vely = vely;
	pub.publish(info);
}

int main (int argc, char **argv) {
	ros::init(argc,argv,"publisher");
	ros::NodeHandle nh;
	pub = nh.advertise<assignment_2::info_robot>("/my_info",1);
	ros::Subscriber sub = nh.subscribe("odom",1,myCallback);
	ros::Rate rate(1);
	assignment_2::info_robot info;
	rate.sleep();
	ros::spin();
}
