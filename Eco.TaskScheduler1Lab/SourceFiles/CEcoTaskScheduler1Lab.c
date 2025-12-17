/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTaskScheduler1Lab_C761620F
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoTaskScheduler1Lab_C761620F
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoTaskScheduler1Lab.h"
#include "CEcoTask1Lab.h"

/* Выделяем память под один экземпляр */
CEcoTaskScheduler1Lab_C761620F g_xCEcoTaskScheduler1Lab_C761620F = {0};

/* Резервируем область под статические задачи */
#define MAX_TASK_POOL_SIZE   10
CEcoTask1Lab_C761620F g_taskPoolArray_C761620F[MAX_TASK_POOL_SIZE] = {0};

/* Резервируем область под стеки статических задач */
#define TOTAL_STACK_SIZE   4096 * MAX_TASK_POOL_SIZE
uint64_t g_taskStackMemory_C761620F[TOTAL_STACK_SIZE] = {0};

/* Контекст */
uint64_t * volatile g_activeTaskControlBlock_C761620F = 0;

uint64_t g_activeTaskSlot_C761620F = 0;

// extern IEcoVFB1* g_pIVFB;
extern IEcoTask1VTbl g_x81A466F4C27540B1B33D0661E5470F1BVTbl_C761620F;

/*
 *
 * <сводка>
 *   Функция TaskSwitchContext
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */

/*__attribute__((naked))*/ void CEcoTaskScheduler1Lab_C761620F_TaskSwitchContext( void ) {
    uint64_t nextTaskSlot = g_activeTaskSlot_C761620F;
    const uint64_t MAX_ROUND_ROBIN_SLOTS = 2;
    CEcoTask1Lab_C761620F* nextTask = 0;
    
    nextTaskSlot = nextTaskSlot + 1;
    if (nextTaskSlot > (MAX_ROUND_ROBIN_SLOTS - 1)) {
        nextTaskSlot = 0;
    }
    g_activeTaskSlot_C761620F = nextTaskSlot;

   // g_xCEcoTaskScheduler1Lab_C761620F.m_pIArmTimer->pVTbl->Reset(g_xCEcoTaskScheduler1Lab_C761620F.m_pIArmTimer);
    nextTask = &g_taskPoolArray_C761620F[nextTaskSlot];
    if (nextTask->pfunc != 0) {
        nextTask->pfunc();
    }
}

/*
 *
 * <сводка>
 *   Функция TimerHandler
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
void CEcoTaskScheduler1Lab_C761620F_TimerHandler(void) {

    /* Сохранение контекста текущей задачи */
    //__asm volatile (
    //"STP 	X0, X1, [SP, #-0x10]! \n"
    //"STP 	X2, X3, [SP, #-0x10]! \n"
    //"STP 	X4, X5, [SP, #-0x10]! \n"
    //"STP 	X6, X7, [SP, #-0x10]! \n"
    //"STP 	X8, X9, [SP, #-0x10]! \n"
    //"STP 	X10, X11, [SP, #-0x10]!\n"
    //"STP 	X12, X13, [SP, #-0x10]!\n"
    //"STP 	X14, X15, [SP, #-0x10]!\n"
    //"STP 	X16, X17, [SP, #-0x10]!\n"
    //"STP 	X18, X19, [SP, #-0x10]!\n"
    //"STP 	X20, X21, [SP, #-0x10]!\n"
    //"STP 	X22, X23, [SP, #-0x10]!\n"
    //"STP 	X24, X25, [SP, #-0x10]!\n"
    //"STP 	X26, X27, [SP, #-0x10]!\n"
    //"STP 	X28, X29, [SP, #-0x10]!\n"
    //"STP 	X30, XZR, [SP, #-0x10]!\n"
    //"MRS		X3, SPSR_EL1\n"
    //"MRS		X2, ELR_EL1\n"
    //"STP 	X2, X3, [SP, #-0x10]!\n"
    //"LDR 	X0, =g_pxCurrentTCB_C761620F \n"
    //"LDR 	X1, [X0] \n"
    //"MOV 	X0, SP \n"
    //"STR 	X0, [X1] \n"
    //);
    /* Переключение контекста задач */
    __asm volatile (
    "BL 	CEcoTaskScheduler1Lab_C761620F_TaskSwitchContext \n"
    );

    /* Востановление контекста следующей задачи */
    //__asm volatile ( "LDR		X0, =g_pxCurrentTCB_C761620F \n"
    //"LDR		X1, [X0] \n"
    //"LDR		X0, [X1] \n"
    //"MOV		SP, X0 \n"
    //"LDP 	X2, X3, [SP], #0x10 \n"
    //"MSR		SPSR_EL1, X3 \n"
    //"MSR		ELR_EL1, X2 \n"
    //"LDP 	X30, XZR, [SP], #0x10 \n"
    //"LDP 	X28, X29, [SP], #0x10 \n"
    //"LDP 	X26, X27, [SP], #0x10 \n"
    //"LDP 	X24, X25, [SP], #0x10 \n"
    //"LDP 	X22, X23, [SP], #0x10 \n"
    //"LDP 	X20, X21, [SP], #0x10 \n"
    //"LDP 	X18, X19, [SP], #0x10 \n"
    //"LDP 	X16, X17, [SP], #0x10 \n"
    //"LDP 	X14, X15, [SP], #0x10 \n"
    //"LDP 	X12, X13, [SP], #0x10 \n"
    //"LDP 	X10, X11, [SP], #0x10 \n"
    //"LDP 	X8, X9, [SP], #0x10 \n"
    //"LDP 	X6, X7, [SP], #0x10 \n"
    //"LDP 	X4, X5, [SP], #0x10 \n"
    //"LDP 	X2, X3, [SP], #0x10 \n"
    //"LDP 	X0, X1, [SP], #0x10 \n"
    //"ERET \n"
    //);
    
}


/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoTaskScheduler1Lab
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_QueryInterface(/* in */ IEcoTaskScheduler1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoTaskScheduler1) ) {
        *ppv = &pCMe->m_pVTblIScheduler;
        pCMe->m_pVTblIScheduler->AddRef((IEcoTaskScheduler1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIScheduler;
        pCMe->m_pVTblIScheduler->AddRef((IEcoTaskScheduler1*)pCMe);
    }
    else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoTaskScheduler1Lab
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_AddRef(/* in */ IEcoTaskScheduler1Ptr_t me) {
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoTaskScheduler1Lab
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Release(/* in */ IEcoTaskScheduler1Ptr_t me) {
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoTaskScheduler1Lab_C761620F((IEcoTaskScheduler1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Init(/*in*/IEcoTaskScheduler1Ptr_t me, /*in*/ IEcoInterfaceBus1Ptr_t pIBus) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 || pIBus == 0) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция InitWith
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_InitWith(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ IEcoInterfaceBus1Ptr_t pIBus, /*in*/ voidptr_t heapStartAddress, /*in*/ uint32_t size) {
    CEcoTaskScheduler1Lab_C761620F* schedulerInstance = (CEcoTaskScheduler1Lab_C761620F*)me;
    int16_t operationResult = -1;
    const uint32_t TIMER_INTERVAL_MICROSECONDS = 1000000;
    IEcoTimer1Ptr_t timerInterface = 0;

    /* Проверка указателей */
    if (me == 0 || pIBus == 0) {
        return -1;
    }

    /* Инициализация данных */
    schedulerInstance->m_pTaskList = g_taskPoolArray_C761620F;
    schedulerInstance->m_pIBus = pIBus;

    /* Получение интерфейса для работы с программируемым таймером */
    operationResult = schedulerInstance->m_pIBus->pVTbl->QueryComponent(
        schedulerInstance->m_pIBus, 
        &CID_EcoTimer1, 
        0, 
        &IID_IEcoTimer1, 
        (void**) &timerInterface
    );
    
    /* Проверка результата */
    if (operationResult != 0) {
        return operationResult;
    }
    
    if (timerInterface == 0) {
        return -1;
    }
    
    schedulerInstance->m_pIArmTimer = timerInterface;

    /* Установка обработчика прерывания программируемого таймера */
    schedulerInstance->m_pIArmTimer->pVTbl->set_Interval(schedulerInstance->m_pIArmTimer, TIMER_INTERVAL_MICROSECONDS);
    schedulerInstance->m_pIArmTimer->pVTbl->set_IrqHandler(
        schedulerInstance->m_pIArmTimer, 
        (voidptr_t*)CEcoTaskScheduler1Lab_C761620F_TimerHandler
    );

    return 0;
}

/*
 *
 * <сводка>
 *   Функция NewTask
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_NewTask(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ voidptr_t address, /*in*/ voidptr_t data, /*in*/ uint32_t stackSize, /* out */ IEcoTask1** ppITask) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/
    int32_t availableSlot = 0;
    int32_t remainingRegisters = 30;
    uint64_t* currentStackPos = 0;
    CEcoTask1Lab_C761620F* taskDescriptor = 0;
    const int32_t STACK_SIZE_PER_TASK = 4096;
    const int32_t TOTAL_REGISTERS = 30;

    /* Проверка указателей */
    if (me == 0 || ppITask == 0) {
        return -1;
    }

    /* Поиск свободного слота в пуле задач */
    for (availableSlot = 0; availableSlot < MAX_TASK_POOL_SIZE; availableSlot++) {
        taskDescriptor = &g_taskPoolArray_C761620F[availableSlot];
        
        if (taskDescriptor->pfunc == 0) {
            /* Инициализация дескриптора задачи */
            taskDescriptor->pfunc = address;
            taskDescriptor->m_cRef = 1;
            taskDescriptor->m_sp = (byte_t*)&g_taskStackMemory_C761620F[availableSlot * STACK_SIZE_PER_TASK];
            
            /* Настройка стека */
            currentStackPos = taskDescriptor->m_sp;
            remainingRegisters = TOTAL_REGISTERS;
            
            while (remainingRegisters > 0) {
                currentStackPos = currentStackPos - 1;
                remainingRegisters = remainingRegisters - 1;
            }
            
            *currentStackPos = (uint64_t)taskDescriptor->pfunc;
            taskDescriptor->m_pVTblITask = &g_x81A466F4C27540B1B33D0661E5470F1BVTbl_C761620F;
            *ppITask = (IEcoTask1*)taskDescriptor;
            return 0;
        }
    }
    return -1;
}

/*
 *
 * <сводка>
 *   Функция DeleteTask
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_DeleteTask(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ uint16_t taskId) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция SuspendTask
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_SuspendTask(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ uint16_t taskId) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция ResumeTask
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_ResumeTask(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ uint16_t taskId) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция RegisterInterrupt
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_RegisterInterrupt(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ uint16_t number, /*in*/ voidptr_t handlerAddress, /*in*/ int32_t flag) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция UnRegisterInterrupt
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_UnRegisterInterrupt(/*in*/ IEcoTaskScheduler1Ptr_t me, /*in*/ uint16_t number) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Start
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
/* Вспомогательная функция для поиска задачи с минимальным временем выполнения */
static int16_t FindNextTaskToExecute(CEcoTask1Lab_C761620F* taskArray, int16_t arraySize) {
    int16_t candidateIndex = -1;
    int16_t scanIndex = 0;
    uint16_t minExecutionTime = 0;
    uint16_t currentExecutionTime = 0;
    bool_t foundValidTask = 0;
    
    for (scanIndex = 0; scanIndex < arraySize; scanIndex++) {
        if (taskArray[scanIndex].pfunc != 0) {
            currentExecutionTime = taskArray[scanIndex].executionTime;
            
            if (candidateIndex == -1) {
                candidateIndex = scanIndex;
                minExecutionTime = currentExecutionTime;
                foundValidTask = 1;
            } else {
                if (currentExecutionTime < minExecutionTime) {
                    candidateIndex = scanIndex;
                    minExecutionTime = currentExecutionTime;
                }
            }
        }
    }
    
    if (foundValidTask) {
        return candidateIndex;
    }
    return -1;
}

int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Start(/*in*/ IEcoTaskScheduler1Ptr_t me) {
    CEcoTaskScheduler1Lab_C761620F* schedulerInstance = (CEcoTaskScheduler1Lab_C761620F*)me;
    int16_t taskToRun = -1;
    CEcoTask1Lab_C761620F* taskDescriptor = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

	/* Основной цикл планировщика */
	while (1) {
		taskToRun = FindNextTaskToExecute(schedulerInstance->m_pTaskList, MAX_TASK_POOL_SIZE);
		
		if (taskToRun >= 0) {
			taskDescriptor = &schedulerInstance->m_pTaskList[taskToRun];
			if (taskDescriptor->pfunc != 0) {
				taskDescriptor->pfunc();
				taskDescriptor->pfunc = 0;
			}
		}
	}

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Stop
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Stop(/*in*/ struct IEcoTaskScheduler1* me) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}



/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoTaskScheduler1Lab_C761620F(/*in*/ IEcoTaskScheduler1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;*/

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    return 0;
}

/* Create Virtual Table IEcoTaskScheduler1Lab */
IEcoTaskScheduler1VTbl g_x155C975395654F85B9AA27D5F377A79EVTbl_C761620F = {
    CEcoTaskScheduler1Lab_C761620F_QueryInterface,
    CEcoTaskScheduler1Lab_C761620F_AddRef,
    CEcoTaskScheduler1Lab_C761620F_Release,
    CEcoTaskScheduler1Lab_C761620F_Init,
    CEcoTaskScheduler1Lab_C761620F_InitWith,
    CEcoTaskScheduler1Lab_C761620F_NewTask,
    CEcoTaskScheduler1Lab_C761620F_DeleteTask,
    CEcoTaskScheduler1Lab_C761620F_SuspendTask,
    CEcoTaskScheduler1Lab_C761620F_ResumeTask,
    CEcoTaskScheduler1Lab_C761620F_RegisterInterrupt,
    CEcoTaskScheduler1Lab_C761620F_UnRegisterInterrupt,
    CEcoTaskScheduler1Lab_C761620F_Start,
    CEcoTaskScheduler1Lab_C761620F_Stop
};




/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoTaskScheduler1Lab_C761620F(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoTaskScheduler1Ptr_t* ppITaskScheduler) {
    int16_t result = -1;
    CEcoTaskScheduler1Lab_C761620F* pCMe = 0;

    /* Проверка указателей */
    if (ppITaskScheduler == 0) {
        return result;
    }

    /* Получение указателя на структуру компонента расположенной в области инициализированных данных */
    pCMe = &g_xCEcoTaskScheduler1Lab_C761620F;

    if (pCMe->m_cRef == 0) {
        /* Установка счетчика ссылок на компонент */
        pCMe->m_cRef = 1;

        /* Создание таблицы функций интерфейса IEcoTaskScheduler1 */
        pCMe->m_pVTblIScheduler = &g_x155C975395654F85B9AA27D5F377A79EVTbl_C761620F;

        result = 0;
    }

    /* Возврат указателя на интерфейс */
    *ppITaskScheduler = (IEcoTaskScheduler1*)pCMe;

    return result;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoTaskScheduler1Lab_C761620F(/* in */ IEcoTaskScheduler1Ptr_t pITaskScheduler) {
    /*CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)pITaskScheduler;*/

}
