// HERE I WILL IMPLEMENT THE ACTION CLIENT FOR SETTING OR CANCELING A TARGET
#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <assignment_2/PlanningAction.h>
#include <assignment_2/PlanningActionGoal.h>
#include <assignment_2_2022/PlanningActionResult.h>
#include "assignment_2/Goal.h"
#include "actionlib_msgs/GoalStatus.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

double posx, posy;
int n_reach, n_canc;

int main (int argc, char **argv) {
	
	ros::init(argc,argv,"action_client");
	ros::NodeHandle nh;
	
	ros::Rate rate(1);
	
	actionlib::SimpleActionClient<assignment_2::PlanningAction> ac("/reaching_goal",true);
	
	ROS_INFO("Waiting for action server to start...");
	ac.waitForServer();
	
	ROS_INFO("Action server started, waiting for input...");
	char decision;
	bool reached = false;
	bool finished_before_timeout;
	actionlib::SimpleClientGoalState state = ac.getState();
	assignment_2::PlanningGoal goal;
	ros::ServiceClient client = nh.serviceClient<assignment_2::Goal>("/result");
	while(ros::ok()) {

		
		std::cout<<"Welcome to my program!\n";
		std::cout<<"Enter 'g' to set a new goal\n";
		std::cout<<"Enter 'c' to cancel the current goal\n";
		std::cout<<"Enter 'p' to print the number of goal reached and cancelled\n";
		std::cout<<"Enter 'e' to exit from the program\n";
		
		std::cin>>decision;
		
		state = ac.getState();
		ROS_INFO("CURRENT STATE: %s",state.toString().c_str());
		
		if (decision == 'g') {
			// Check current goal and set new
			
			int init = state.toString().compare("LOST");
			int finish = state.toString().compare("SUCCEEDED");
			int canc = state.toString().compare("PREEMPTED");
			ROS_INFO("%d",init);
			
			if (init == 0 | finish == 0 | canc == 0) {
				std::cout<<"Set the goal:\n";
				std::cin>>posx>>posy;
		
				goal.target_pose.pose.position.x = posx;
				goal.target_pose.pose.position.y = posy;
	
				ac.sendGoal(goal);
			}
			else {
				ROS_INFO("Error! A goal is being currently pursued by the robot, if you want to set a new goal, cancel the current goal or wait for the robot to reach the current goal");
			}
		}
		
		else if (decision == 'c') {
			// Check current goal and cancel it
			
			state = ac.getState();
			
			int init = state.toString().compare("LOST");
			int finish = state.toString().compare("SUCCEEDED");
			int canc = state.toString().compare("PREEMPTED");
			
			if (init == 0 | finish == 0 | canc == 0) {
				ROS_INFO("Error! There is no current goal pursued by the robot!");
			}
			else {
				ac.cancelGoal();
				ROS_INFO("Goal successfully cancelled!");
			}
		}
		else if (decision == 'p') {
			assignment_2::Goal num;
			
			client.call(num);
			
			n_reach = num.response.reach;
			n_canc = num.response.canc;
			
			ROS_INFO("NUMBER OF REACHED GOALS: [%d], NUMBER OF CANCELLED GOALS: [%d]", n_reach, n_canc);
		}
		else if (decision == 'e') {
			std::cout<<"Bye bye!\n";
			return 0;
		} 
		else {
			std::cout<<"You inserted an invalid command, please type a valid one!\n";
		}

		ros::spinOnce();
	}
}
