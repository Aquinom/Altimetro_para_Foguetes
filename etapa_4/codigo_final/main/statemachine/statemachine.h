#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//====================================================
// INCLUDES EXTERNOS
//====================================================
#include <stdbool.h>

//====================================================
// TYPES
//====================================================

typedef enum
{
    PRELAUNCH = 0,
    LAUNCH,
    FLYING,
    PEAK,
    FINDME

} state_t;

//====================================================
// PUBLIC FUNCTIONS
//====================================================

void state_machine_task_start(void);

#endif /* STATEMACHINE_H */
