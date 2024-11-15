#include "menu_02.h"
#include "controls.h"
#include "masterLog.h"


#define MAX_TIME 60
#define DEFAULT_TIME 10
#define MIN_TIME 05
#define DIFF_TIME 05

autoFlushTimers flushTimer;

// // Helper function to check if the menu label matches the target menu label
// bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t childMenu = 0)
// {
//     return strcmp(menu->Items[childMenu].label, targetThreshold.Items[childMenu].label) == 0;
// }

bool autoFlushTimerAdjValue(bool timerDefaultMode)
{
    static bool isDefaultTimer = timerDefaultMode;
    static uint8_t timerVal = isDefaultTimer ? DEFAULT_TIME: MIN_TIME;

    if (ButtonD.state() && timerVal > MIN_TIME)
    {
        timerVal -= DIFF_TIME;
    }
    else if (ButtonU.state() && timerVal < MAX_TIME)
    {
        timerVal += DIFF_TIME;
    }

    if (ButtonE.state() || isDefaultTimer)
    {
        if (flushTimer.selectTimer == STARTUP_TIMER)
        {
            flushTimer.startUpTimer = timerVal;
        }
        else if (flushTimer.selectTimer == SUBSEQUENT_TIMER)
        {
            flushTimer.subsequentTimer = timerVal;
        }
        else
        {
            Flags.setFlag(Settings, AUTO_FLUSH, false);
            return false;
        }

        DEBUG_RTT(LOG_WARNING, "Timer value set to %d.", timerVal);
        Flags.setFlag(Settings, AUTO_FLUSH, true);
        timerVal = MIN_TIME;
        isDefaultTimer = false;
        flushTimer.selectTimer = NO_TIMER;
        return true;
    }
    Disp->display(timerVal);
}

bool autoFlushLoop(uint32_t argChoice, MenuItem *argMenu)
{
    if (ButtonE.state())
    {
        switch (argChoice)
        {
        case 1:  return Flags.setFlag(Settings, AUTO_FLUSH, true);
        case 2:  return Flags.setFlag(Settings, AUTO_FLUSH, false);
        case 3:  flushTimer.selectTimer = STARTUP_TIMER; return true;
        case 4:  flushTimer.selectTimer = SUBSEQUENT_TIMER; return true;
        }
    }
    return false;
}

bool autoFlushTimerLoop(uint32_t argChoice, MenuItem *argMenu)
{
    static bool autoFlushTimer = false;
    
    if (!autoFlushTimer && ButtonE.state())
    {
        switch (argChoice)
        {
        case 1:
            Disp->display("Default time 10 minutes is set");
            return autoFlushTimerAdjValue(true);
        case 2:
            Disp->display("Set time from 05-60 minutes");
            autoFlushTimer = true;
            backAndForthSetup();
            break;
        }
    }
    else if (autoFlushTimer)
    {
        return autoFlushTimerAdjValue(false);
    }

    return false;
}