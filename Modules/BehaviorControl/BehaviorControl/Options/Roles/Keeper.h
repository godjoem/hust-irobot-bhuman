/**
 * @file Keeper.h
 *
 * This file defines an option that <#...#>
 *
 * @author 米昱文
 */

option(Keeper)
{
    initial_state(start)
    {
        //         if(theBallModel.estimate.position.norm() <= 500 && theBallModel.estimate.position.norm() > 0)
        //                goto walkToBall;
        transition
        {
            if(state_time>3000)
                goto Ready;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            //PlaySound("allright.wav");
            Stand();
        }
    }
    
    state(walkToPoint)
    {
        transition
        {
            if((theRobotPose.translation-Vector2f(-4400.f,0.f)).norm()<100.f && std::norm(theLibCodeRelease.angleToCenter)<5_deg)   //keeper
                goto finalState;
            if(theBallModel.estimate.position.norm() <= 800.f && theBallModel.estimate.position.norm() > 0)
                goto Ready;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToCenter,(Vector2f(-4400.f,0.f) - theRobotPose.translation)));  //keeper
        }
    }
    
    
    aborted_state(finalState)
    {
        transition
        {
            if((theRobotPose.translation-Vector2f(-4400.f,0.f)).norm()<=100.f)   //???
                goto Ready;
            if((theRobotPose.translation-Vector2f(-4400.f,0.f)).norm() > 100.f)   //???
                goto walkToPoint;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            Stand();
        }
    }
    
    
    state(Ready)
    {
        transition
        {
            
            if(theBallModel.estimate.position.norm() <= 500.f && theBallModel.estimate.position.norm() > 0)
                goto walkToBall;
            if(theTeamBallModel.position.x() <= -1000 && theTeamBallModel.position.x() >= -3900.f)
                    goto MoveFront;
            if(theTeamBallModel.position.x() <= -1000 && theTeamBallModel.position.x() <=  -3900.f && theTeamBallModel.position.y() <= 0)
                goto MoveRight;
            if(theTeamBallModel.position.x() <= -1000 && theTeamBallModel.position.x() <=  -3900.f && theTeamBallModel.position.y() > 0)
                goto MoveLeft;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            Stand();
        }
    }
    
    state(MoveFront)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() <= 800 && theBallModel.estimate.position.norm() > 0)
                goto walkToBall;
            Vector2f theAbsoluteBallPosition=Transformation::robotToField(theRobotPose,theBallModel.estimate.position);
            float YRobot=300*theAbsoluteBallPosition.y()/(4500-std::abs(theAbsoluteBallPosition.x()));
            if(theBallModel.estimate.position.norm() <= 1000.f && theBallModel.estimate.position.norm() > 0)
                goto Keep;
            if(theBallModel.estimate.velocity.x() ==0 && theBallModel.estimate.velocity.y() ==0
               && (theRobotPose.translation-Vector2f(-4200.f,YRobot)).norm() < 100.f
               && std::abs(theBallModel.estimate.position.angle()) <= 5_deg
               && state_time > 800) //&& theLibCodeRelease.between(theRobotPose.translation.y(),(0.367*theAbsoluteBallPosition.y()-50.f),(0.367*theBallModel.estimate.position.y()+50.f))
                goto ReadyToKeep;
            if(theBallModel.estimate.position.norm() > 2000)
                goto walkToPoint;
        }
        action
        {
            Vector2f theAbsoluteBallPosition=Transformation::robotToField(theRobotPose,theBallModel.estimate.position);
            float YRobot=300*theAbsoluteBallPosition.y()/(4500-std::abs(theAbsoluteBallPosition.x()));
            //PlaySound("arm.wav");
            HeadControlMode(HeadControl::lookAtBall);
            //WalkPathMode(Pose2f(theBallModel.estimate.position.angle() + theRobotPose.rotation ,-4200.f,YRobot),Pose2f(1.f,1.f,1.f),false);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theBallModel.estimate.position.angle(),(Vector2f(-4200.f,YRobot) - theRobotPose.translation)));
        }
    }
    
    state(MoveRight)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() <= 800 && theBallModel.estimate.position.norm() > 0)
                goto walkToBall;
            if(theBallModel.estimate.position.norm() <= 1000.f  && theBallModel.estimate.position.norm() > 0)
                goto Keep;
            if(std::abs(theBallModel.estimate.position.angle()) <= 5_deg && (theRobotPose.translation-Vector2f(-4200.f,-1100.f)).norm() < 100.f)
                goto ReadyToKeep;
            if(theLibCodeRelease.timeSinceBallWasSeen> 2000.f)
                goto walkToPoint;
        }
        action
        {
            //Vector2f theAbsoluteBallPosition=Transformation::fieldToRobot(theRobotPose.translation,theBallModel.estimate.position);
            //PlaySound("arm.wav");
            HeadControlMode(HeadControl::lookAtBall);
            WalkPathMode(Pose2f((theBallModel.estimate.position.angle() + theRobotPose.rotation),-4200.f,-1100.f),Pose2f(1.f,1.f,1.f),false);
            //WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theBallModel.estimate.position.angle() , (Vector2f(-4300.f,-1100.f) - theRobotPose.translation)));
        }
    }
    
    state(MoveLeft)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() <= 800 && theBallModel.estimate.position.norm() > 0)
                goto walkToBall;
            if(theBallModel.estimate.position.norm() <= 1000.f && theBallModel.estimate.position.norm() > 0)
                goto Keep;
            //            if(std::abs(theBallModel.estimate.position.angle()) >= 20_deg)
            //                goto Ready;
            if(std::abs(theBallModel.estimate.position.angle()) <= 5_deg && (theRobotPose.translation-Vector2f(-4200.f,1100.f)).norm() < 100.f)
                goto ReadyToKeep;
            if(theLibCodeRelease.timeSinceBallWasSeen> 2000.f)
                goto walkToPoint;
        }
        action
        {
            //Vector2f theAbsoluteBallPosition=Transformation::fieldToRobot(theRobotPose.translation,theBallModel.estimate.position);
            //PlaySound("arm.wav");
            HeadControlMode(HeadControl::lookAtBall);
            WalkPathMode(Pose2f((theBallModel.estimate.position.angle() + theRobotPose.rotation),-4200.f,1100.f),Pose2f(1.f,1.f,1.f),false);
            //WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theBallModel.estimate.position.angle() ,   (Vector2f(-4300.f,1100.f) - theRobotPose.translation)));
        }
    }
    
    state(ReadyToKeep)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() <= 800 && theBallModel.estimate.position.norm() > 0)
                goto walkToBall;
            if(theBallModel.estimate.velocity.x() > 0 && theBallModel.estimate.position.norm() <= 500 && theBallModel.estimate.position.norm() > 0)
                goto Keep;
            if((theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.x()), 100.f,1000.f) || theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.y()), 100.f,800.f)) && theTeamBallModel.position.x() > -3900 && state_time >=1000)
                goto MoveFront;
            if((theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.x()), 100.f,1000.f) || theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.y()), 100.f,800.f)) && theTeamBallModel.position.x() <  -3900 && theTeamBallModel.position.y() < 0 && state_time >=1000)
                goto MoveRight;
            if((theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.x()), 100.f,1000.f) || theLibCodeRelease.between(std::abs(theBallModel.estimate.velocity.y()), 100.f,800.f)) && theTeamBallModel.position.x() <  -3900 && theTeamBallModel.position.y() > 0 && state_time >=1000)
                goto MoveLeft;
            if(theLibCodeRelease.timeSinceBallWasSeen> 2000 || theBallModel.estimate.position.norm() >= 4000)
                goto Back;
        }
        action
        {
            //PlaySound("greetings.wav");
            HeadControlMode(HeadControl::lookAtBall);
            Stand();
        }
    }
    
    state(Keep)
    {
        transition
        {
            if(action_done)
                goto delay;
        }
        action
        {
            HeadControlMode(HeadControl::lookAtBall);
            SpecialAction(SpecialActionRequest::keeperSitDownAndStandUp);
        }
    }
    
    state(delay)
    {
        transition
        {
            if(state_time>600)
                goto confirm;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            Stand();
        }
    }
    
    state(confirm)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() <= 1100 && theBallModel.estimate.position.norm() > 0
               && std::abs(theBallModel.estimate.velocity.x()) <= 200 && std::abs(theBallModel.estimate.velocity.y()) <= 200)
                goto walkToBall;
            if(theBallModel.estimate.position.norm() <= 1100
               && std::abs(theBallModel.estimate.velocity.x()) > 0
               && std::abs(theBallModel.estimate.velocity.y()) > 0
               && theBallModel.estimate.position.norm() > 0)
                goto Ready;
            
            if(theBallModel.estimate.position.norm() > 800 || theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto walkToPoint;
        }
        action
        {
            //PlaySound("high.wav");
            HeadControlMode(HeadControl::lookAround);
            Stand();
        }
    }
    
    state(walkToBall)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() > 1000)
                goto Back;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto walkToPoint;
            if(theBallModel.estimate.position.norm() < 500.f && theTeamBallModel.position.y() <= theRobotPose.translation.y() && theBallModel.estimate.position.norm() > 0)
                goto alignBesideBallRight;
            if(theBallModel.estimate.position.norm() < 500.f && theTeamBallModel.position.y() > theRobotPose.translation.y() && theBallModel.estimate.position.norm() > 0)
                goto alignBesideBallLeft;
        }
        action
        {
            //PlaySound("lost.wav");
            HeadControlMode(HeadControl::lookAround);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
        }
    }
    
    //移动到从左踢球的角度
    state(alignBesideBallLeft)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > 2000)
                goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -30.f, 20.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 300.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 75_deg)
                goto sideKickLeft;
        }
        action
        {
            //PlaySound("ohno.wav");
            HeadControlMode(HeadControl::lookAround);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() -10.f));
        }
    }
    
    state(alignBesideBallRight)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > 2000)
                goto searchForBall;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -20.f, 30.f)
               && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 300.f)
               && std::abs(theLibCodeRelease.angleToGoal) < 75_deg)
                goto sideKickRight;
        }
        action
        {
            PlaySound("ohno.wav");
            HeadControlMode(HeadControl::lookAround);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() +10.f));
        }
    }
    
    state(sideKickLeft)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto Back;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::left), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
        }
    }
    
    state(sideKickRight)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto Back;
        }
        action
        {
            HeadControlMode(HeadControl::lookAround);
            InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::right), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(Back)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() < 400 && theBallModel.estimate.position.norm() > 0)
                goto delay;
            if(theBallModel.estimate.position.norm() < 2000 && theBallModel.estimate.position.norm() > 0)
                goto Ready;
            if((theRobotPose.translation-Vector2f(-4400.f,0.f)).norm()<100.f)
                goto Ready;
        }
        action
        {
            //PlaySound("normalPlay.wav");
            HeadControlMode(HeadControl::lookAround);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToCenter,(Vector2f(-4400.f,0.f) - theRobotPose.translation)));
        }
    }
    
    state(searchForBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 300)
                goto Ready;
            if(theLibCodeRelease.timeSinceBallWasSeen >= 10000)
                goto Back;
        }
        action
        {
            //PlaySound("ground.wav");
            HeadControlMode(HeadControl::lookAround);
            Stand();
        }
    }
}
