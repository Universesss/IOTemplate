#include "vex.h"

// DEFAULT FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////
void default_constants()
{
    // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
    chassis.set_drive_constants(10, 1.5, 0, 10, 0);
    chassis.set_heading_constants(6, .4, 0, 1, 0);
    chassis.set_turn_constants(12, .4, .03, 3, 15);
    chassis.set_swing_constants(12, .3, .001, 2, 15);

    // Each exit condition set is in the form (settle_error, settle_time, timeout).
    chassis.set_drive_exit_conditions(1.5, 300, 5000);
    chassis.set_turn_exit_conditions(1, 300, 3000);
    chassis.set_swing_exit_conditions(1, 300, 3000);
}

///////////////////////////////////////////////////////////
//*Defines
///////////////////////////////////////////////////////////

bool SortColor = false;

TeamColor Ballcolor = UNKNOWN;

TeamColor BallCOlorWithAI = UNKNOWN;

///////////////////////////////////////////////////////////
//*Threads
///////////////////////////////////////////////////////////

int threadExample()
{
    return 0;
}

int value;

Location loc = Basket;
TeamColor Color = UNKNOWN;

void ColorSort(double duration)
{
    gatePiston.set(false);

    intake.setVelocity(85, pct);
    Optical.integrationTime(5);

    double startTime = Brain.Timer.time(sec);

    while (Brain.Timer.time(sec) - startTime < duration)
    {
        intake.spin(fwd);

        if (SortColor)
        {
            value = Optical.hue();

            if (value >= 100 && value <= 260)
            {
                if (Color == RED)
                {
                    if (loc == Middle)
                    {
                        midTop.set(true);
                        wait(0.3, sec);
                    }
                    else if (loc == Basket)
                    {
                        midTop.set(false);
                        gatePiston.set(true);
                        wait(0.3, sec);
                    }
                    else
                    {
                        gatePiston.set(false);
                        midTop.set(false);
                    }
                }

                if (Color == BLUE)
                {
                    if (loc == Basket)
                    {
                        midTop.set(false);
                        gatePiston.set(false);
                        wait(0.3, sec);
                    }
                    else if (loc == Middle)
                    {
                        midTop.set(false);
                        gatePiston.set(false);
                        wait(0.8, sec);
                    }
                    else
                    {
                        gatePiston.set(false);
                        midTop.set(true);
                    }
                }
            }
            else
            {
                if (Color == RED)
                {

                    midTop.set(false);
                    gatePiston.set(false);
                }
                else if (Color == BLUE)
                {
                    if (loc == Middle)
                    {
                        midTop.set(true);
                    }
                    else
                    {
                        midTop.set(false);
                    }
                    if (loc == Basket)
                    {
                        gatePiston.set(true);
                    }
                    else
                    {
                        gatePiston.set(false);
                    }
                }
            }
        }

        Brain.Screen.printAt(10, 140, "Hue: %.2f", Optical.hue());
        Brain.Screen.printAt(10, 160, "Brightness: %.2f", Optical.brightness());

        wait(10, msec);
    }
    gatePiston.set(false);
    midTop.set(false);

    intake.setVelocity(127, pct);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RenkDeneme()
{
    Color = RED; // alliance color
    SortColor = true;

    loc = Middle; // istenmeyen topun gideceği yer

    ColorSort(15); // renk ayırma fonksiyonu çalıştırılır, parametre süreyi belirler
}

void threadExamples()
{
    thread Thread1 = thread(threadExample);
}

void Skills()
{
    // intake.setVelocity(127, pct);
    gatePiston.set(true);
    midTop.set(false);
    chassis.drive_max_voltage = 6;

    // MatchLoad  --1
    chassis.drive_distance(30, -2);
    chassis.turn_to_angle(90);
    matchLoader.set(true);
    chassis.drive_distance(-7.2, 91);

    intake.spin(forward, 12, volt);
    chassis.drive_distance(17, 90);
    wait(0.3, seconds);

    // scoring   --TOP-1
    chassis.drive_distance(-24, 90.5);
    gatePiston.set(false);
    wait(0.1, seconds);
    sepet.set(true);
    wait(4.2, seconds);
    matchLoader.set(false);

    // MatchLoad  --2
    chassis.drive_distance(5, -90);
    chassis.turn_to_angle(-180);
    sepet.set(false);
    wait(0.1, seconds);
    chassis.drive_distance(8,-180); //
    wait(0.1, seconds);
    chassis.drive_distance(60, -93); // 80
    chassis.turn_to_angle(-90);      //
    chassis.drive_distance(25, -90); //
    wait(0.01, seconds);
    chassis.drive_distance(24.5, -35);
    gatePiston.set(true);
    chassis.turn_to_angle(-90);
    matchLoader.set(true);
    chassis.drive_distance(23, -89);
    wait(0.3, seconds);

    // scoring   --TOP-2
    chassis.drive_distance(-25, -92);
    sepet.set(true);
    wait(0.1, seconds);
    gatePiston.set(false);
    wait(3.4, seconds);
    matchLoader.set(false);

    // Parking
    chassis.drive_distance(18,-90);
    chassis.drive_distance(70, -160);
    chassis.turn_to_angle(0);
    LeftDriveSmart.spin(reverse, 11, volt);
    RightDriveSmart.spin(reverse, 12, volt);
    wait( 1.5, seconds);
    LeftDriveSmart.stop(hold);
    RightDriveSmart.stop(hold);
}

void RobotSkills()
{
    intake.setVelocity(127, pct);
    gatePiston.set(true);
    midTop.set(false);

    // MatchLoad  --1
    chassis.drive_distance(30, -2);
    chassis.turn_to_angle(90);
    matchLoader.set(true);
    chassis.drive_distance(-7.2, 91);

    intake.spin(forward);
    chassis.drive_distance(17, 90);
    wait(0.1, seconds);

    // scoring   --TOP-1
    chassis.drive_distance(-25, 90);
    sepet.set(true);
    wait(0.1, seconds);
    gatePiston.set(false);
    wait(3.4, seconds);
    matchLoader.set(false);

    // MatchLoad  --2
    chassis.drive_distance(4, -90);
    chassis.turn_to_angle(180);
    chassis.drive_distance(50, 270); // 80
    chassis.turn_to_angle(270);      //
    chassis.drive_distance(25, 270); //
    wait(0.01, seconds);
    chassis.drive_distance(30, -35);
    gatePiston.set(true);
    chassis.turn_to_angle(-90);
    matchLoader.set(true);
    chassis.drive_distance(21, -90);
    wait(0.3, seconds);

    // scoring   --TOP-2
    chassis.drive_distance(-25, -92);
    sepet.set(true);
    wait(0.1, seconds);
    gatePiston.set(false);
    wait(3.4, seconds);
    matchLoader.set(false);

    // Parking
    chassis.drive_distance(50, 180);
    chassis.turn_to_angle(0);
    chassis.drive_distance(-50, 0);
}

void redLeft()
{
    Color = RED;
    loc = Basket;

    gatePiston.set(true);

    // matchload
    chassis.drive_distance(30.5, 4);
    chassis.turn_to_angle(-90);
    matchLoader.set(true);
    chassis.drive_distance(-7.2, -91);

    intake.spin(forward);
    chassis.drive_distance(11.51, -90); // 13

    // scoring   --TOP-1
    chassis.drive_distance(-25, -93);
    sepet.set(true);
    wait(0.1, seconds);
    gatePiston.set(false);
    midTop.set(false);

    ColorSort(2.5);
    // wait(3,sec);

    // wait(0.5,seconds);

    wait(0.1, seconds);
    matchLoader.set(false);
    wait(0.1, seconds);

    chassis.drive_max_voltage = 12;

    // Scoring   --Mid-1
    gatePiston.set(true);
    sepet.set(false);

    chassis.drive_distance(7.5, 135);

    chassis.turn_to_angle(135);
    intake.spin(fwd);
    chassis.drive_max_voltage = 7;
    chassis.drive_distance(24, 135);
    chassis.drive_max_voltage = 12;
    chassis.turn_to_angle(-43);
    chassis.drive_distance(-14, -45);

    sepet.set(true);
    wait(0.1, sec);
    midTop.set(true);
    chassis.turn_to_angle(-46);
    chassis.drive_distance(2, -45);
}

void redRightAwp()
{

    gatePiston.set(true);

    // matchload
    chassis.drive_distance(30.2, -1);
    chassis.turn_to_angle(90);
    matchLoader.set(true);
    chassis.drive_distance(-7.2, 91);

    intake.spin(forward);
    chassis.drive_distance(13, 90);
    wait(0.1, seconds);

    // scoring   --TOP-1
    chassis.drive_distance(-24.8, 90);
    sepet.set(true);
    wait(0.1, seconds);
    gatePiston.set(false);
    wait(0.1, seconds);

    wait(0.7, seconds);
    matchLoader.set(false);
    wait(2.3, seconds);

    chassis.drive_max_voltage = 12;

    // Scoring   --Bottom-1
    gatePiston.set(true);
    sepet.set(false);
    chassis.drive_distance(3, 90);
    chassis.turn_to_angle(-180);

    chassis.drive_distance(10, -180);
    chassis.turn_to_angle(-135);
    intake.spin(fwd);
    chassis.drive_max_voltage = 6;
    chassis.drive_distance(41, -135);
    sepet.set(true);
    wait(0.7, sec);
    intake.spin(reverse);
}

void ileri()
{
    chassis.drive_distance(20, 0);
}
void saga()
{
    chassis.turn_to_angle(90);
}

void geri()
{
    chassis.drive_distance(-500, 0);
}
void sola()
{
    chassis.turn_to_angle(-90);
}
