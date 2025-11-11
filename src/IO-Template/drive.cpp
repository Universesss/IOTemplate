#include "vex.h"

Drive::Drive(enum ::drive_setup drive_setup, motor_group DriveL, motor_group DriveR, int gyro_port, float wheel_diameter, float wheel_ratio, float gyro_scale) : wheel_diameter(wheel_diameter),
                                                                                                                                                                    wheel_ratio(wheel_ratio),
                                                                                                                                                                    gyro_scale(gyro_scale),
                                                                                                                                                                    drive_in_to_deg_ratio(wheel_ratio / 360.0 * M_PI * wheel_diameter),
                                                                                                                                                                    drive_setup(drive_setup),
                                                                                                                                                                    DriveL(DriveL),
                                                                                                                                                                    DriveR(DriveR),
                                                                                                                                                                 ////////
                                                                                                                                                                    Gyro(inertial(gyro_port))
{
}

// eğer sadece sürmek istiyorsanız bu fonksiyon geçerlidir
// Bu fonksiyon çoğu fonksiyonun içinde de kullanılır.

void Drive::drive_with_voltage(float leftVoltage, float rightVoltage)
{
    DriveL.spin(fwd, leftVoltage, volt);
    DriveR.spin(fwd, rightVoltage, volt);
}

// All PID constants are passed as kP, kI, kD, and startI. The kP, kI, and kD are pretty standard,
// but startI keeps the integral value at 0 until the absolute value of the error is below startI.
// This prevents integral windup on bigger turns.

void Drive::set_turn_constants(float turn_max_voltage, float turn_kp, float turn_ki, float turn_kd, float turn_starti)
{
    this->turn_max_voltage = turn_max_voltage;
    this->turn_kp = turn_kp;
    this->turn_ki = turn_ki;
    this->turn_kd = turn_kd;
    this->turn_starti = turn_starti;
}

void Drive::set_drive_constants(float drive_max_voltage, float drive_kp, float drive_ki, float drive_kd, float drive_starti)
{
    this->drive_max_voltage = drive_max_voltage;
    this->drive_kp = drive_kp;
    this->drive_ki = drive_ki;
    this->drive_kd = drive_kd;
    this->drive_starti = drive_starti;
}

void Drive::set_heading_constants(float heading_max_voltage, float heading_kp, float heading_ki, float heading_kd, float heading_starti)
{
    this->heading_max_voltage = heading_max_voltage;
    this->heading_kp = heading_kp;
    this->heading_ki = heading_ki;
    this->heading_kd = heading_kd;
    this->heading_starti = heading_starti;
}

void Drive::set_swing_constants(float swing_max_voltage, float swing_kp, float swing_ki, float swing_kd, float swing_starti)
{
    this->swing_max_voltage = swing_max_voltage;
    this->swing_kp = swing_kp;
    this->swing_ki = swing_ki;
    this->swing_kd = swing_kd;
    this->swing_starti = swing_starti;
}

// Yerleşme hatası ve yerleşme süresi, istenen konuma ulaşılıp ulaşılmadığını kontrol etmek için birlikte çalışır, ancak
// zaman aşımı ayrıdır. Robot, yerleşme süresi boyunca yerleşme hatası içinde kalmalıdır
// yerleşmek için. Hareketin süresi yerleşmeden zaman aşımına ulaşırsa, robot
// pes eder ve bir sonraki harekete geçer.

void Drive::set_turn_exit_conditions(float turn_settle_error, float turn_settle_time, float turn_timeout)
{
    this->turn_settle_error = turn_settle_error;
    this->turn_settle_time = turn_settle_time;
    this->turn_timeout = turn_timeout;
}

void Drive::set_drive_exit_conditions(float drive_settle_error, float drive_settle_time, float drive_timeout)
{
    this->drive_settle_error = drive_settle_error;
    this->drive_settle_time = drive_settle_time;
    this->drive_timeout = drive_timeout;
}

void Drive::set_swing_exit_conditions(float swing_settle_error, float swing_settle_time, float swing_timeout)
{
    this->swing_settle_error = swing_settle_error;
    this->swing_settle_time = swing_settle_time;
    this->swing_timeout = swing_timeout;
}

// bu değer eğer gyro scale'iniz 360 değilse çok önemlidir
// gyrodan aldığı değeri 360 a göre oranlar ve
// siz 90 derece dön derseniz aradaki hatayı hesaplayarak kapatır.



float Drive::get_absolute_heading()
{
    // float AverageGyro = (reduce_0_to_360(Gyro.rotation()*360.0/gyro_scale)+reduce_0_to_360(Intertial.rotation()*360.0/gyro_scale))/2.0;
    return (reduce_0_to_360(Gyro.rotation()*360.0/gyro_scale));
}

float Drive::get_left_position_in()
{
    return (DriveL.position(deg) * drive_in_to_deg_ratio);
}

float Drive::get_right_position_in()
{
    return (DriveR.position(deg) * drive_in_to_deg_ratio);
}

void Drive::turn_to_angle(float angle)
{
    turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage)
{
    turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

void Drive::turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout)
{
    turn_to_angle(angle, turn_max_voltage, turn_settle_error, turn_settle_time, turn_timeout, turn_kp, turn_ki, turn_kd, turn_starti);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Drive::turn_to_angle(float angle, float turn_max_voltage, float turn_settle_error, float turn_settle_time, float turn_timeout, float turn_kp, float turn_ki, float turn_kd, float turn_starti)
{

    desired_heading = angle;
    // İstenen yön, bir hareketten diğerine açıyı taşır. Bu şekilde, eğer robot
    // bir dönüş hareketini tamamladığında, dönüş hareketinde belirtilen açıda sürmeye devam edecektir.
    PID turnPID(reduce_negative_180_to_180(angle - get_absolute_heading()), turn_kp, turn_ki, turn_kd, turn_starti, turn_settle_error, turn_settle_time, turn_timeout);
    while (turnPID.is_settled() == false)
    {
        float error = reduce_negative_180_to_180(angle - get_absolute_heading());
        // Açının -180 ile 180 derece arasında bir değere düşürülmesi, robotun dönüş yaparken
        // her zaman daha kısa yolu izlemesini sağlar.
        float output = turnPID.compute(error, 10);
        output = clamp(output, -turn_max_voltage, turn_max_voltage);
        drive_with_voltage(output, -output);
        task::sleep(10);
    }
    DriveL.stop(hold);
    DriveR.stop(hold);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void Drive::drive_distance(float distance)
{
    drive_distance(distance, desired_heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading)
{
    drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage)
{
    drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout)
{
    drive_distance(distance, heading, drive_max_voltage, heading_max_voltage, drive_settle_error, drive_settle_time, drive_timeout, drive_kp, drive_ki, drive_kd, drive_starti, heading_kp, heading_ki, heading_kd, heading_starti);
}

/////////////////////////////////////////////////////////////////////
void Drive::drive_distance(float distance, float heading, float drive_max_voltage, float heading_max_voltage, float drive_settle_error, float drive_settle_time, float drive_timeout, float drive_kp, float drive_ki, float drive_kd, float drive_starti, float heading_kp, float heading_ki, float heading_kd, float heading_starti)
{

    desired_heading = heading;
    PID drivePID(distance, drive_kp, drive_ki, drive_kd, drive_starti, drive_settle_error, drive_settle_time, drive_timeout);
    PID headingPID(reduce_negative_180_to_180(heading - get_absolute_heading()), heading_kp, heading_ki, heading_kd, heading_starti);
    float start_average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
    // Sürücü konumunu sıfırlamak yerine, bu fonksiyon sadece
    // sürücü konumunun ne zaman başladığını not eder ve bu değere göre hatayı belirler.
    float average_position = start_average_position;
    while (drivePID.is_settled() == false)
    {
        average_position = (get_left_position_in() + get_right_position_in()) / 2.0;
        float drive_error = distance + start_average_position - average_position;
        float heading_error = reduce_negative_180_to_180(heading - get_absolute_heading());
        // Tıpkı dönüşlerde olduğu gibi, -180 dereceden 180 dereceye
        // düşürmek robotun istenen yöne doğru en hızlı yolu almasını sağlar.
        float drive_output = drivePID.compute(drive_error, 30);
        float heading_output = headingPID.compute(heading_error, 30);

        drive_output = clamp(drive_output, -drive_max_voltage, drive_max_voltage);
        heading_output = clamp(heading_output, -heading_max_voltage, heading_max_voltage);

        drive_with_voltage(drive_output + heading_output, drive_output - heading_output);
        task::sleep(10);
    }
    DriveL.stop(hold);
    DriveR.stop(hold);
}

////////////////////////////////////

////////////////////////////////////

void Drive::left_swing_to_angle(float angle)
{
    left_swing_to_angle(angle, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

// Değişebilir
/////////////////////////////////////////////////////////////////
void Drive::left_swing_to_angle(float angle, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti)
{
    desired_heading = angle;
    PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
    while (swingPID.is_settled() == false)
    {
        float error = reduce_negative_180_to_180(angle - get_absolute_heading());
        float output = swingPID.compute(error, 10);
        output = clamp(output, -swing_max_voltage, swing_max_voltage);
        DriveL.spin(fwd, output, volt);
        // Only the left side of the drive turns, making this a "left swing".
        DriveR.stop(hold);
        task::sleep(10);
    }
    DriveL.stop(coast);
    DriveR.stop(coast);
}
//////////////////////////////////////////////////////////////////////////////

void Drive::right_swing_to_angle(float angle)
{
    right_swing_to_angle(angle, swing_max_voltage, swing_settle_error, swing_settle_time, swing_timeout, swing_kp, swing_ki, swing_kd, swing_starti);
}

void Drive::right_swing_to_angle(float angle, float swing_max_voltage, float swing_settle_error, float swing_settle_time, float swing_timeout, float swing_kp, float swing_ki, float swing_kd, float swing_starti)
{
    desired_heading = angle;
    PID swingPID(reduce_negative_180_to_180(angle - get_absolute_heading()), swing_kp, swing_ki, swing_kd, swing_starti, swing_settle_error, swing_settle_time, swing_timeout);
    while (swingPID.is_settled() == false)
    {
        float error = reduce_negative_180_to_180(angle - get_absolute_heading());
        float output = swingPID.compute(error, 10);
        output = clamp(output, -swing_max_voltage, swing_max_voltage);
        DriveR.spin(reverse, output, volt);
        // Only the right side of the drive turns, so this is a "right swing".
        DriveL.stop(hold);
        task::sleep(10);
    }
    DriveL.stop(hold);
    DriveR.stop(hold);
}

// Default Swing
void Drive::Sverve(float leftSpeed, float rightSpeed, float duration)
{
    Sverve(leftSpeed, rightSpeed, true, true, duration);
}

void Drive::Sverve(float leftSpeed, float rightSpeed, bool leftDirection, bool rightDirection, float duration)
{ // sağ sol hız, sağ sol yön, süre
    if (leftDirection == false)
        leftSpeed *= -1;
    if (rightDirection == false)
        rightSpeed *= -1;

    DriveL.setVelocity(leftSpeed, percent);
    DriveR.setVelocity(rightSpeed, percent);

    DriveL.spin(forward);
    DriveR.spin(forward);

    wait(duration, sec);

    DriveL.stop(hold);
    DriveR.stop(hold);
}

// Pivot Swing
/*void Drive::Sverve(double radius, double angle, double speed){
    double wheelBase = 400;
    double angularSpeed = speed * (angle * M_PI / 180);

    double leftSpeed = angularSpeed*(radius+wheelBase/2);
    double rightSpeed = angularSpeed*(radius-wheelBase/2);

    DriveL.spin(forward, leftSpeed, rpm);
    DriveR.spin(forward, rightSpeed, rpm);

  double turnTime = (angle / 360.0) * ( 2 * M_PI * radius / leftSpeed);

    wait(turnTime, sec);

    DriveL.stop();
    DriveR.stop();
}
*/

// The usercontrol functions use deadband=5 everywhere. This value pretty much gets the job done,
// but it can be changed with no repercussions.

/*void Drive::control_arcade(){
    float throttle = deadband(controller(primary).Axis3.value(), 5);
    float turn = deadband(controller(primary).Axis1.value(), 5);

  // Sol ve sağ motor pozisyonları arasındaki farkı düzeltmek için PID
  static PID arcadePID(0, turn_kp, turn_ki, turn_kd, 0, 0, 0, 0); // Bu sabitleri ayarlamamız gerekebilir
    float left_position = get_left_position_in();
    float right_position = get_right_position_in();
    float position_error = left_position - right_position;

    float turn_adjustment = arcadePID.compute(position_error, 10);

  // Gaz ve PID ile ayarlanmış dönüşü uygulama
    DriveL.spin(fwd, to_volt(throttle + turn + turn_adjustment), volt);
    DriveR.spin(fwd, to_volt(throttle - turn - turn_adjustment), volt);
}*/

///////////////////////////////////////////////////////////////
void Drive::control_arcade()
{
    float throttle = deadband(controller(primary).Axis3.value(), 5);
    float turn = deadband(controller(primary).Axis1.value(), 5);
    DriveL.spin(fwd, to_volt(throttle + turn), volt);
    DriveR.spin(fwd, to_volt(throttle - turn), volt);
}

void Drive::control_tank()
{
    float leftthrottle = deadband(controller(primary).Axis3.value(), 5);
    float rightthrottle = deadband(controller(primary).Axis2.value(), 5);
    DriveL.spin(fwd, to_volt(leftthrottle), volt);
    DriveR.spin(fwd, to_volt(rightthrottle), volt);
}