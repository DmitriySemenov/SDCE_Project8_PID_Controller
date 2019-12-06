#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif
#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <ctime>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid, pid_th;
  /**
   * TODO: Initialize the pid variable.
   */
	double Kp_val = 0.13;
	double Ki_val = 0.0008;
	double Kd_val = 2.8;
	double dz_start = -0.1;
	double dz_end = 0.1;

	double Kp_th_val = 0.13;
	double Ki_th_val = 0.0001;
	double Kd_th_val = 3;
	double dz_th_start = -0.5;
	double dz_th_end = 0.5;

	double target_speed = 60;
	int lap_num = 1;
	double lap_cte_total = 0;
	std::clock_t start_time = -1;
	std::clock_t prev_time = 0;


	pid.Init(Kp_val, Ki_val, Kd_val, dz_start, dz_end);
	pid_th.Init(Kp_th_val, Ki_th_val, Kd_th_val, dz_th_start, dz_th_end);

  h.onMessage([&pid, &pid_th, &target_speed, &lap_num, &lap_cte_total, &start_time, &prev_time](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value = 0;
					double throttle = 0.3;
					double lap1_duration = 43.2;
					double lap_duration = 42.7;
					double curr_time, duration, time_diff;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */

					if (start_time == -1)
						start_time = std::clock();

					curr_time = std::clock();
					duration = (curr_time - start_time) / (double)CLOCKS_PER_SEC;
					time_diff = (curr_time - prev_time) / (double)CLOCKS_PER_SEC;
					prev_time = curr_time;

					pid.UpdateError(cte);
					steer_value = pid.UpdateControl();

					pid_th.UpdateError(speed - target_speed);
					throttle = pid_th.UpdateControl();
					lap_cte_total += fabs(cte);

					if (lap_num == 1)
					{
						if (duration >= lap1_duration)
						{
							std::cout << " Lap: " << lap_num << " LAP CTE TOTAL: " << lap_cte_total << std::endl;
							lap_num += 1;
							start_time = std::clock();
							lap_cte_total = 0;
						}
					}
					else {
						if (duration >= lap_duration)
						{
							std::cout << " Lap: " << lap_num << " LAP CTE TOTAL: " << lap_cte_total << std::endl;
							lap_num += 1;
							start_time = std::clock();
							lap_cte_total = 0;
						}
					} 
          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
          //          << " Lap: " << lap_num << " LAP CTE TOTAL: " << lap_cte_total << std::endl;
					std::cout << "CTE: " << cte << " Time Step: " << time_diff << "\r";

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
#ifdef _WIN32
	if (h.listen("127.0.0.1", port)) {
		std::cout << "Listening to port " << port << std::endl;
	}
	else {
		std::cerr << "Failed to listen to port" << std::endl;
		return -1;
	}
#else
	if (h.listen(port)) {
		std::cout << "Listening to port " << port << std::endl;
	}
	else {
		std::cerr << "Failed to listen to port" << std::endl;
		return -1;
	}
#endif
  
  h.run();
}