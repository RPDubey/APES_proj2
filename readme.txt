changes made

1) in FreeRTOSConfig.h
 	#define configSUPPORT_STATIC_ALLOCATION			0 from 1
 	#define configUSE_IDLE_HOOK						0 
	#define configUSE_TICK_HOOK						0
	#define configUSE_MALLOC_FAILED_HOOK			0
	#define configCHECK_FOR_STACK_OVERFLOW			0 form 2
 	
 	
 2)commented out entire mpu_wrappers.c, ie basically delete the file
 
 3)added some call back functions in callbacks.c	
 
 4)startup_ccs.c
 	// IntDefaultHandler,                   // SVCall handler

    vPortSVCHandler,                        // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    //IntDefaultHandler,                    // The PendSV handler
    //IntDefaultHandler,                    // The SysTick handler
    xPortPendSVHandler,                     // The PendSV handler
    xPortSysTickHandler,                    // The SysTick handler
 
 
 //*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************
extern void xPortPendSVHandler(void);
extern void vPortSVCHandler(void);
extern void xPortSysTickHandler(void);