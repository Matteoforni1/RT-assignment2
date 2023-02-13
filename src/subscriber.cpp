// incuding libraries
#include "ros/ros.h"
#include "assignment_2/info_robot.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <assignment_2/PlanningActionGoal.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

// initializing variables
double px_r, py_r, px_g, py_g, vx, vz, gdx, gdy;
double avg_speed_x = 0;
double avg_speed_z = 0;
int n_sub = 0;
double publish_rate;

// callback function for the subscriber to the custom topic
void myCallback(const assignment_2::info_robot::ConstPtr& msg) {
	
	// we take the information from the custom message
	px_r = msg->x;
	py_r = msg->y;
	vx = msg->velx;
	vz = msg->velz;
	
	// we compute the average speeds
	avg_speed_x *= n_sub;
	avg_speed_z *= n_sub;
	n_sub++;
	avg_speed_x += vx;
	avg_speed_z += vz;
	avg_speed_x /= n_sub;
	avg_speed_z /= n_sub;
	
	// the average velocities are printed
	ROS_INFO("AVERAGE VELOCITIES: [%f,%f]",avg_speed_x,avg_speed_z);
}

// callback function for the subscriber to the topic /reaching_goal/goal
void myCallback2(const assignment_2::PlanningActionGoal::ConstPtr& msg) {
	
	// we get information about the robot's current position
	px_g = msg->goal.target_pose.pose.position.x;
	py_g = msg->goal.target_pose.pose.position.y;
}

void distance() {
	// we compute the distances between the robot and the target both on the x axis and the y axis
	gdx = abs(px_r-px_g);
	gdy = abs(py_r-py_g);
		
	// the two distances are printed
	ROS_INFO("DISTANCE FROM TARGET: [%f,%f]",gdx,gdy);
}

int main (int argc, char **argv) {
	
	//  initializing the node
	ros::init(argc,argv,"subscriber");
	ros::NodeHandle nh;
	// get frequency from the launch file and setting the rate to that parameter
	ros::param::get("publish_rate", publish_rate);
	ros::Rate rate(publish_rate);
	
	// initializing the two subscribers
	ros::Subscriber sub1 = nh.subscribe("my_info",1,myCallback);
	ros::Subscriber sub2 = nh.subscribe("reaching_goal/goal",1,myCallback2);
	
	// while the program is running...
	while (ros::ok()) {
		
		// function for computing the distance
		distance();
		rate.sleep();
		ros::spinOnce();
	}
}
