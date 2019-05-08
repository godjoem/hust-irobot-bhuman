/**
 * @file LibCodeReleaseProvider.cpp
 */

#include "LibCodeReleaseProvider.h"

MAKE_MODULE(LibCodeReleaseProvider, behaviorControl);

void LibCodeReleaseProvider::update(LibCodeRelease& libCodeRelease)
{
  libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen);
  libCodeRelease.angleToGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline + 200.f, 0.f)).angle();
  libCodeRelease.angleToGoalLeft = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline + 200.f, +400.f)).angle();
  libCodeRelease.angleToGoalRight = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline +200.f, -400.f)).angle();
  libCodeRelease.angleToPenaltyPoint = (theRobotPose.inversePose * Vector2f(-2700.f,0.f)).angle();
  libCodeRelease.angleToCenter = (theRobotPose.inversePose * Vector2f(0.f,0.f)).angle();
  libCodeRelease.angleToBackPointLeft = (theRobotPose.inversePose * Vector2f(-3200.f,+1000.f)).angle();
  libCodeRelease.angleToBackPointRight = (theRobotPose.inversePose * Vector2f(-2700.f,-1000.f)).angle();
  libCodeRelease.angleToStrikerPointLeft = (theRobotPose.inversePose * Vector2f(-850.f,+800.f)).angle();
  libCodeRelease.angleToStrikerPointRight = (theRobotPose.inversePose * Vector2f(-850.f,-800.f)).angle();
  libCodeRelease.angleToKeeperPoint = (theRobotPose.inversePose * Vector2f(-4200.f,0.f)).angle();
  libCodeRelease.angleToRightCorner = (theRobotPose.inversePose * Vector2f(-4500.f,0.f)).angle();
  libCodeRelease.angleToLeftCorner = (theRobotPose.inversePose * Vector2f(4500.f,0.f)).angle();

  libCodeRelease.between = [&](float value, float min, float max) -> bool
  {
    return value >= min && value <= max;
  };
}
