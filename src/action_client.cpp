// including libraries
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

// initializing variables
double posx, posy;
int n_reach, n_canc;

// main function
int main (int argc, char **argv) {
	
	// initalizing the node
	ros::init(argc,argv,"action_client");
	ros::NodeHandle nh;
	
	// initializing the action client
	actionlib::SimpleActionClient<assignment_2::PlanningAction> ac("/reaching_goal",true);
	
	// waiting for the action server to start, so the action client can interact with it
	ROS_INFO("Waiting for action server to start...");
	ac.waitForServer();
	
	ROS_INFO("Action server started, waiting for input...");
	
	// other initializations...
	char decision;
	bool reached = false;
	bool finished_before_timeout;
	
	// Getting the goal state, in order to see what operations are possible (if everything goes correctly, the initial state should be: LOST)
	actionlib::SimpleClientGoalState state = ac.getState();
	
	assignment_2::PlanningGoal goal;
	ros::ServiceClient client = nh.serviceClient<assignment_2::Goal>("/result");
	
	// while the program runs:
	while(ros::ok()) {
	
		// A menù is spawned in the terminal, where instructions are given to the user about the commands that can be executed
		std::cout<<"Welcome to my program!\n";
		std::cout<<"Enter 'g' to set a new goal\n";
		std::cout<<"Enter 'c' to cancel the current goal\n";
		std::cout<<"Enter 'p' to print the number of goal reached and cancelled\n";
		std::cout<<"Enter 'e' to exit from the program\n";
		
		// The program takes the user decision
		std::cin>>decision;
		
		// the current goal state is taken
		state = ac.getState();
		ROS_INFO("CURRENT STATE: %s",state.toString().c_str());
		
		// if the user typed 'g', the program tries to set a new goal
		if (decision == 'g') {
			
			state = ac.getState();
			
			// we compare the current state with three possible states that the program can have while we are not pursueing a goal
			int init = state.toString().compare("LOST");
			int finish = state.toString().compare("SUCCEEDED");
			int canc = state.toString().compare("PREEMPTED");
			ROS_INFO("%d",init);
			
			// if we are in one of these three states, the user can set the goal
			if (init == 0 | finish == 0 | canc == 0) {
				std::cout<<"Set the goal:\n";
				std::cin>>posx>>posy;
				
				// setting the goal
				goal.target_pose.pose.position.x = posx;
				goal.target_pose.pose.position.y = posy;
	
				// sending the goal to the server
				ac.sendGoal(goal);
			}
			
			// if a goal is being pursued, the user receives a warning
			else {
				ROS_INFO("Error! A goal is being currently pursued by the robot, if you want to set a new goal, cancel the current goal or wait for the robot to reach the current goal");
			}
		}
		
		// if the user typed 'c' the program tries to cancel the current goal
		else if (decision == 'c') {

			//state = ac.getState();
			
			// we compare the current state with three possible states that the program can have while we are not pursueing a goal
			int init = state.toString().compare("LOST");
			int finish = state.toString().compare("SUCCEEDED");
			int canc = state.toString().compare("PREEMPTED");
			
			// if we are in one of these three states, there is no goal to cancel, the user receives a warning
			if (init == 0 | finish == 0 | canc == 0) {
				ROS_INFO("Error! There is no current goal pursued by the robot!");
			}
			
			// otherwise, we cancel the current goal
			else {
				ac.cancelGoal();
				ROS_INFO("Goal successfully cancelled!");
			}
		}
		
		// if the user types 'p' the program calls the service node to know the numbers of goals reached and cancelled
		else if (decision == 'p') {
			
			assignment_2::Goal num;
			
			// the program asks for the service
			client.call(num);
			
			// we take from the response the number of goals reached and cancelled
			n_reach = num.response.reach;
			n_canc = num.response.canc;
			
			// we  print the information that we have just obtained
			ROS_INFO("NUMBER OF REACHED GOALS: [%d], NUMBER OF CANCELLED GOALS: [%d]", n_reach, n_canc);
		}
		
		// if the user types 'e' the program ends its execution
		else if (decision == 'e') {
			std::cout<<"Bye bye!\n";
			return 0;
		} 
		
		// if another input has been entered by the user an error message is displayed
		else {
			std::cout<<"You inserted an invalid command, please type a valid one!\n";
		}

		ros::spinOnce();
	}
}
