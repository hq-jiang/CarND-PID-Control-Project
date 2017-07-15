#include "PID.h"
#include <iostream>


using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {
    ticks = 0;
    old_t = clock();
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    PID::Kp = Kp;
    PID::Ki = Ki;
    PID::Kd = Kd;
}

void PID::UpdateError(double cte) {
    p_error = cte;
    i_error += cte;
    d_error = cte - d_error;
}

double PID::TotalError() {


    double delta_t = (float)(clock()-old_t)/CLOCKS_PER_SEC;
    double steering_angle = - p_error*Kp - i_error*Ki*delta_t - d_error*Kd/delta_t;
    ticks += 1;
    std::cout << "Ticks: " << ticks << std::endl;
    std::cout << "delta_t: " << delta_t << std::endl;
    old_t = clock();

    return steering_angle;
}

