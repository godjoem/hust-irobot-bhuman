/**
 * @file ChooseRole.h
 *
 * This file defines an option that 根据robotinfo分配角色
 *
 * @author 米昱文
 */

option(ChooseRole)
{
  initial_state(start)
  {
      transition
      {
          if(state_time > 1000)
              goto judge;
      }
      action
      {
          LookForward();
          Stand();
      }
  }
    
    state(judge)
    {
        transition
        {
            if(theRobotInfo.number == 1)
                goto keeper;
            else if(theRobotInfo.number == 2 || theRobotInfo.number == 5)
                goto striker;
            else if(theRobotInfo.number == 4)
                goto defender1;
            else if(theRobotInfo.number == 3)
                goto defender2;
            else
                goto stand;
        }
    }
    
    state(keeper)
    {
        action
        {
            Test2();
        }
    }
    
    state(striker)
    {
        action
        {
            Test();
        }
    }
    
    state(defender1)
    {
        action
        {
            Defender1();
        }
    }
    
    state(defender2)
    {
        action
        {
            Defender2();
        }
    }
    
    state(stand)
    {
        action
        {
            LookForward();
            Stand();
        }
    }
}
