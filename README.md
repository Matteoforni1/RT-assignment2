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

- The subscriber node: This node subscribes to two topics: the ```/my_info``` already mentioned before, in order to take information about the robot's current position and velocity (both horizonthal and vertical) and the ```/reaching_goal/goal```, from which the node takes the current goal's horizonthal and vertical position. Inside the callback function of the first subscription (the one to the ```/my_info``` topic), this node also computes the norm of the robot's velocity, combining horizonthal and vertical velocity, by computing: ```V = sqrt((Vx)^2 + (Vy)^2)```, then it computes the new average velocity by doing: ```New_avg = ((Old_avg * n_subs) + V)/(n_subs + 1)``` and to conclude it publishes it on the terminal window. Moreover, this node also computes the current distance of the robot to the target, by doing: ```d = sqrt((dx)^2 + (dy)^2)```, where dx is the difference between the robot horizhontal position and the gaol horizonthal position (dy is the same, but for the vertical positions).

- The action client node

- The service node

## Pseudocode:



## How to run the code:



## Possible improvements:

There is some possible improvement to implement into this code:
