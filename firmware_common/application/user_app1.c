/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  static u16 au16NotesRight[] =    {F5, F5, F5, F5, F5, E5, D5, E5, F5, G5, A5, A5, A5, A5, A5, G5, F5, G5, A5, A5S, C6, F5, F5, D6, C6, A5S, A5, G5, F5, NO, NO};
  static u16 au16DurationRight[] = {QN, QN, HN, EN, EN, EN, EN, EN, EN, QN, QN, QN, HN, EN, EN, EN, EN, EN, EN, QN,  HN, HN, EN, EN, EN, EN,  QN, QN, HN, HN, FN};
  static u16 au16SilenceDurationRight[] = {RT, RT, HT, RT, RT, RT, RT, RT, RT, RT, RT, RT, HT, RT, RT, RT, RT, RT, RT, RT,  RT, HT, RT, RT, RT, RT,  RT, RT, RT, HT, HT};

  static u8 u8CurrentIndexRight = 0;
  static u32 u32CurrentTimeElapsedRight = 0;
  static u16 u16CurrentNoteDurationRight = 0;
  static bool bNoteActiveRight = TRUE;
  
  static u16 au16NotesLeft[] =                  {F4, F4, A4, A4, D4, D4, F4, F4, A3S, A3S, D4, D4, C4, C4, E4, E4};
  static u16 au16DurationLeft[] =               {EN, EN, EN, EN, EN, EN, EN, EN, EN,  EN,  EN, EN, EN, EN, EN, EN};
  static u16 au16SilenceDurationLeft[] =        {RT, RT, RT, RT, RT, RT, RT, RT, RT,  RT,  RT, RT, RT, RT, RT, RT};

  static u8 u8CurrentIndexLeft = 0;
  static u32 u32CurrentTimeElapsedLeft = 0;
  static u16 u16CurrentNoteDurationLeft = 0;
  static bool bNoteActiveLeft = TRUE;
 
  if (u8CurrentIndexLeft < 16) {
    if (u32CurrentTimeElapsedLeft >= u16CurrentNoteDurationLeft) {
      u32CurrentTimeElapsedLeft = 0;
      if (bNoteActiveLeft == TRUE) {
        PWMAudioSetFrequency(BUZZER1, au16NotesLeft[u8CurrentIndexLeft]);
        PWMAudioOn(BUZZER1);
        u16CurrentNoteDurationLeft = au16DurationLeft[u8CurrentIndexLeft];
        bNoteActiveLeft = FALSE;
      }
      else if (bNoteActiveLeft == FALSE) {
        PWMAudioOff(BUZZER1);
        u16CurrentNoteDurationLeft = au16SilenceDurationLeft[u8CurrentIndexLeft];
        u8CurrentIndexLeft++;
        bNoteActiveLeft = TRUE;
      }
    }
    u32CurrentTimeElapsedLeft++;
    if (u8CurrentIndexLeft == 16) {
      u8CurrentIndexLeft = 0;
    }
  }
  
  if (u8CurrentIndexRight < 31) {
    if (u32CurrentTimeElapsedRight >= u16CurrentNoteDurationRight) {
      u32CurrentTimeElapsedRight = 0;
      if (bNoteActiveRight == TRUE) {
        PWMAudioSetFrequency(BUZZER2, au16NotesRight[u8CurrentIndexRight]);
        PWMAudioOn(BUZZER2);
        u16CurrentNoteDurationRight = au16DurationRight[u8CurrentIndexRight];
        bNoteActiveRight = FALSE;
      }
      else if (bNoteActiveRight == FALSE) {
        PWMAudioOff(BUZZER2);
        u16CurrentNoteDurationRight = au16SilenceDurationRight[u8CurrentIndexRight];
        u8CurrentIndexRight++;
        bNoteActiveRight = TRUE;
      }
    }
    u32CurrentTimeElapsedRight++;
    if (u8CurrentIndexRight == 31) {
      u8CurrentIndexRight = 0;
    }
  }
 
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
