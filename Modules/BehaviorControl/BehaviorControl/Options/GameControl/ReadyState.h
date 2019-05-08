/** behavior for the ready state */
// 准备状态根据不同角色令机器人去到不同位置
option(ReadyState)
{
  /* position has been reached -> stand and wait */
  initial_state(stand)
  {
      
      transition
      {
          if(action_done)
              goto chooserole;
      }

    action
    {
      HeadControlMode(HeadControl::lookForward);
      Stand();
    }
  }
    
    state(chooserole)
    {
        transition
        {
            if(theRobotInfo.number == 1)
                goto walkToKeeperPoint;
            else if(theRobotInfo.number == 5)
            {
                if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
                    goto walkToStrikerPoint;
                else
                    goto walkToStrikerKickOffPoint;
            }
            else if(theRobotInfo.number == 2)
            {
                if(theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
                    goto walkToSideStrikerPoint;
                else
                    goto walkToSideStrikerKickOffPoint;
            }
            else if(theRobotInfo.number == 4)
                goto walkToLeftDefenderPoint;
            else if(theRobotInfo.number == 3)
                goto walkToRightDefenderPoint;
        }
    }
    
    state(walkToKeeperPoint)
    {
        transition
        {
            if((theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-4350.f))
               &&(theLibCodeRelease.between(theRobotPose.translation.y(),-50.f,50.f)))
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToGoal,-4400.f,0.f));
        }
    }
    
    state(walkToStrikerPoint)
    {
        transition
        {
            if(theLibCodeRelease.between(theRobotPose.translation.x(),-950.f,-850.f) &&
               theLibCodeRelease.between(theRobotPose.translation.y(),-5.f,5.f) )
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-900.f,0.f));
        }
    }
    
    state(walkToStrikerKickOffPoint)
    {
        transition
        {
            if(theLibCodeRelease.between(theRobotPose.translation.x(),-255.f,-245.f) &&
               theLibCodeRelease.between(theRobotPose.translation.y(),-5.f,5.f) )
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-250.f,0.f));
        }
    }
    
    state(walkToSideStrikerPoint)
    {
        transition
        {
            if(theLibCodeRelease.between(theRobotPose.translation.x(),-950.f,-850.f) &&
               theLibCodeRelease.between(theRobotPose.translation.y(),1450.f,1550.f) )
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-900.f,1500.f));
        }
    }
    
    state(walkToSideStrikerKickOffPoint)
    {
        transition
        {
            if(theLibCodeRelease.between(theRobotPose.translation.x(),-255.f,-245.f) &&
               theLibCodeRelease.between(theRobotPose.translation.y(),1450.f,1550.f) )
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-250.f,1500.f));
        }
    }
    
    state(walkToLeftDefenderPoint)
    {
        transition
        {
            if((theLibCodeRelease.between(theRobotPose.translation.x(),-2850.f,-2750.f))
               &&(theLibCodeRelease.between(theRobotPose.translation.y(),1050.f,1150.f)))
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToGoal,-2800.f,1100.f));
        }
    }
    
    state(walkToRightDefenderPoint)
    {
        transition
        {
            if((theLibCodeRelease.between(theRobotPose.translation.x(),-2850.f,-2750.f))
               &&(theLibCodeRelease.between(theRobotPose.translation.y(),-1150.f,-1050.f)))
            {
                goto turnToCenter;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkPathMode(Pose2f(theLibCodeRelease.angleToGoal,-2800.f,-1100.f));
        }
    }
    
    
    state(turnToCenter)
    {
        transition
        {
            if(std::norm(theLibCodeRelease.angleToCenter)<5_deg)
            {
                goto end;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theLibCodeRelease.angleToCenter,0.f,0.f));
        }
    }
    
    state(end)
    {
        action
        {
            HeadControlMode(HeadControl::lookForward);
            Stand();
        }
    }
}
