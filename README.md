## PID Controller Project

In this Udacity project we have to implement and tune a PID controller for the autonomous car in the Udacity simulator.

This project contains the following files:
- src folder and CMakeLists.txt to for running the code
- install scripts for the uWebSockets to interface the simulator
- the Udacity simulator is not part of this repository but can be downloaded here: https://github.com/udacity/self-driving-car-sim/releases


## Contents
#### [1. PID Basics](#pid-basics)
#### [2. Manual Tuning](#manual-tuning)
#### [3. Auto Tuning](#auto-tuning)



### PID Basics

The PID controller is the most used controller in the field and last time I checked, 95% of all controllers were PID. The reason lies in the simplicity of its algorithm and the wide array of use cases. As the name "PID" implies the controller consists of three parts, the proportional (P), the integral (I) and the derivative (D) part. Let's take a look at these.

##### P controller
The P controller takes in a error and returns a control signal with proportional to the error. This is the most basic part and it means that we steer harder if we are farther from the middle of the road.

##### I controller
The I controller accumulates the error and returns a control signal proportional to the integral. This part is important to eliminate steady-state errors and is usually used if we have a delay in the system, most common in the form of a constant systematic error.

##### D controller
The D controller returns a control signal proportional to the rate of error. In our case it can be seen as a damping (this video explains it very well: https://www.youtube.com/watch?v=4Y7zG48uHRo). The higher the D-gain the harder the damping is. Low damping might allow our system to oscillate while high damping will slow down the reaction time.

##### Our control system
Depending on our system we might not need all gains from the PID controller. The field is studied very well, especially for linear systems. If we examine the motion model of our car, we can see that we have a nonlinear system. It has something similar to integral behavior or integral behavior with a delay.  
If we look into the literature, we can see that a PD controller should be sufficient for the job. To be specific, a PID controller is ideal for a steady-state IT1 system (straight road) but a PD controller is better for a unsteady IT1 system (curved road).

| Plant <td colspan="5"> __Controller__ |
| ------------- |:-------------:| -----:|
|   | P | I | PI | PD | PID |
| P | - | + | +  | -  | over-engineered |
| I | + (u) | unstable | + | + (s) | over-engineered |
| PT1 | + | - | + (u&s) | - | over-engineered |
| PTn | - | - | + | - | + (u&s) |
| ITn | - | unstable | - | + (u) | + (s)|
| Tt  | - | - | + (u&s) | - | - |
__u__: unsteady system  
__s__: steady system  

##### Tuning strategy:
Our lecturer Sebastian Thrun suggested an automated process called 'twiggle' for this assignment. For this to work, we need a set of parameters which at least keep the car on track or we can't calculate the error. Therefore, we manually tune the parameters first and use twiggle to fine tune our system.

### Manual Tuning
First, I tried to tune the controller with a empirical method found on wikipedia (https://en.wikipedia.org/wiki/PID_controller). It suggests to start from zero for all parameters and increase the P-gain until oscillation. The cut the P-gain in half and increase the I-gain until the offset is corrected in sufficient time and at last use the D-gain the get rid of any oscillations. This might work for PT1 systems but for our car this method did not work and made our car unstable, in the sense that the oscillation amplitude increases indefinitely. The problem lies in the integral behavior in of our system. We want the P and D-gain to carry most of the weight and not the I-gain, especially for curves. The I-gain is either to slow, or so strong that we get too much oscillation.  

PI contoller:  
![PI controller](./gifs/PI_controller.gif)


The correct way to tune our parameters is to increase our P-gain until oscillation and reduce it to less than a half and then use the D-gain as a damping. The sweet spot is critical damping (also see video), the point where oscillation disappears. The I controller is not mandatory, so I kept the I-gain low.  
We can print out the proportion of each controller to check which controller is most active and we try to keep the I proportion low.

PD contoller:  
![PD controller](./gifs/PD_controller.gif)

After trial and error, I have found these parameters to be sufficient:  
P: 0.1  
I: 0.01  
D: 0.001


### Auto Tuning
When we succeeded to keep the car on track we can then proceed with the auto tuning. We basically change one parameter at a time and observe the cross-track error over one lap. If it improves the error we increase this parameter further, otherwise we want to lower the increase.  
With this algorithm we improve our parameters to the following:  
P: 0.12  
I: 0.01  
D: 0.0012  
Unfortunately, this strategy does not allow for big parameter changes, since big changes might cause our car to get off track. Therefore we cannot exceed the local minimum of our parameter set.
