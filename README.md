# PID Controller Project

# Introduction
Goal of this project was to develop and tune a PID controller for steering (and optionally throttle) to have a car drive around the track in a simulator without exiting the track boundaries.

# Controller overview
The final implementation of the controller is a full PID controller that includes all 3 of the terms.
The effect of each of the terms and turning of the parameters is going to be described below.

If the controller is completely disabled and all the controller gains are set to 0, vehicle just keeps going in a straight line and fails to drive around the track:

Video with [all gains set to 0](Videos/01_P_Term_0p0.mp4)

The P-term of the controller is the main way that the car's steering is adjusted to have it go around the track. The CTE (cross track error) is fed back from the simulator, which is multiplied by the P-gain to give the P-term of the controller. A good idea is to start tuning the P-term by applying a small P-gain first and seeing the effect.

Here's what happens when [P-gain is set to 0.05](Videos/02_P_Term_0p05.mp4). It can be seen that the vehicle does try to follow the track and reacts to CTE but eventually becomes unstable and goes off track.

Increasing the value of P-gain to [0.13](Videos/03_P_Term_0p13.mp4) only makes the matters worse and vehicle becomes unstable sooner.
It's clear that just a P controller is not enough to solve the problem. 

Next, a D-term is applied to try to remove instability and reduce oscillations of vehicle around the track. D-term acts on the rate of change of the error.

Initial value of D-gain of 1.0 is selected, while keeping P-gain at 0.13.
As can be seen from [this video](Vides/04_P_Term_0p13_D_Term_1p0.mp4) D-term has a positive effect, which minimizes oscillations, however it is not enough to fully solve the problem.

Since it was found that D-term helps reduce oscillations, it is tuned up. Increase in the D-gain has a positive effect and helps the vehicle go around the track, but setting it too high eventually causes the vehicle to go off track.
For example, [this video](Videos/05_P_Term_0p13_D_Term_6p8.mp4) demonstrates the effect of having a very high D-gain of 6.8.

Eventually a good compromise value of D-gain is found to be 2.8. As can be seen from [this video](Videos/06_P_Term_0p13_D_Term_2p8.mp4) the vehicle is now able to handle the long sweeping corners better and is able to make it all the way around the track.

As part of the code, a tracker of CTE over the whole lap was implemented. It works by adding the absolute value of the CTE and having an estimate of how much time it takes to go around the track at a given speed. Since the throttle controller was also implemented and it does a good job of tracking target speed of 60 MPH, most laps around the track take around 42.7 seconds (after the very first one that takes a bit longer). 

Using a CTE total tracker it's possible to see the effect of control paramter changes. After each new update to parameters, vehicle would go around the track twice and both CTE total values were recorded.

At this point, with just the PD controller, CTE values were found to be 1090 and 1042.

Next, and I-term is added to the controller. Since I-term acts on the sum of the error, it should help minimize the CTE during sweeping corners where the error will gradually accumulate over time. 

Initially I-gain is set to 0.1, but it can be seen from [this video](Videos/07_P_Term_0p13_D_Term_2p8_I_Term_0p1.mp4) that it's too high and I-term overpowers the other two terms and causes vehicle to spin in circles. Even reducing it by a factor of 10 to 0.01 is not enough as can be seen [here](Videos/08_P_Term_0p13_D_Term_2p8_I_Term_0p01.mp4). Reducing it again by a factor of 10 to 0.001 finally has the desired effect and vehicle makes it around the track. CTE for this PID controller is found to be 874, and 837, which is a better result than the previous PD-only controller with the same P and D gains.
Further tuning showed that the best I-gain is 0.0008, which results in 857, and 814 CTE values for 2 laps.

One last improvement that was made is an introduction of deadzone to the I-term. I-term introduces some oscillations when vehicle is driving near a straight lane (even though it helps during cornering), so to reduce the effect that a small amount of error causes the deadzone of -0.1 to 0.1 was set. This slightly improved the CTE to 839, 818.

Final set of parameters that worked well with my setup was: P = 0.13, I = 0.0008, D = 2.8.
Here's a [video of the vehicle driving full lap with best parameters](Videos/10_P_Term_0p13_D_Term_2p8_I_Term_0p0008_DZ.mp4).

