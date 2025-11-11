#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"
#include "IO-Template/global.h"
#include "IO-Template/drive.h"
#include "IO-Template/util.h"
#include "IO-Template/PID.h"
#include "autons.h"



extern TeamColor detectAllianceColor();
extern TeamColor detectBallColor();

extern TeamColor detectBallColorWithAI();



#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)

  