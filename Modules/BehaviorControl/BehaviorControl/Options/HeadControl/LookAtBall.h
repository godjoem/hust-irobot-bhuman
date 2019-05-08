/**
 * @file LookAtBall.h
 *
 * This file defines an option that <#...#>
 *
 * @author 米昱文
 */

option(LookAtBall)
{
  initial_state(start)
  {
      transition
      {
          if(action_done)
              goto lookAtBallTarget;
      }
      action
      {
          SetHeadTargetOnGround(Vector3f(theBallModel.estimate.position.x(),theBallModel.estimate.position.y(),50.f),HeadMotionRequest::autoCamera,150_deg);
      }
  }
    
    state(lookAtBallTarget)
    {
        if(action_done)
            goto start;
        action
        {
            //SetHeadTarget(Vector3f(theBallModel.estimate.position.x()-theRobotPose.translation.x(),theBallModel.estimate.position.y()-theRobotPose.translation.y(),50.f),HeadMotionRequest::autoCamera,false,toDegrees(150.f));
            SetHeadTargetOnGround(Vector3f(theBallModel.estimate.position.x(),theBallModel.estimate.position.y(),50.f),HeadMotionRequest::autoCamera,150_deg);
        }
    }
}
