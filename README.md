# Second Assignment - Research Track 1
## Summary:

This project has been developed by Forni Matteo, a student of Robotics Engineering at the University of Genoa.
This document contains the information required for the second assignment, which include:

- How the code works
- Pseudocode
- How to run the code
- Possible improvements

## How the code works:

The code is divided into four different C++ nodes. These nodes are the following ones:

- The publisher node: this node does two things: first, it subscribes to the topic ```/odom``` from which it takes the current horizonthal and vertical positions and velocities of the robot; then it publishes these information on the topic ```/my_info```, using the custom message defined into the file ```info_robot.msg```, which can be found inside the ```msg``` folder inside the ```assignment_2``` package. 

- The subscriber node: This node subscribes to two topics: the ```/my_info``` already mentioned before, in order to take information about the robot's current position and velocity (both horizonthal and vertical) and the ```/reaching_goal/goal```, from which the node takes the current goal's horizonthal and vertical position. Inside the callback function of the first subscription (the one to the ```/my_info``` topic), this node also computes the norm of the robot's velocity, combining horizonthal and vertical velocity, by computing: ```V = sqrt((Vx)^2 + (Vy)^2)```, then it computes the new average velocity by doing: ```New_avg = ((Old_avg * n_subs) + V)/(n_subs + 1)``` and to conclude, this node publishes the new average velocity on the terminal window. Moreover, this node also computes the current distance of the robot to the target, by doing: ```d = sqrt((dx)^2 + (dy)^2)```, where dx is the difference between the robot horizhontal position and the gaol horizonthal position (dy is the same, but for the vertical positions) and publishes also this value on the terminal window.
The rate at which this node publishes the information mentioned above on the terminal window is set by parameters in the launch file.

- The action client node: This node implements an action client which will allow the user to set or cancel a goal, or call a service to print the number of goals reached and cancelled during the execution. First of all the node initializes a SimpleActionClient and waits for the action server to start; once the server has started, a menù will be printed on the terminal window. The user has the oppurtunity to enter different coomands:
If the user presses 'g', the node will control the state of the action: if no goal is currently being pursued (otherwise the user will be alerted and the menù will be reprinted on the terminal), the user can enters two integer numbers, which will be the horizonthal and vertical coordinates of the goal point. Then, the goal will be set and the server will start to make the robot follow the goal (to do so, I used the Planning.action file inside the action folder of the package assignment_2).
If the user presses 'c', the node will control the state of the action: if a goal is currently being pursued (otherwise the user will be alerted and the menù will be reprinted on the terminal), the node cancels the goal, so the action server will stop to make the robot follow the goal.
If the user presses 'p', this node will use a service, calling the Service Node and asking it the number of goals reached and cancelled (more explanations will be given in the Service Node description).
If the user presses 'e', the node will terminate its execution, displying a 'bye bye' message.
If the user presses any other button, an error alert message will be displayed on the terminal and the menù will be reprinted.

- The service node: This node does two things: subscribes to the ```/reaching_goal/result``` topic, in order to get information about the status of the goal set by the action client; if the status is equal to 2, it means that a goal has been cancelled, so the number of goals cancelled is increased by one; if the status is equal to 3, it means that a goal has been reached, so the number of goal reached is increased by one.
This node also communicates with the action client menù by using a custom service: when the user presses 'p' on the action client menù, the action client node sends a request for a custom service to the service node; the service node will reply by sending to it the number of goals reached and cancelled.

## Pseudocode:

- Action Client Node:

import libraries

FUNCTION printing()
  
  set the number of reached goal to the reach value of the response
  set the number of cancelled goal to the canc value of the response
  print these two numbers
  
END FUNCTION

FUNCTION MAIN()

  initialize the node
  initialize the action client
  
  waiting for the server
  
  get the current state of the action
  
  initialize the service
  
  WHILE(True)
    
    print the menù
    take as input the user decision
    get the current state of the action
    
    IF the user typed 'g'
    
      compare the state string with the possible states: LOST, PREEMPTED and SUCCEEDED
    
      IF the user is in one of these three states
      
        ask the user to set the goal's coordinates
        take the two coordinates as inputs
        set the goal position
        send the goal
        
      ELSE
      
        error message printed
        
      END IF
      
    ELSE IF the user typed 'c'
    
      compare the state string with the possible states: LOST, PREEMPTED and SUCCEEDED
      
      IF the user is in one of these three states
      
        error message printed
        
      ELSE
      
        cancel the goal
        
      END IF
      
    ELSE IF the user typed 'p'
    
      call for the service
      call the function printing()
      
   ELSE IF the user typed 'e'
   
    return (we terminate the execution)
    
  ELSE
  
    error message printed
    
  END IF

END FUNCTION

## How to run the code:

In order to run this code, you need first of all the ROS noetic version on your computer. To obtain it, you have two choices:
- You can try to install it on your Ubuntu (22 version), but this is usually the longer way to do it;
- Alternatively, you can install [Docker](https://docs.docker.com/get-docker/) and then follow the instruction to install ROS at this [link](http://wiki.ros.org/ROS/Installation).
After you got ROS, you can clone this repository by typing on your terminal: ```git clone https://github.com/Matteoforni1/RT-assignment2```.
Inside the cloned repository you will find two packages: the package: the ```assignment_2_2022``` package and the ```assignment_2``` package.
WARNING: Before running the launch files in these packages, be sure to have Xterm installed on your machine (you can just type the command ```apt-get install xterm```  on your terminal), because it is used in the launch file inside the ```assignment_2``` package.
Both these packages contains a launch file that should be run:
- First run on your terminal the command: ```roslaunch assignment_2_2022 assignment1.launch```, in this way you will generate the simulation environment and the robot, while two windows will spawn: one is Gazebo and the other is Rviz;
- Then run on your terminal the command: ```roslaunch assignment_2 assignment.launch```, this will spawn four terminal windows: one is the user interface (the action client node), one is the publisher node, one is the service node and the last one publishes information about the average velocities and the distance between the robot and the target (the subscriber node).
After that, you are ready to use the program!

## Possible improvements:

There is some possible improvement to implement into this code:

- On the action server (which is in the assignment_2_2022 package), it happens sometimes that the robot may stuck itself when it is close to the angles of a wall. This problem can be avoided by cancelling the goal if the robot happens to move towards an angle, but the code on the action server can be improved in order to fix the problem.
- When the user is asked to give the coordinates for setting a new goal, invalid inputs are not handled and they will bug permanently the action client (the only option available is to close and restart the program); so an invalid input handler can be added in the action client.
- When the user presses 'e' on the action client menù in order to close it, only the action client window will close: the other three terminal windows, Rviz and Gazebo need to be closed manually. To fix this problem, a custom service can be used, such that when the user close the action client menù, the other windows also close.
