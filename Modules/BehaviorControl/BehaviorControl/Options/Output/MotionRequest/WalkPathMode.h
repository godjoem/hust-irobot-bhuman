/**
 * @file WalkPathMode.h
 *
 * This file defines an option that <#...#>
 *
 * @author 米昱文
 */

option(WalkPathMode, (const Pose2f&) target, (const Pose2f&)(Pose2f(1.f,1.f,1.f)) speed, (bool)(true)excludePArea)
{
  initial_state(start)
  {
      action
      {
          theMotionRequest = thePathPlanner.plan(target, speed, excludePArea);
      }
  }
}
