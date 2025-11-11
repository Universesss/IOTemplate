#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
motor leftMotorA = motor(PORT2, ratio6_1, true); // ön
motor leftMotorB = motor(PORT4, ratio6_1, false); // üst
motor leftMotorC = motor(PORT6, ratio6_1, true); // arka
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT10, ratio6_1, false); // ön
motor rightMotorB = motor(PORT3, ratio6_1, true);  // üst
motor rightMotorC = motor(PORT9, ratio6_1, false);  // arka
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
inertial Intertial = inertial(PORT8);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, Intertial, 319.19, 320, 40, mm, 1);
controller Controller1 = controller(primary);


digital_out gatePiston = digital_out(Brain.ThreeWirePort.H);
digital_out midTop     = digital_out(Brain.ThreeWirePort.G);
digital_out matchLoader= digital_out(Brain.ThreeWirePort.F);
digital_out sepet      = digital_out(Brain.ThreeWirePort.E);

motor intake1 = motor(PORT19, ratio6_1, true);
motor intake2 = motor(PORT18, ratio6_1, false);

motor_group intake = motor_group(intake1, intake2);



aivision::colordesc RED_DESC (1, 200, 20, 30, 10.0, 0.20);
// aivision::colordesc BLUE_DESC (2, 40, 180, 160, 10.0, 0.20);
aivision::colordesc BLUE_DESC (2, 200, 260, 230, 10.0, 0.15);

aivision aiVision = aivision(PORT11, RED_DESC, BLUE_DESC);

optical Optical = optical(PORT1);





TeamColor teamColor = UNKNOWN;






// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1()
{
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while (true)
  {
    if (RemoteControlCodeEnabled)
    {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();

      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5)
      {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1)
        {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      }
      else
      {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5)
      {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1)
        {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      }
      else
      {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }

      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1)
      {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1)
      {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}
void vexcodeInit(void)
{
  // nothing to initialize
}