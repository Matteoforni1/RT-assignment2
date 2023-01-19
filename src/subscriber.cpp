// HERE I WILL IMPLEMENT A NODE WHICH WILL SUBSCRIBE TO CUSTOM MSGS AND PRINT AVG VELOCITY AND DISTANCE TO THE TARGET
// USE PARAM TO SET PRINTING FREQUENCY
#include "ros/ros.h"
#include "assignment_2/info_robot.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <assignment_2/PlanningActionGoal.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

double px_r, py_r, px_g, py_g, vx, vy, gdx, gdy;
double avg_speed_x = 0;
double avg_speed_y = 0;
int n_sub = 0;

void myCallback(const assignment_2::info_robot::ConstPtr& msg) {
	px_r = msg->x;
	py_r = msg->y;
	vx = msg->velx;
	vy = msg->vely;
	//ROS_INFO("VELS: [%f,%f]",vx,vy);
	avg_speed_x *= n_sub;
	avg_speed_y *= n_sub;
	n_sub++;
	avg_speed_x += vx;
	avg_speed_y += vy;
	avg_speed_x /= n_sub;
	avg_speed_y /= n_sub;
	//ROS_INFO("AVERAGE VELOCITIES: [%f,%f]",avg_speed_x,avg_speed_y);
}

void myCallback2(const assignment_2::PlanningActionGoal::ConstPtr& msg) {
	px_g = msg->goal.target_pose.pose.position.x;
	py_g = msg->goal.target_pose.pose.position.y;
}

int main (int argc, char **argv) {
	ros::init(argc,argv,"subscriber");
	ros::NodeHandle nh;
	while (ros::ok()) {
		ros::Subscriber sub1 = nh.subscribe("my_info",1,myCallback);
		ros::Subscriber sub2 = nh.subscribe("reaching_goal/goal",1,myCallback2);
		gdx = abs(px_r-py_g);
		gdy = abs(py_r-py_g);
		ROS_INFO("DISTANCE FROM TARGET: [%f,%f]",gdx,gdy);
		ros::Rate rate(1);
		rate.sleep();
		ros::spinOnce();
	}
}
