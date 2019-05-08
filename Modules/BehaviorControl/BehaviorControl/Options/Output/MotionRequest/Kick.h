/**
 * @file Kick.h
 *
 * This file defines an option that <#...#>
 *
 * @author 米昱文
 */

option(Kick, (KickRequest::KickMotionID) id, (bool)(false) mirror,(bool)(false) armsBackFix)
{
    /** Set the motion request. */
    initial_state(setRequest)
    {
        transition
        {
            if(theMotionInfo.motion == MotionRequest::kick &&
               theMotionInfo.kickRequest.kickMotionType == id &&
               theMotionInfo.kickRequest.mirror == mirror&&
               theMotionInfo.kickRequest.armsBackFix == armsBackFix)
                goto requestIsExecuted;
        }
        action
        {
            theMotionRequest.motion = MotionRequest::kick;
            theMotionRequest.kickRequest.kickMotionType = id;
            theMotionRequest.kickRequest.mirror = mirror;
            theMotionRequest.kickRequest.armsBackFix = armsBackFix;
        }
    }
    
    /** The motion process has started executing the request. */
    target_state(requestIsExecuted)
    {
        transition
        {
            if(theMotionInfo.motion != MotionRequest::kick ||
               theMotionInfo.kickRequest.kickMotionType != id ||
               theMotionInfo.kickRequest.mirror != mirror||
               theMotionInfo.kickRequest.armsBackFix != armsBackFix)
                goto setRequest;
        }
        action
        {
            theMotionRequest.motion = MotionRequest::kick;
            theMotionRequest.kickRequest.kickMotionType = id;
            theMotionRequest.kickRequest.mirror = mirror;
            theMotionRequest.kickRequest.armsBackFix = armsBackFix;
        }
    }
}
