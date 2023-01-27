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

- The action client node: This node implements an action client which will allow the user to set or cancel a goal, or call a service to print the number of goals reached and cancelled during the execution. First of all the node initializes a SimpleActionClient and waits for the action server to start; once the server has started, a menù will be printed on the terminal window. The user has the oppurtunity to enter different coomands:
If the user presses 'g', the node will control the state of the action: if no goal is currently being pursued (otherwise the user will be alerted and the menù will be reprinted on the terminal), the user can enters two integer numbers, which will be the horizonthal and vertical coordinates of the goal point. Then, the goal will be set and the server will start to make the robot follow the goal (to do so, I used the Planning.action file inside the action folder of the package assignment_2).
If the user presses 'c', the node will control the state of the action: if a goal is currently being pursued (otherwise the user will be alerted and the menù will be reprinted on the terminal), the node cancels the goal, so the action server will stop to make the robot follow the goal.
If the user presses 'p', this node will use a service, calling the Service Node and asking it the number of goals reached and cancelled (more explanations will be given in the Service Node description).
If the user presses 'e', the node will terminate its execution, displying a 'bye bye' message.
If the user presses any other button, an error alert message will be displayed on the terminal and the menù will be reprinted.

- The service node

## Pseudocode:



## How to run the code:



## Possible improvements:

There is some possible improvement to implement into this code:
