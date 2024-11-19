#include "menu_02.h"
#include "controls.h"
#include "masterLog.h"
#include "rofsCore.h"


#define MAX_TIME 60
#define MAX_PROD_TIME 150
#define DEFAULT_PROD_TIME 50
#define DEFAULT_TIME 10
#define MIN_TIME 05
#define DIFF_TIME 05

autoFlushTimers flushTimer;

// // Helper function to check if the menu label matches the target menu label
// bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetCountTime, uint8_t childMenu = 0)
// {
//     return strcmp(menu->Items[childMenu].label, targetCountTime.Items[childMenu].label) == 0;
// }

bool autoFlushTimerAdjValue(bool *timerDefaultMode)
{

    uint8_t minCountTime = flushTimer.selectTimer == PRODUCTION_TIMER ? DEFAULT_PROD_TIME : MIN_TIME;
    uint8_t maxCountTime = flushTimer.selectTimer == PRODUCTION_TIMER ? MAX_PROD_TIME : MAX_TIME;

    static uint8_t timerVal = minCountTime;

    if (ButtonD.state() && timerVal > minCountTime)
    {
        timerVal -= DIFF_TIME;
    }
    else if (ButtonU.state() && timerVal < maxCountTime)
    {
        timerVal += DIFF_TIME;
    }

    if (ButtonE.state())
    {
        if (flushTimer.selectTimer == STARTUP_TIMER)
        {
            flushTimer.startUpTimer = timerVal;
        }
        else if (flushTimer.selectTimer == PRODUCTION_TIMER)
        {
            flushTimer.productionTimer = timerVal;
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
        *timerDefaultMode = false;
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
        case 4:  flushTimer.selectTimer = PRODUCTION_TIMER; return true;
        case 5:  flushTimer.selectTimer = SUBSEQUENT_TIMER; return true;
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
            flushTimer.selectTimer == STARTUP_TIMER ?
            flushTimer.startUpTimer = DEFAULT_TIME : flushTimer.subsequentTimer = DEFAULT_TIME ;
            flushTimer.selectTimer == NO_TIMER;
            return true;

        case 2:
            Disp->display("Set time from 05-60 minutes");
            autoFlushTimer = true;
            backAndForthSetup();
            break;
        }
    }
    else if (autoFlushTimer)
    {
        return autoFlushTimerAdjValue(&autoFlushTimer);
    }

    return false;
}

bool productionTimerLoop(uint32_t argChoice, MenuItem *argMenu)
{
    static bool productionTimer = false;

    if (!productionTimer && ButtonE.state())
    {
        switch (argChoice)
        {
        case 1:
            Disp->display("Default time 50 minutes is set");
            flushTimer.productionTimer = DEFAULT_PROD_TIME;
            flushTimer.selectTimer == NO_TIMER;
            return true;
      
        case 2:
            Disp->display("Set time from 50-150 minutes");
            productionTimer = true;
            backAndForthSetup();
            break;
        }
    }
    else if (productionTimer)
    {
        return autoFlushTimerAdjValue(&productionTimer);
    }

    return false;
}

void cycleCount()
{
    uint8_t flushCycleCount = 0;
    const char *cycle[] = {"First", "Second", "Third", "Fourth", "Fifth", "Subsequent (5+)"};
    flushTimer.cycleCount > 6 ? flushCycleCount = 0 : flushCycleCount;
    uint32_t timeLeft = HAL_GetTick() - (flushTimer.cycleCount == 1 ? flushTimer.startUpTimer : flushTimer.subsequentTimer);
    menu.Display_Log("Purifying - %s Auto Flush Cycle - Flush time left: ", cycle[flushCycleCount], timeLeft);
}

void autoFlushST4(void)
{
    Flags.update();
    static uint8_t curCycleCount = 0;
    if (!Flags.flagStatus(Status, AUTO_FLUSH) && Flags.flagStatus(Settings, AUTO_FLUSH))
    {
        curCycleCount++;
    }
    if (Flags.flagStatus(Status, LEVEL_SENSOR) && curCycleCount > 0)
    {
        curCycleCount = 0;
    }
	if ((Flags.flagStatus(Status, LEVEL_SENSOR) && Flags.timeElapsed(AUTO_FLUSH, AUTO_FLUSH_FINISH_5M))
		|| Flags.timeElapsed(AUTO_FLUSH, AUTO_FLUSH_TIMEOUT) && Flags.flagStatus(Status, AUTO_FLUSH)
		|| !Flags.flagStatus(Status, LEVEL_SENSOR) && Flags.flagStatus(Status, AUTO_FLUSH_PAUSE) 
		&& Flags.timeElapsed(AUTO_FLUSH, flushTimer.pausedTime) && Flags.flagStatus(Status, AUTO_FLUSH))
    {
        invokeST4(false);
        Flags.setFlag(Status, AUTO_FLUSH_PAUSE, false);
        flushTimer.pausedTime = 0;
        DEBUG_RTT(LOG_WARNING, "***AUTO_FLUSH_CYCLE_STOPPED***");
    }
    else if (Flags.flagStatus(Status, PURE_CYCLE) && Flags.flagStatus(Settings, AUTO_FLUSH) && !Flags.flagStatus(Status, LEVEL_SENSOR))
    {
        cycleCount();
        invokeST4(true);
    }
}