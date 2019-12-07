# PID Controller Project

# Introduction
Goal of this project was to develop and tune a PID controller for steering (and optionally throttle) to have a car drive around the track in a simulator without exiting the track boundaries.

# Controller overview
The final implementation of the controller is a full PID controller that includes all 3 of the terms.
The effect of each of the terms and turning of the parameters is going to be described below.

If the controller is completely disabled and all the controller gains are set to 0, vehicle just keeps going in a straight line and fails to drive around the track.

[a relative link](Videos/01_P_Term_0p0.mp4)

The P-term of the controller is the main way that the car's steering is adjusted to have it go around the track. The CTE (cross track error) is fed back from the simulator, which is multiplied by the P-gain to give the P-term of the controller. A good idea is to start tuning the P-term by applying a small P-gain first and seeing the effect.

Here's what happens when [P-gain is set to 0.05](Videos/02_P_Term_0p05.mp4). It can be seen that the vehicle does try to follow the track and reacts to CTE but eventually becomes unstable and goes off track.
