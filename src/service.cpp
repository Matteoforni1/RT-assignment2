// including libraries
#include "ros/ros.h"
#include "assignment_2/info_robot.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <assignment_2/PlanningActionGoal.h>
#include <assignment_2_2022/PlanningActionResult.h>
#include "assignment_2/Goal.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "actionlib_msgs/GoalStatus.h"

// initializing variables
int n_reach = 0;
int n_cancel = 0;
int state;

// callback function for the subscription to the topic /reaching_goal/result
void myCallback(const assignment_2_2022::PlanningActionResult::ConstPtr& msg) {
	
	// we get the current state of the action
	state = msg->status.status;
	ROS_INFO("%d",state);
	
	// if the state is 2, a goal has been cancelled, so we increase of one unit the number of cancelled goals
	if(state == 2) {
		n_cancel += 1;
	}
	// if the state is 1, a goal has been reached, so we increase of one unit the number of reached goals
	else if(state == 3) {
		n_reach += 1;
	}
}

// callback function for the custom service
bool serviceCallback(assignment_2::Goal::Request &req, assignment_2::Goal::Response &res) {
	
	// we set as response the number of goals reached and cancelled
	res.reach = n_reach;
	res.canc = n_cancel;
	return true;
}

int main(int argc, char **argv) {
	
	//  initializing the node
	ros::init(argc,argv,"service");
	ros::NodeHandle nh;
	
	// initializing the custom service and the subscriber
	ros::ServiceServer service = nh.advertiseService("/result",serviceCallback);
	ros::Subscriber sub = nh.subscribe("reaching_goal/result",1,myCallback);
	
	ros::spin();
}
