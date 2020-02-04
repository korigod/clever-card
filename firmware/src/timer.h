#ifndef TIMER_H_INC
#define TIMER_H_INC

#define TIMER1_PRESCALER timerPrescale16
#define TIMER1_PRESCALER_VALUE (1 << TIMER1_PRESCALER)

const uint16_t timerMinTicksToWait;

void initializeTimer(void);

void enableAndTriggerTimerInterrupt(void (*timerCallback)(void));

void disableTimerInterrupt(void);

void setTimerToWaitTicks(uint16_t ticks);

void TIMER1_IRQHandler(void);

#endif /* TIMER_H_INC */
