/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"

STREAMABLE(LibCodeRelease,
{
  FUNCTION(bool(float value, float min, float max)) between,

  (float) angleToGoal,
  //(float) angleToGoal1,        
  (float) angleToGoalLeft,
  (float) angleToGoalRight,
  (float) angleToPenaltyPoint,
  (float) angleToCenter,
  (float) angleToBackPointRight,
  (float) angleToBackPointLeft,
  (float) angleToStrikerPointLeft,
  (float) angleToStrikerPointRight,
  (float) angleToKeeperPoint,
  (float) angleToRightCorner,
  (float) angleToLeftCorner,
  (int) timeSinceBallWasSeen,
});
