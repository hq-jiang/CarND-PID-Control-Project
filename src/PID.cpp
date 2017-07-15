#include "PID.h"
#include <iostream>
#include <math.h>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {
    ticks = 0;
    old_t = clock();
    p_error=0.;
    i_error=0.;
    d_old=0.;
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    PID::Kp = Kp;
    PID::Ki = Ki;
    PID::Kd = Kd;
}

void PID::UpdateError(double cte) {
    double delta_t = (float)(clock()-old_t)/CLOCKS_PER_SEC;
    p_error = cte;
    i_error += cte*delta_t;
    d_error = (cte - d_old)/delta_t;
    d_old = cte;

}

double PID::TotalError() {
    double control_p = p_error*Kp;
    double control_i = i_error*Ki;
    double control_d = d_error*Kd;
    double steering_angle = - control_p - control_i - control_d;
    double control_sum = fabs(control_p)+fabs(control_i)+fabs(control_d);
    ticks += 1;
    std::cout << "Ticks: " << ticks << std::endl;

    std::cout << "control_sum: " << control_sum << std::endl;
    printf("P: %.2f, I: %.2f, D: %.2f \n", fabs(control_p/control_sum), fabs(control_i/control_sum), fabs(control_d/control_sum));
    old_t = clock();

    return steering_angle;
}

