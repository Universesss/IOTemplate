#include "vex.h"
#include "IO-Template/drive.h"
PID::PID(float error, float kp, float ki, float kd, float starti) : error(error),
                                                                    kp(kp),
                                                                    ki(ki),
                                                                    kd(kd),
                                                                    starti(starti) {};

PID::PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout) : error(error),
                                                                                                                        kp(kp),
                                                                                                                        ki(ki),
                                                                                                                        kd(kd),
                                                                                                                        starti(starti),
                                                                                                                        settle_error(settle_error),
                                                                                                                        settle_time(settle_time),
                                                                                                                        timeout(timeout) {};

float PID::compute(float error, float settle)
{
    if (fabs(error) < starti)
    { // StartI integral kurulmasını önlemek için kullanılır. //fabs mutlak değer alır.
        accumulated_error += error;
    }
    if ((error > 0 && previous_error < 0) || (error < 0 && previous_error > 0))
    {
        accumulated_error = 0;
    } // Bu if deyimi, hatanın 0'ı geçip geçmediğini kontrol eder ve geçtiyse integral terimini ortadan kaldırır.

    output = kp * error + ki * accumulated_error + kd * (error - previous_error);

    previous_error = error;

    if (fabs(error) < settle_error)
    {
        time_spent_settled += settle;
    }
    else
    {
        time_spent_settled = 0;
    }

    time_spent_running += settle;

    return output;
}

bool PID::is_settled()
{
    if (time_spent_running > timeout && timeout != 0)
    {
        return (true);
    } // Zaman aşımı 0'a eşitse, hareket hiçbir zaman gerçekten zaman aşımına uğramaz.
    // Zaman aşımını 0 olarak ayarlamak, onu sonsuza ayarlamakla eşdeğerdir.
    if (time_spent_settled > settle_time)
    {
        return (true);
    }
    return (false);
}