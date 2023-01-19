// HERE I WILL IMPLEMENT A NODE WHICH WILL NUMBER OF GOALS REACHED AND CANCELLED WHEN CALLED
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

int n_reach = 0;
int n_cancel = 0;
int state;

void myCallback(const assignment_2_2022::PlanningActionResult::ConstPtr& msg) {
	state = msg->status.status;
	ROS_INFO("%d",state);
	if(state == 2) {
		n_cancel += 1;
	}
	else if(state == 3) {
		n_reach += 1;
	}
}

bool serviceCallback(assignment_2::Goal::Request &req, assignment_2::Goal::Response &res) {
	res.reach = n_reach;
	res.canc = n_cancel;
	return true;
}

int main(int argc, char **argv) {
	ros::init(argc,argv,"service");
	ros::NodeHandle nh;
	ros::ServiceServer service = nh.advertiseService("/result",serviceCallback);
	ros::Subscriber sub = nh.subscribe("reaching_goal/result",1,myCallback);
	ros::spin();
}
