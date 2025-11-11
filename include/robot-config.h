using namespace vex;

extern brain Brain;

//To set up a motor called LeftFront here, you'd use
//extern motor LeftFront;

//Add your devices below, and don't forget to do the same in robot-config.cpp:

extern inertial Intertial;

extern motor leftMotorA;
extern motor leftMotorB;
extern motor leftMotorC;
extern motor_group LeftDriveSmart;

extern motor rightMotorA;
extern motor rightMotorB;
extern motor rightMotorC;
extern motor_group RightDriveSmart;

extern controller Controller1;

extern vex::digital_out gatePiston;
extern vex::digital_out midTop;  
extern vex::digital_out matchLoader;
extern vex::digital_out sepet;     

extern motor intake1;
extern motor intake2;
extern motor_group intake;

extern aivision aiVision;
extern aivision::colordesc RED_DESC;
extern aivision::colordesc BLUE_DESC;

enum TeamColor { RED, BLUE, UNKNOWN };
extern TeamColor teamColor;

extern optical Optical;

void  vexcodeInit( void );