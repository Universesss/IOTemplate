#include "vex.h"


using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                             IO-Template Config                            */
/*  Robotumuzda yapılan herhangi bir değişiklikte veya benzeri config        */
/*  Yapılması gereken şeylerde genelde değişiklikler bu alanda olur          */
/*---------------------------------------------------------------------------*/

Drive chassis(

    // DriveTrain türleri
    // örneğin;
    // ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION

    ZERO_TRACKER_NO_ODOM,

   // Left Motors:
    motor_group(leftMotorA, leftMotorB, leftMotorC),

    // Right Motors:
    motor_group(rightMotorA, rightMotorB, rightMotorC),

    // Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
    PORT8,//PORT8,

    // Input your wheel diameter. (4" omnis are actually closer to 4.125"):
    4.125,

    // External ratio, must be in decimal, in the format of input teeth/output teeth.
    // If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
    // If the motor drives the wheel directly, this value is 1:
    0.5,

    // Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
    // For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
    358
);
float deger = 0;

int current_auton_selection = 0;
bool auto_started = false;

void pre_auton(void)
{
  //! ROBOT CONFIGURATION
  vexcodeInit();
  default_constants();
  //! DO NOT REMOVE
  
  while (!auto_started)
  {
    //////////////////////////////////////////
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(5, 20, "Battery: %d%%", Brain.Battery.capacity());
    Brain.Screen.printAt(5, 40, "Selected Auton:");

    if (Brain.Screen.pressing())
    {
      while (Brain.Screen.pressing())
      {
      } // Wait for release
      current_auton_selection = (current_auton_selection + 1) % 5; // Cycle to next auton

      // Event handlers for controller button presses

      Controller1.ButtonY.pressed([]()
                                  {
                                    current_auton_selection = (current_auton_selection + 1) % 5; // Cycle forward
                                  });
      Controller1.ButtonRight.pressed([]()
                                      {
                                        current_auton_selection = (current_auton_selection - 1 + 5) % 5; // Cycle backward
                                      });
    }

    // Display the selected autonomous route on both screens
    switch (current_auton_selection)
    {
    case 0:
      Brain.Screen.printAt(5, 60, "Ileri");
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      // Controller1.Screen.print("Ileri");
      break;
    case 1:
      Brain.Screen.printAt(5, 60, "Geri");
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("Geri");
      break;
    case 2:
      Brain.Screen.printAt(5, 60, "Saga");
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("Saga");
      break;
    case 3:
      Brain.Screen.printAt(5, 60, "Sola");
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("Sola");
      break;
    case 4:
      Brain.Screen.printAt(5, 60, "ileri");
      Controller1.Screen.setCursor(3, 1);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("ileri");
      break;
    }

    task::sleep(100); // Check frequently for taps
  } 
}

void autonomous(void)
{
  deger = chassis.get_absolute_heading();
  auto_started = true;
  Controller1.Screen.clearLine(3);
  // çalıştırmak istediğiniz otonomu buraya yazınız
  //  blueAwp();
  // redRightAwp();
  // redLeft();
  // RobotSkills();
  Skills();

  // RenkDeneme();
}

////////////////////////////////////////////



////////////////////////////////////////////
void usercontrol(void)
{
  
  while (1)
  {

    intake.setVelocity(127, pct);

    // usercontrol code here

    if(Controller1.ButtonR1.pressing()){
      intake.spin(fwd);
    }else if(Controller1.ButtonL1.pressing()){
      intake.spin(reverse);
    }
    // else {
    //   intake.stop();
    // }
    else if(Controller1.ButtonR2.pressing()){
      gatePiston.set(true);
      intake.spin(fwd);
    }else if(Controller1.ButtonL2.pressing()){
      sepet.set(true);
      intake.spin(forward);
    }else{
      sepet.set(false);
      gatePiston.set(false);
      intake.stop();
    }

    
    


    chassis.control_arcade();

    wait(20, msec);
  }
}










TeamColor detectBallColor() {
  if (!Optical.isNearObject()) {
    Brain.Screen.printAt(5, 100, "No Ball Detected");
    return UNKNOWN;
  }

  
  double hue = Optical.hue();

  if (hue >= 50 && hue <= 260 ) { // mavi aralığı
    Brain.Screen.printAt(5, 100, "Blue Ball Detected");
    return BLUE;
  }
  
  else if ((hue >= 0 && hue <= 49) || (hue >= 300 && hue <= 360)) { // kırmızı aralığı
    Brain.Screen.printAt(5, 100, "Red Ball Detected");
    return RED;
  }
  
  else {
    Brain.Screen.printAt(5, 100, "Unknown Ball");
    return UNKNOWN;
  }
}










void vites(){
  midTop.set(!midTop.value());
}

void load(){
  matchLoader.set(!matchLoader.value());
}

int main()
{

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  

  Controller1.Screen.print(teamColor);;

  intake.setVelocity(127, pct);

  RightDriveSmart.setVelocity(127, percent);
  LeftDriveSmart.setVelocity(127, percent);
  
  Optical.setLightPower(127, percent);
  Optical.setLight(ledState::on);

  Controller1.ButtonY.pressed(vites);
  Controller1.ButtonX.pressed(load);
    


  

  pre_auton();

  while (true)
  {
    
    wait(100, msec);
  }
}