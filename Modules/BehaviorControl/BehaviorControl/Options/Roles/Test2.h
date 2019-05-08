/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Keeper2.h
 * Author: czq&&blb
 *
 * Created on March 27, 2018, 4:49 PM
 */


option(Test2)
{
    common_transition
    {
        if(Transformation::robotToField(theRobotPose,theBallModel.estimate.position).x() < 1800.f && theBallModel.estimate.velocity.x()<-300.f)
        {
            if(BallPhysics::getEndPosition(theBallModel.estimate.position, theBallModel.estimate.velocity,-0.6f).x() < 200.f)
            {
                if(BallPhysics::getEndPosition(theBallModel.estimate.position, theBallModel.estimate.velocity,-0.6f).y() > 200.f)
                    goto leftDefend;
                else if(BallPhysics::getEndPosition(theBallModel.estimate.position, theBallModel.estimate.velocity,-0.6f).y() < -200.f)
                    goto rightDefend;
                else
                    goto doDefend;
            }
            
    }
    }
    
    initial_state(start)
    {
        transition
        {
            if(state_time > 1000)
                goto standAtOwnGoal;
        }
        action
        {
            Stand();
            HeadControlMode(HeadControl::lookForward);
        }
    }
    
    state(standAtOwnGoal)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() < 1000.f  && theLibCodeRelease.timeSinceBallWasSeen < 200.f)
                goto startToKick;
            else
                goto walkToInitialPosition;
        }
        action
        {
            Stand();
            HeadControlMode(HeadControl::lookForward);
        }
    }
    
    state(startToKick)
    {
        transition
        {
            if(state_time>300)
            {
                goto turnToBall;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            Stand();
        }
    }
    
    state(turnToBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto walkToInitialPosition;
            if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
                goto walkToBall;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
//            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theBallModel.estimate.position.angle(), -4300.f, 200.f/(theBallModel.estimate.position.x()+4000.f)*theBallModel.estimate.position.y()));

            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }
    
    state(walkToBall)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() < 450.f && theLibCodeRelease.timeSinceBallWasSeen < 200.f)
                goto alignToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut ||
               theBallModel.estimate.position.norm() > 2000.f ||
               !theLibCodeRelease.between(theRobotPose.translation.x(),-4150.f,-3500.f) ||
               !theLibCodeRelease.between(theRobotPose.translation.y(),-800.f,800.f))
                goto walkToInitialPosition;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), theBallModel.estimate.position);
        }
    }
    
    
    
    state(alignToBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto walkToInitialPosition;
            if(std::abs(theLibCodeRelease.angleToGoalLeft) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall;
        }
        action
        {
            LookAtBall();
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToGoalLeft, theBallModel.estimate.position.x() - 450.f, theBallModel.estimate.position.y()));
        }
    }
    state(alignBehindBall)
    {
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto walkToInitialPosition;
            if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -70.f, -40.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 190.f, 210.f) && std::abs(theLibCodeRelease.angleToStrikerPointRight) < 2_deg)
                goto kick;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToStrikerPointRight, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 55.f));
        }
    }
    
    state(kick)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto standAfterKick;
        }
        action
        {
            HeadControlMode(HeadControl::lookForward);
            //KickForward(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theLibCodeRelease.angleToStrikerRight, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() + 55.f));
            Kick(KickRequest::kickForward);
        }
    }
    ////////////
    state(standAfterKick)
    {
        transition
        {
            if(state_time>2000 && action_done)
                goto judgeAfterKick;
        }
        action
        {
            LookAtBall();
            Stand();
        }
    }
    
    state(judgeAfterKick)
    {
        transition
        {
            if(theBallModel.estimate.position.norm()<1200.f && theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-3800.f) && theLibCodeRelease.between(theRobotPose.translation.y(),-800.f,800.f) && theLibCodeRelease.timeSinceBallWasSeen<200.f )
                goto startToKick;
            else
                goto walkToInitialPosition;
        }
    }
    
    state(walkToInitialPosition)
    {
        transition
        {
            if(theBallModel.estimate.position.norm()<1200.f && theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-2900.f) && theLibCodeRelease.between(theRobotPose.translation.y(),-800.f,800.f) && theLibCodeRelease.timeSinceBallWasSeen<200.f)
                goto startToKick;
            else if((theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-4350.f))
                    &&(theLibCodeRelease.between(theRobotPose.translation.y(),-25.f,25.f)))
                goto walkEnd;
        }
        action
        {
            //LookAtBall();
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-4400.f,0.f) ,Pose2f(0.8f, 0.8f, 0.8f));
            //WalkToTarget(Pose2f(80.f, 80.f, 80.f),Pose2f(theLibCodeRelease.angleToDoor,-4400.f,0.f));
        }
    }
    
    state(leftDefend)
    {
        transition
        {
            if(action_done && state_time > 2000)
                goto judgeAfterKick;
            // if((theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-4350.f))
            //       &&(theLibCodeRelease.between(theRobotPose.translation.y(),50.f,150.f)))
            //     goto specialActionA;
        }
        action
        {
            //WalkToTarget(Pose2f(50.f, 50.f, 70.f), Pose2f(0.f, 0.f, 100.f));
            SpecialAction(SpecialActionRequest::keeperSitDownAndStandUp);
            
        }
    }
    
    state(rightDefend)
    {
        transition
        {
            if(action_done && state_time > 2000)
                goto judgeAfterKick;
            // if((theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-4350.f))
            //       &&(theLibCodeRelease.between(theRobotPose.translation.y(),-150.f,-50.f)))
            //     goto specialActionB;
        }
        action
        {
            //WalkToTarget(Pose2f(50.f, 50.f, 70.f), Pose2f(0.f, 0.f, -100.f));
            SpecialAction(SpecialActionRequest::keeperSitDownAndStandUp);
            //SpecialAction(SpecialActionRequest::keeperJumpLeftPenalty, true);
            
        }
    }
    
    state(doDefend)
    {
        transition
        {
            if(state_time > 2000 && action_done)
                goto judgeAfterKick;
            //if((theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-4350.f))
            //      &&(theLibCodeRelease.between(theRobotPose.translation.y(),-50.f,50.f)))
            //    goto specialActionC;
        }
        action
        {
            //WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(0.f, 0.f, 0.f));
            SpecialAction(SpecialActionRequest::keeperSitDownAndStandUp);
        }
    }
    
    

    
    state(walkEnd)
    {
        transition
        {
            if(theBallModel.estimate.position.norm()<1200.f && theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-2900.f) && theLibCodeRelease.between(theRobotPose.translation.y(),-800.f,800.f) && theLibCodeRelease.timeSinceBallWasSeen < 200.f)
                goto startToKick;
            if(theLibCodeRelease.between(theRobotPose.translation.x(),-4150.f,-4050.f) && theLibCodeRelease.between(theRobotPose.translation.y(),-50.f,50.f))
                goto end;
        }
        action
        {
            
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-4100.f,0.f));
            HeadControlMode(HeadControl::lookForward);
            //WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(theLibCodeRelease.angleToCenter,-4400.f,0.f));
            //LookAtBall();
        }
    }
    
    state(end)
    {
        transition
        {
            if(theBallModel.estimate.position.norm()<1200.f && theLibCodeRelease.between(theRobotPose.translation.x(),-4450.f,-2900.f) && theLibCodeRelease.between(theRobotPose.translation.y(),-800.f,800.f)&& theLibCodeRelease.timeSinceBallWasSeen < 200.f)
                goto startToKick;
            //else if(state_time > 1000)
            //      goto walkToInitialPosition;
            //if(std::abs(theLibCodeRelease.angleToCenter)<5_deg)
            //   goto start;
        }
        action
        {
            LookAtBall();
            //theHeadControlMode = HeadControl::lookForward;
            //LookForward(theLibCodeRelease.angleToGoal);
            //Stand();
            WalkPathMode(Pose2f(theLibCodeRelease.angleToCenter,-4100.f,0.f));
            //WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(theLibCodeRelease.angleToCenter,0.f,0.f));
        }
    }
    
}



