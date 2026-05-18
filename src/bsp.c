#include "stdint.h"
#include "stdlib.h"

#include "bsp.h"

volatile uint32_t VectorTable[82] __attribute__((section(".RAMVectorTable")));


__attribute__( ( used ) )  TCB_t * volatile pxCurrentTCB = NULL;
//__attribute__( ( used ) )  TCB_t * volatile pxCurrentTCBs[ 2 ];
//#define pxCurrentTCB    xTaskGetCurrentTaskHandle()

unsigned long uxCriticalNesting = 0xaaaaaaaa;
volatile unsigned long uxCurrentNumberOfTasks = ( unsigned long ) 0U;
volatile unsigned long xYieldPendings[ 1 ] = { pdFALSE };
unsigned long uxTaskNumber = ( unsigned long ) 0U;
volatile unsigned long uxTopReadyPriority = 4;


List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
List_t xDelayedTaskList1;
List_t xDelayedTaskList2;
List_t * volatile pxDelayedTaskList;
List_t * volatile pxOverflowDelayedTaskList;
List_t xPendingReadyList; 


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);

void startFirstTask(void) __attribute__( ( naked ) );
void SVC_Handler(void) __attribute__( ( naked ) );
void PendSV_Handler( void ) __attribute__( ( naked ) );


void SystemInit(void);


__attribute__( ( used ) )  void vTaskSwitchContext( void );





inline __attribute__( ( always_inline ) ) void SetBASEPRI( uint32_t ulNewMaskValue )
{
    __asm volatile
    (
        "   msr basepri, %0 " ::"r" ( ulNewMaskValue ) : "memory"
    );
}

inline __attribute__( ( always_inline ) )  void RaiseBASEPRI( void )
{
    uint32_t ulNewBASEPRI;

    __asm volatile
    (
        "   mov %0, %1                                              \n" \
        "   msr basepri, %0                                         \n" \
        "   isb                                                     \n" \
        "   dsb                                                     \n" \
        : "=r" ( ulNewBASEPRI ) : "i" ( configMAX_SYSCALL_INTERRUPT_PRIORITY ) : "memory"
    );
}

inline __attribute__( ( always_inline ) )   uint32_t ulRaiseBASEPRI( void )
{
    uint32_t ulOriginalBASEPRI, ulNewBASEPRI;

    __asm volatile
    (
        "   mrs %0, basepri                                         \n" \
        "   mov %1, %2                                              \n" \
        "   msr basepri, %1                                         \n" \
        "   isb                                                     \n" \
        "   dsb                                                     \n" \
        : "=r" ( ulOriginalBASEPRI ), "=r" ( ulNewBASEPRI ) : "i" ( configMAX_SYSCALL_INTERRUPT_PRIORITY ) : "memory"
    );
    return ulOriginalBASEPRI;
}

void EnterCritical( void )
{
    DISABLE_INTERRUPTS();
    uxCriticalNesting++;
}

void ExitCritical( void )
{
    uxCriticalNesting--;

    if( uxCriticalNesting == 0 )
    {
        ENABLE_INTERRUPTS();
    }
}


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}
void SysTick_Handler(void)
{
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void PendSV_Handler(void)
{
    __asm volatile
    (
        "   mrs r0, psp                         \n"
        "   isb                                 \n"
        "                                       \n"
        "   ldr r3, =pxCurrentTCB               \n"
        "   ldr r2, [r3]                        \n"
        "                                       \n"
        "   tst r14, #0x10                      \n" 
        "   it eq                               \n"
        "   vstmdbeq r0!, {s16-s31}             \n"
        "                                       \n"
        "   stmdb r0!, {r4-r11, r14}            \n" 
        "   str r0, [r2]                        \n"
        "                                       \n"
        "   stmdb sp!, {r0, r3}                 \n"
        "   mov r0, %0                          \n"
        "   msr basepri, r0                     \n"
        "   dsb                                 \n"
        "   isb                                 \n"
        "   bl vTaskSwitchContext               \n"
        "   mov r0, #0                          \n"
        "   msr basepri, r0                     \n"
        "   ldmia sp!, {r0, r3}                 \n"
        "                                       \n"
        "   ldr r1, [r3]                        \n" 
        "   ldr r0, [r1]                        \n"
        "                                       \n"
        "   ldmia r0!, {r4-r11, r14}            \n" 
        "                                       \n"
        "   tst r14, #0x10                      \n" 
        "   it eq                               \n"
        "   vldmiaeq r0!, {s16-s31}             \n"
        "                                       \n"
        "   msr psp, r0                         \n"
        "   isb                                 \n"
        "                                       \n"
        #ifdef WORKAROUND_PMU_CM001 /* XMC4000 specific errata workaround. */
            #if WORKAROUND_PMU_CM001 == 1
                "           push { r14 }                \n"
                "           pop { pc }                  \n"
            #endif
        #endif
        "                                       \n"
        "   bx r14                              \n"
        "                                       \n"
        "   .ltorg                              \n"
        ::"i" ( configMAX_SYSCALL_INTERRUPT_PRIORITY )
    );	
}

void SVC_Handler(void)
{
    __asm volatile (
        "   ldr r3, =pxCurrentTCB           \n"
        "   ldr r1, [r3]                    \n"
        "   ldr r0, [r1]                    \n"
        "   ldmia r0!, {r4-r11, r14}        \n"
        "   msr psp, r0                     \n"
        "   isb                             \n"
        "   mov r0, #0                      \n"
        "   msr basepri, r0                 \n"
        "   bx r14                          \n"
        "                                   \n"
        "   .ltorg                          \n"
        );	
}

void DebugMon_Handler(void)
{
}

void SystemInit(void)
{
	
	
}



void startFirstTask(void)
{
    __asm volatile (
        " ldr r0, =0xE000ED08   \n"
        " ldr r0, [r0]          \n"
        " ldr r0, [r0]          \n"
        " msr msp, r0           \n"
        " mov r0, #0            \n"
        " msr control, r0       \n"
        " cpsie i               \n"
        " cpsie f               \n"
        " dsb                   \n"
        " isb                   \n"
        " svc 0                 \n" 
        " nop                   \n"
        " .ltorg                \n"
        );	
}

#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )                                        \
    do {                                                                                       \
        List_t * const pxConstList = ( pxList );                                               \
        /* Increment the index to the next item and return the item, ensuring */               \
        /* we don't return the marker used at the end of the list.  */                         \
        ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;                           \
        if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) ) \
        {                                                                                      \
            ( pxConstList )->pxIndex = ( pxConstList )->xListEnd.pxNext;                       \
        }                                                                                      \
        ( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;                                         \
    } while( 0 )

void vTaskSwitchContext( void )
{
	unsigned long uxTopPriority = uxTopReadyPriority;
												
	while( listLIST_IS_EMPTY( &( pxReadyTasksLists[ uxTopPriority ] ) ) != pdFALSE )
	{                
		--uxTopPriority;    
	}  
	listGET_OWNER_OF_NEXT_ENTRY( pxCurrentTCB, &( pxReadyTasksLists[ uxTopPriority ] ) ); 
	uxTopReadyPriority = uxTopPriority;     	
}

#define taskStackSize_Key (8*50)
uint32_t stackKey[taskStackSize_Key];
TCB_t taskKey;
int a=0;
int b=0;
void task_key(void *p_arg)
{
	a=0;
	while(1)
	{
		a++;
	}
}

#define taskStackSize_Led (8*50)
uint32_t stackLed[taskStackSize_Led];
TCB_t taskLed;
void task_led(void *p_arg)
{
	b=0;
	while(1)
	{
		b++;
	}	
}



uint32_t * initialiseStack( uint32_t * pxTopOfStack,TaskFunction_t pxCode )
{
    pxTopOfStack--;

    *pxTopOfStack = portINITIAL_XPSR;                                    /* xPSR */
    pxTopOfStack--;
    *pxTopOfStack = ( ( uint32_t ) pxCode ) & portSTART_ADDRESS_MASK; /* PC */
    pxTopOfStack--;
    *pxTopOfStack = ( uint32_t ) 0;             /* LR */

    /* Save code space by skipping register initialisation. */
    pxTopOfStack -= 5;                            /* R12, R3, R2 and R1. */
    *pxTopOfStack = ( uint32_t ) 0; /* R0 */

    /* A save method is being used that requires each task to maintain its
     * own exec return value. */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_EXC_RETURN;

    pxTopOfStack -= 8; /* R11, R10, R9, R8, R7, R6, R5 and R4. */

    return pxTopOfStack;
}

void vListInitialise( List_t * const pxList )
{
    pxList->pxIndex = ( ListItem_t * ) &( pxList->xListEnd );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->xListEnd.xItemValue = portMAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->xListEnd.pxNext = ( ListItem_t * ) &( pxList->xListEnd );
    pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );

    pxList->uxNumberOfItems = ( unsigned long ) 0U;
}



void listINSERT_END(List_t *  pxList, ListItem_t * pxNewListItem ) 
{
	ListItem_t * const pxIndex = ( pxList )->pxIndex; 
	( pxNewListItem )->pxNext = pxIndex; 
	( pxNewListItem )->pxPrevious = pxIndex->pxPrevious;

	pxIndex->pxPrevious->pxNext = ( pxNewListItem );
	pxIndex->pxPrevious = ( pxNewListItem ); 

	( pxNewListItem )->pxContainer = ( pxList );

	( ( pxList )->uxNumberOfItems ) = ( unsigned long ) ( ( ( pxList )->uxNumberOfItems ) + 1U );
}

void createTask(void)
{
	uint32_t  * pxTopOfStack;
    unsigned long uxPriority;	
	
	ENTER_CRITICAL();
	


    for( uxPriority = ( unsigned long ) 0U; uxPriority < ( unsigned long ) configMAX_PRIORITIES; uxPriority++ )
    {
        vListInitialise( &( pxReadyTasksLists[ uxPriority ] ) );
    }

    vListInitialise( &xDelayedTaskList1 );
    vListInitialise( &xDelayedTaskList2 );
    vListInitialise( &xPendingReadyList );	
	
	
	
	( void ) memset( ( void * ) &taskKey, 0x00, sizeof( TCB_t ) );
	( void ) memset( ( void * ) stackKey, 0x00, taskStackSize_Key );	
	taskKey.pxStack = stackKey;
	taskKey.uxPriority = 4;

	pxTopOfStack = &( taskKey.pxStack[ taskStackSize_Key - ( uint32_t ) 1 ] );
	taskKey.pxTopOfStack = initialiseStack(pxTopOfStack,(TaskFunction_t )task_key);
	
	taskKey.xStateListItem.pxContainer = NULL;
	taskKey.xStateListItem.pvOwner = &taskKey;
	
	taskKey.xEventListItem.pxContainer = NULL;
	taskKey.xEventListItem.xItemValue = 1;	
	taskKey.xEventListItem.pvOwner = &taskKey;	
	
	
	uxCurrentNumberOfTasks = ( unsigned long ) ( uxCurrentNumberOfTasks + 1U );
	uxTaskNumber++;

	
	pxCurrentTCB = &taskKey;
	
	listINSERT_END(&( pxReadyTasksLists[ ( &taskKey )->uxPriority ] ), &( ( &taskKey )->xStateListItem ));
	
	( void ) memset( ( void * ) &taskLed, 0x00, sizeof( TCB_t ) );
	( void ) memset( ( void * ) stackLed, 0x00, taskStackSize_Led );	
	taskLed.pxStack = stackLed;
	taskLed.uxPriority = 4;

	pxTopOfStack = &( taskLed.pxStack[ taskStackSize_Led - ( uint32_t ) 1 ] );
	taskLed.pxTopOfStack = initialiseStack(pxTopOfStack,(TaskFunction_t )task_led);
	
	taskLed.xStateListItem.pxContainer = NULL;
	taskLed.xStateListItem.pvOwner = &taskLed;
	
	taskLed.xEventListItem.pxContainer = NULL;
	taskLed.xEventListItem.xItemValue = 1;	
	taskLed.xEventListItem.pvOwner = &taskLed;		
	
	uxCurrentNumberOfTasks = ( unsigned long ) ( uxCurrentNumberOfTasks + 1U );
	uxTaskNumber++;	
	
	listINSERT_END(&( pxReadyTasksLists[ ( &taskLed )->uxPriority ] ), &( ( &taskLed )->xStateListItem ));	
	
	
	
	EXIT_CRITICAL();	
}

int main(void)
{
	uint8_t i=0;  

	//  for(i = 0; i < 48; i++)
	for(i = 0; i < 82; i++)	
	{
		VectorTable[i] = *(volatile uint32_t*)((uint32_t)0x08002800 + (i<<2));
	}

	(*((volatile uint32_t *)0xE000ED08UL)) = (uint32_t)VectorTable;	
	
	SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup_4;
	
	
	uxCriticalNesting = 0;
	
	
	
	createTask();
	
    /* Stop and clear the SysTick. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;
    portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_LOAD_REG = ( ( ( unsigned long ) 4000000 ) / 1000 ) - 1UL;
    portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT_CONFIG | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );	
	
	
	startFirstTask();
	
    for( ; ; )
    {
        /* Should not reach here. */
    }	
}








