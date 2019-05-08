///*
// * File:   LookAround.h
// * Author: tulip
// *
// * Created on 2017?3?10?, ??9:50
// */
//
//#ifndef LOOKAROUND_H
//#define    LOOKAROUND_H
//option(LookAround)
//{
//    initial_state(lookAround)
//    {
//        transition
//        {
//            if(state_time > 100)
//                goto lookLeft;
//        }
//        action
//        {
//            Stand();
//        }
//    }
//    
//    state(lookLeft)
//    {
//        transition
//        {
//            if(state_time > 500)
//                goto wait1;
//        }
//        action
//        {
//            SetHeadPanTilt(-0.8f, -0.35f, 30_deg);
//        }
//    }
//    
//    state(wait1)
//    {
//        transition
//        {
//            if(state_time >= 600)
//                goto lookCenter1;
//        }
//    }
//    
//    state(lookCenter1)
//    {
//        transition
//        {
//            if(action_done)
//                goto wait2;
//        }
//        action
//        {
//            SetHeadPanTilt(0.f, 0.35f, 30_deg);
//        }
//    }
//    
//    state(wait2)
//    {
//        transition
//        {
//            if(state_time >= 600)
//                goto lookRight;
//        }
//    }
//    
//    state(lookRight)
//    {
//        transition
//        {
//            if(action_done)
//                goto wait3;
//        }
//        action
//        {
//            SetHeadPanTilt(0.8f, -0.35f, 30_deg);
//        }
//    }
//    
//    state(wait3)
//    {
//        transition
//        {
//            if(state_time >= 600)
//                goto lookCenter2;
//        }
//    }
//    
//    state(lookCenter2)
//    {
//        transition
//        {
//            if(action_done)
//                goto wait4;
//        }
//        action
//        {
//            SetHeadPanTilt(0.f, 0.35f, 30_deg);
//        }
//    }
//    
//    state(wait4)
//    {
//        transition
//        {
//            if(state_time >= 400)
//                goto lookLeft;
//        }
//    }
//}
//
//
//#endif    /* LOOKAROUND_H */
