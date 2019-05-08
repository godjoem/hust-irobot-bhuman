/**
 * @file Defender1.h
 *
 * This file defines an option that the right defender
 *
 * @author 米昱文
 */

option(Defender2)
{
    initial_state(start)
    {
        transition
        {
            if(theRobotPose.translation.x() > 1000.f || theRobotPose.translation.y() > 0.f)
                goto walkBack;
            if(state_time > 1000)
                goto waitBall;
            
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            Stand();
        }
    }
    
    
    state(waitBall)
    {
        transition
        {
            if(state_time > 800 && theBallModel.estimate.position.norm() <= 2000)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //SystemCall::playSound("helpMe.wav");
            Stand();
        }
    }
    
    state(turnToBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theBallModel.estimate.position.angle()) <5_deg)
                goto walkToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //SystemCall::playSound("helpMe.wav");
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }
    
    state(walkToBall)
    {
        transition
        {
            if(theRobotPose.translation.x() > 1000.f || theRobotPose.translation.y() > 0.f)
                goto walkBack;
            if(std::abs(theBallModel.estimate.position.angle()) > 20_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theBallModel.estimate.position.norm() < 350.f && (theTeamBallModel.position.y() >= theRobotPose.translation.y()))
                goto alignToGoalCarryingClock;
            if(theBallModel.estimate.position.norm() < 350.f)
                goto alignToGoalCarryingAntiClock;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
        }
    }
    
    state(alignToGoalCarryingClock)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) > 90_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theTeamBallModel.position.x() >= 2500 && (theTeamBallModel.position.y()<=2000 && theTeamBallModel.position.y() >= -2000))
                goto shootJudge;
            if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
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
            if(std::abs(theBallModel.estimate.position.angle()) > 90_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(theTeamBallModel.position.x() >= 2500 && (theTeamBallModel.position.y()<=2000 && theTeamBallModel.position.y() >= -2000))
                goto shootJudge;
            if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBallCarrying;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 200, theBallModel.estimate.position.y()- 100.f));
        }
    }
    
    state(alignBehindBallCarrying)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) > 80_deg)
                goto turnToBall;
            //  if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
            //    goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 40.f, 60.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto carryingBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //Stand();
            //SystemCall::playSound("helpMe.wav");
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
        }
    }
    
    state(carryingBall)
    {
        transition
        {
            //judge whether it needs to execute the state of avoiding obstacle or not
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
                
                if(theBallModel.estimate.position.norm() >= 0)
                    goto turnToBall;
                
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
        }
    }
    
    state(alignBehindBallRightObs)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) > 90_deg)
                goto turnToBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -20.f, 0.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto kickBallAwayObstacleToRight;
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
            if(std::abs(theBallModel.estimate.position.angle()) > 90_deg)
                goto turnToBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 0.f, 20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 2_deg)
                goto kickBallAwayObstacleToLeft;
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
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            PlaySound("arm.wav");
            //Stand();
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(kickBallAwayObstacleToLeft)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto turnToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            PlaySound("arm.wav");
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 170.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    
    //Prepare to shoot
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
            if(std::abs(theBallModel.estimate.position.angle()) > 120_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theLibCodeRelease.angleToGoalLeft) < 10_deg)
                goto alignBehindBallShootLeft;
        }
        action
        {
            HeadControlMode(HeadControl::lookAtBall);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoalLeft, theBallModel.estimate.position.x() - 400, theBallModel.estimate.position.y()+ 50.f));
        }
    }
    
    state(alignToGoalShootRight)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) > 120_deg)
                goto turnToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theLibCodeRelease.angleToGoalRight) < 10_deg)
                goto alignBehindBallShootRight;
        }
        action
        {
            HeadControlMode(HeadControl::lookAtBall);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoalRight, theBallModel.estimate.position.x() - 400, theBallModel.estimate.position.y()+ 50.f));
        }
    }
    
    state(alignBehindBallShootLeft)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theBallModel.estimate.position.angle()) > 120_deg)
                goto turnToBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoalLeft) < 5_deg
               && theRobotPose.translation.x() < 3200.f)
                goto kickLong;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoalLeft) < 5_deg
               && theRobotPose.translation.x() >= 3200.f
               && theLibCodeRelease.between(theRobotPose.translation.y(), -2500.f, 2500.f))
                goto kick;
            if((theRobotPose.translation.x() >= 3200.f
                && theRobotPose.translation.y() <= -2500.f) ||(theRobotPose.translation.x() >= 3200.f
                                                               && theRobotPose.translation.y() >= 2500.f))
                goto turnToBall;
            
        }
        action
        {
            HeadControlMode(HeadControl::lookAtBall);
            //PlaySound("fire.wav");
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalLeft, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() + 30.f));
        }
    }
    
    state(alignBehindBallShootRight)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
            if(std::abs(theBallModel.estimate.position.angle()) > 120_deg)
                goto turnToBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoalRight) < 5_deg
               && theRobotPose.translation.x() < 3200.f)
                goto kickLong;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f, -20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
               && std::abs(theLibCodeRelease.angleToGoalRight) < 5_deg
               && theRobotPose.translation.x() >= 3200.f
               && theLibCodeRelease.between(theRobotPose.translation.y(), -2500.f, 2500.f))
                goto kick;
            if((theRobotPose.translation.x() >= 3200.f
                && theRobotPose.translation.y() <= -2500.f) ||(theRobotPose.translation.x() >= 3200.f
                                                               && theRobotPose.translation.y() >= 2500.f))
                goto turnToBall;
            
        }
        action
        {
            HeadControlMode(HeadControl::lookAtBall);
            //PlaySound("fire.wav");
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalRight, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() + 30.f));
        }
    }
    
    state(kickLong)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
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
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    
    state(searchForBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > 2000)
                goto searchForBall;
            if(theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto turnToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
        }
    }
    
    state(walkBack)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() < 1000.f && theRobotPose.translation.x() < 0.f)
                goto turnToBall;
            if((theRobotPose.translation-Vector2f(-2800.f,-1100.f)).norm() < 100.f && std::abs(theLibCodeRelease.angleToGoal) < 5_deg)
                goto waitBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal,(Vector2f(-2800.f,-1100) - theRobotPose.translation)));
        }
    }
}
