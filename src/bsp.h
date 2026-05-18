#ifndef __BSP_H__
#define __BSP_H__

#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

#define     __IM     volatile const
#define     __OM     volatile 
#define     __IOM    volatile   

#define NVIC_PriorityGroup_0                                                                                           \
    ((uint32_t)0x700) /*!< 0 bits for pre-emption priority                                                             \
                           4 bits for subpriority */
#define NVIC_PriorityGroup_1                                                                                           \
    ((uint32_t)0x600) /*!< 1 bits for pre-emption priority                                                             \
                           3 bits for subpriority */
#define NVIC_PriorityGroup_2                                                                                           \
    ((uint32_t)0x500) /*!< 2 bits for pre-emption priority                                                             \
                           2 bits for subpriority */
#define NVIC_PriorityGroup_3                                                                                           \
    ((uint32_t)0x400) /*!< 3 bits for pre-emption priority                                                             \
                           1 bits for subpriority */
#define NVIC_PriorityGroup_4                                                                                           \
    ((uint32_t)0x300) /*!< 4 bits for pre-emption priority                                                             \
                           0 bits for subpriority */

#define configMAX_PRIORITIES                       5
#define configMAX_TASK_NAME_LEN                    16
#define configKERNEL_INTERRUPT_PRIORITY         0xf0//
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     0x50//


#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )

#define portNVIC_SYSTICK_CLK_BIT              ( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT       ( 1UL << 16UL )
#define portNVIC_SYSTICK_CLK_BIT_CONFIG    ( portNVIC_SYSTICK_CLK_BIT )

#define portNVIC_INT_CTRL_REG     ( *( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT    ( 1UL << 28UL )
#define portYIELD()                                     \
    {                                                   \
        /* Set a PendSV to request a context switch. */ \
        portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
                                                        \
        /* Barriers are normally not required but do ensure the code is completely \
         * within the specified behaviour for the architecture. */ \
        __asm volatile ( "dsb" ::: "memory" );                     \
        __asm volatile ( "isb" );                                  \
    }
#define portEND_SWITCHING_ISR( xSwitchRequired ) \
    do                                           \
    {                                            \
        if( xSwitchRequired != pdFALSE )         \
        {                                        \
            portYIELD();                         \
        }                                        \
    } while( 0 )
#define portYIELD_FROM_ISR( x )    portEND_SWITCHING_ISR( x )
#define portINITIAL_XPSR                      ( 0x01000000 )
#define portINITIAL_EXC_RETURN                ( 0xfffffffd )
#define portSTART_ADDRESS_MASK                ( ( uint32_t ) 0xfffffffeUL )
#define portMAX_DELAY              ( uint32_t ) 0xffffffffUL

#define pdFALSE                                  ( ( uint32_t ) 0 )
#define pdTRUE                                   ( ( uint32_t ) 1 )
#define tskIDLE_PRIORITY    ( ( unsigned long ) 0U )
typedef void (* TaskFunction_t)( void * arg );
#define listLIST_IS_EMPTY( pxList )                       ( ( ( pxList )->uxNumberOfItems == ( unsigned long ) 0 ) ? pdTRUE : pdFALSE )



typedef struct
{
  __IM  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
  __IOM uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
  __IOM uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
  __IOM uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
  __IOM uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
  __IOM uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
  __IOM uint8_t  SHPR[12U];              /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IOM uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
  __IOM uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
  __IOM uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
  __IOM uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
  __IOM uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
  __IOM uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
  __IOM uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
  __IM  uint32_t ID_PFR[2U];             /*!< Offset: 0x040 (R/ )  Processor Feature Register */
  __IM  uint32_t ID_DFR;                 /*!< Offset: 0x048 (R/ )  Debug Feature Register */
  __IM  uint32_t ID_AFR;                 /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
  __IM  uint32_t ID_MMFR[4U];            /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
  __IM  uint32_t ID_ISAR[5U];            /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
        uint32_t RESERVED0[5U];
  __IOM uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
        uint32_t RESERVED3[93U];
  __OM  uint32_t STIR;                   /*!< Offset: 0x200 ( /W)  Software Triggered Interrupt Register */
} SCB_Type;

#define SCS_BASE            (0xE000E000UL)
#define SysTick_BASE        (SCS_BASE +  0x0010UL)
#define NVIC_BASE           (SCS_BASE +  0x0100UL)
#define SCB_BASE            (SCS_BASE +  0x0D00UL) 


#define SCB                 ((SCB_Type       *)     SCB_BASE      )


#define AIRCR_VECTKEY_MASK ((uint32_t)0x05FA0000)

struct xLIST;
struct xLIST_ITEM
{
    uint32_t xItemValue; 
    struct xLIST_ITEM *  pxNext;
    struct xLIST_ITEM *  pxPrevious;
    void * pvOwner;
    struct xLIST *  pxContainer;
};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM
{
	uint32_t xItemValue;
	struct xLIST_ITEM * pxNext;
	struct xLIST_ITEM * pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;
//typedef struct xLIST_ITEM      MiniListItem_t;


typedef struct xLIST
{
    uint32_t uxNumberOfItems;
    ListItem_t * pxIndex;
    MiniListItem_t xListEnd;
} List_t;

typedef struct tskTaskControlBlock       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile uint32_t * pxTopOfStack; /**< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

    #if ( portUSING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xMPUSettings; /**< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
    #endif

    #if ( configUSE_CORE_AFFINITY == 1 ) && ( configNUMBER_OF_CORES > 1 )
        UBaseType_t uxCoreAffinityMask; /**< Used to link the task to certain cores.  UBaseType_t must have greater than or equal to the number of bits as configNUMBER_OF_CORES. */
    #endif

    ListItem_t xStateListItem;                  /**< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
    ListItem_t xEventListItem;                  /**< Used to reference a task from an event list. */
    unsigned long uxPriority;                     /**< The priority of the task.  0 is the lowest priority. */
    uint32_t * pxStack;                      /**< Points to the start of the stack. */
    #if ( configNUMBER_OF_CORES > 1 )
        volatile BaseType_t xTaskRunState;      /**< Used to identify the core the task is running on, if the task is running. Otherwise, identifies the task's state - not running or yielding. */
        UBaseType_t uxTaskAttributes;           /**< Task's attributes - currently used to identify the idle tasks. */
    #endif
    char pcTaskName[ configMAX_TASK_NAME_LEN ]; /**< Descriptive name given to the task when created.  Facilitates debugging only. */

    #if ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        BaseType_t xPreemptionDisable; /**< Used to prevent the task from being preempted. */
    #endif

    #if ( ( portSTACK_GROWTH > 0 ) || ( configRECORD_STACK_HIGH_ADDRESS == 1 ) )
        StackType_t * pxEndOfStack; /**< Points to the highest valid address for the stack. */
    #endif

    #if ( portCRITICAL_NESTING_IN_TCB == 1 )
        UBaseType_t uxCriticalNesting; /**< Holds the critical section nesting depth for ports that do not maintain their own count in the port layer. */
    #endif

    #if ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxTCBNumber;  /**< Stores a number that increments each time a TCB is created.  It allows debuggers to determine when a task has been deleted and then recreated. */
        UBaseType_t uxTaskNumber; /**< Stores a number specifically for use by third party trace code. */
    #endif

    #if ( configUSE_MUTEXES == 1 )
        UBaseType_t uxBasePriority; /**< The priority last assigned to the task - used by the priority inheritance mechanism. */
        UBaseType_t uxMutexesHeld;
    #endif

    #if ( configUSE_APPLICATION_TASK_TAG == 1 )
        TaskHookFunction_t pxTaskTag;
    #endif

    #if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
        void * pvThreadLocalStoragePointers[ configNUM_THREAD_LOCAL_STORAGE_POINTERS ];
    #endif

    #if ( configGENERATE_RUN_TIME_STATS == 1 )
        configRUN_TIME_COUNTER_TYPE ulRunTimeCounter; /**< Stores the amount of time the task has spent in the Running state. */
    #endif

    #if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
        configTLS_BLOCK_TYPE xTLSBlock; /**< Memory block used as Thread Local Storage (TLS) Block for the task. */
    #endif

    #if ( configUSE_TASK_NOTIFICATIONS == 1 )
        volatile uint32_t ulNotifiedValue[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        volatile uint8_t ucNotifyState[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #endif

    /* See the comments in FreeRTOS.h with the definition of
     * tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE. */
    #if ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the task is a statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #if ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDelayAborted;
    #endif

    #if ( configUSE_POSIX_ERRNO == 1 )
        int iTaskErrno;
    #endif
} tskTCB;

typedef tskTCB TCB_t;


#define SET_INTERRUPT_MASK_FROM_ISR()         ulRaiseBASEPRI()
#define CLEAR_INTERRUPT_MASK_FROM_ISR( x )    SetBASEPRI( x )
#define DISABLE_INTERRUPTS()                  RaiseBASEPRI()
#define ENABLE_INTERRUPTS()                   SetBASEPRI( 0 )
#define ENTER_CRITICAL()                      EnterCritical()
#define EXIT_CRITICAL()                       ExitCritical()


#endif




