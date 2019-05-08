/**
 * @file Test.h
 *
 * This file defines an option that <#...#>
 *
 * @author 米昱文
 */

option(Test)
{
    // 初始状态
  initial_state(start)
  {
      transition
      {
          
          if(state_time >= 100 && theLibCodeRelease.timeSinceBallWasSeen < 200)
              goto startOpenBall;
          // 是本队踢球，跳转
          if(theGameInfo.kickingTeam != theOwnTeamInfo.teamNumber)
              goto waitTenSecond;
          //太久没看到球，跳转
          if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
              goto searchForBall;
      }
      action
      {
          HeadControlMode(HeadControl::lookForward);
          Stand();
      }

  }
    
    state(waitTenSecond)
    {
        transition
        {
            if(state_time >= 12000 || std::abs(theTeamBallModel.position.x()) >= 200)
                goto turnToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            // 执行走到目标点的动作，第一个参数为速度，第二个参数为地点
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }
    
    // 开球
    state(startOpenBall)
    {
        transition
        {
            if(state_time > 800)
                goto moveBehindBallFirst;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            Stand();
            
        }
    }
    
    // 移动到球后位置
    state(moveBehindBallFirst)
    {
        //判断与球距离并跳转
        transition
        {
            if(theBallModel.estimate.position.norm() <= 350.f)
                goto alignBehindBallCarryingFirst;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
        }
    }
    
    
    state(alignBehindBallCarryingFirst)
    {
        transition
        {
            // 判断球的x， y是否在合适范围内
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -20.f, 0.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 130.f, 150.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto carryingBallFirst;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            // 移动到能够将球踢进的位置
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 140.f, theBallModel.estimate.position.y() + 10.f));
        }
    }
    
    //应该是踢球来让球前进
    state(carryingBallFirst)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto turnToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            // 这个踢的动作用的18初始的
            // 第一个参数是踢球的方法， 第二个是位置
            InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 130.f, theBallModel.estimate.position.y()));
            
        }
    }
    
    //走到球的位置
    state(turnToBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }
    
    //用的18初始的
    // 走向球
    state(walkToBall)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) > 5_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theBallModel.estimate.position.norm() < 350.f &&
               (theBallModel.estimate.position.y() >= 0))
                goto alignToGoalCarryingClock;
            if(theBallModel.estimate.position.norm() < 350.f &&
               (theBallModel.estimate.position.y() < 0))
                goto alignToGoalCarryingAntiClock;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
        }
    }
    
    
    state(alignToGoalCarryingClock) {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theTeamBallModel.position.x() >= 1500 && (theTeamBallModel.position.y() <= 2000 &&
                                                         theTeamBallModel.position.y() >= -2000))
                goto shootJudge;
            if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg &&
               std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBallCarrying;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200, theBallModel.estimate.position.y()+ 100.f));
        }
    }
    
    state(alignToGoalCarryingAntiClock)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theTeamBallModel.position.x() >= 1500 && (theTeamBallModel.position.y() <= 2000 &&
                                                         theTeamBallModel.position.y() >= -2000))
                goto shootJudge;
            if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg &&
               std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBallCarrying;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200, theBallModel.estimate.position.y()+ 100.f));
        }
    }
    
    state(alignBehindBallCarrying)
    {
        transition
        {
            if(theTeamBallModel.position.x() >= 1500 && (theTeamBallModel.position.y() <= 2000 &&
                                                         theTeamBallModel.position.y() >= -2000))
                goto shootJudge;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -60.f, -40.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 160.f, 190.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto carryingBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(carryingBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            std::vector<Obstacle> iObstacles = theObstacleModel.obstacles;
            for (auto& obj : iObstacles)
            {
                if(obj.type == Obstacle::Type::opponent)
                {
                    if(obj.center.norm()<=800 && obj.center.y() >= 0 && obj.center.x() > 0 && theTeamBallModel.position.y() >= -2000.f)
                        goto alignBehindBallRightObs; //side kick to break through the opponent
                    if(obj.center.norm()<=800 && obj.center.y() >= 0 && obj.center.x() > 0 && theTeamBallModel.position.y() < -2000.f)
                        goto alignBehindBallLeftObs; //break through the opponent to another side, because the ball is close to the edge
                    if(obj.center.norm()<=800 && obj.center.y() < 0 && obj.center.x() > 0 && theTeamBallModel.position.y() <= 2000.f)
                        goto alignBehindBallLeftObs;
                    if(obj.center.norm()<=800 && obj.center.y() < 0 && obj.center.x() > 0 && theTeamBallModel.position.y() > 2000.f)
                        goto alignBehindBallRightObs;
                }
            }
            if(state_time >= 800 && theLibCodeRelease.timeSinceBallWasSeen < 300)
            {
                goto turnToBall;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(alignBehindBallRightObs)
    {
        transition
        {
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -20.f, 0.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto kickBallAwayObstacleToRight;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 10.f));
        }
    }

    state(alignBehindBallLeftObs)
    {
        transition
        {
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 0.f, 20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto kickBallAwayObstacleToLeft;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 10.f));
        }
    }
    
    state(kickBallAwayObstacleToRight)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            PlaySound("arm.wav");
            //Stand();
            HeadControlMode(HeadControl::lookForward);
            InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::left), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() +20.f));
        }
    }
    
    state(kickBallAwayObstacleToLeft)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            PlaySound("arm.wav");
            InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() - 20.f));
        }
    }
    
    state(shootJudge)
    {
        transition
        {
            if(theTeamBallModel.position.y() >= 0)
                goto alignToGoalShootLeft;
            if(theTeamBallModel.position.y() < 0)
                goto alignToGoalShootRight;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //SystemCall::playSound("helpMe.wav");
            Stand();
        }
    }
    
    state(alignToGoalShootLeft)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theLibCodeRelease.angleToGoalLeft) < 10_deg)
                goto alignBehindBallShootLeft;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalLeft, theBallModel.estimate.position.x() - 400, theBallModel.estimate.position.y()+ 50.f));
        }
    }
    
    state(alignToGoalShootRight)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theLibCodeRelease.angleToGoalRight) < 10_deg)
                goto alignBehindBallShootRight;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalRight, theBallModel.estimate.position.x() - 400, theBallModel.estimate.position.y()+ 50.f));
        }
    }
    
    state(alignBehindBallShootLeft)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -70.f, -40.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 180.f)
               && std::abs(theLibCodeRelease.angleToGoalLeft) < 5_deg
               && theRobotPose.translation.x() >= 3200.f
               && theLibCodeRelease.between(theRobotPose.translation.y(), -2500.f, 2500.f))
                goto kick;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -70.f, -40.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 180.f)
               && std::abs(theLibCodeRelease.angleToGoalLeft) < 5_deg
               && theRobotPose.translation.x() < 3200.f)
                goto kickLong;
            
            if((theRobotPose.translation.x() >= 3200.f
                && theRobotPose.translation.y() <= -2500.f) ||(theRobotPose.translation.x() >= 3200.f
                                                               && theRobotPose.translation.y() >= 2500.f))
                goto turnToBall;
            
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalLeft, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() + 50.f));
        }
    }
    
    state(alignBehindBallShootRight)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -70.f, -40.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 180.f)
               && std::abs(theLibCodeRelease.angleToGoalRight) < 5_deg
               && theRobotPose.translation.x() >= 3200.f
               && theLibCodeRelease.between(theRobotPose.translation.y(), -2500.f, 2500.f))
                goto kick;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -70.f, -40.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 180.f)
               && std::abs(theLibCodeRelease.angleToGoalRight) < 5_deg
               && theRobotPose.translation.x() < 3200.f)
                goto kickLong;
            
            if((theRobotPose.translation.x() >= 3200.f
                && theRobotPose.translation.y() <= -2500.f) ||(theRobotPose.translation.x() >= 3200.f
                                                               && theRobotPose.translation.y() >= 2500.f))
                goto turnToBall;
            
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //PlaySound("fire.wav");
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalRight, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() + 50.f));
        }
    }
    
    state(kickLong)
    {
        transition
        {
            if(state_time > 800 && action_done)
                goto start;
        }
        action
        {
            Kick(KickRequest::kickForward);
        }
    }
    
    state(kick)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto start;
        }
        action
        {
            //Kick(KickRequest::kickForward );
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(searchForBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 150)
                goto turnToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkAtRelativeSpeed(Pose2f(0.6f, 0.f, 0.f));
        }
    }

}

