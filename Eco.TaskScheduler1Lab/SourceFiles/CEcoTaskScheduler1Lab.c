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

/* Forward declaration для видео интерфейса */
typedef struct IEcoVBIOS1Video* IEcoVBIOS1VideoPtr_t;
typedef struct IEcoVBIOS1VideoVTbl {
    int16_t (ECOCALLMETHOD *QueryInterface)(IEcoVBIOS1VideoPtr_t me, const void* riid, void** ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(IEcoVBIOS1VideoPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(IEcoVBIOS1VideoPtr_t me);
    void (ECOCALLMETHOD *WriteString)(IEcoVBIOS1VideoPtr_t me, uint16_t x, uint16_t y, uint16_t w, uint16_t h, byte_t attr, char_t* str, uint16_t len);
} IEcoVBIOS1VideoVTbl;
interface IEcoVBIOS1Video {
    struct IEcoVBIOS1VideoVTbl *pVTbl;
} IEcoVBIOS1Video;

/* Константы для атрибутов символов */
#define CHARACTER_ATTRIBUTE_FORE_COLOR_YELLOW 0x0E
#define CHARACTER_ATTRIBUTE_FORE_COLOR_LIGHT_GREEN 0x0A
#define CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE 0x0F
#define CHARACTER_ATTRIBUTE_FORE_COLOR_CYAN 0x0B

/* Выделяем память под один экземпляр */
CEcoTaskScheduler1Lab_C761620F g_xCEcoTaskScheduler1Lab_C761620F = {0};

/* Резервируем область под статические задачи */
#define MAX_STATIC_TASK_COUNT   10
CEcoTask1Lab_C761620F g_xCEcoTask1List_C761620F[MAX_STATIC_TASK_COUNT] = {0};

/* Резервируем область под стеки статических задач */
#define MAX_STATIC_STACK_TASK_COUNT   4096 * MAX_STATIC_TASK_COUNT
uint64_t g_xCEcoStackTask1List_C761620F[MAX_STATIC_STACK_TASK_COUNT] = {0};

/* Контекст */
uint64_t * volatile g_pxCurrentTCB_C761620F = 0;

uint64_t g_indx = 0;

/* Внешние переменные для вывода информации */
extern IEcoVBIOS1Video* g_pIVideo;

/* Счетчик выполненных задач для информативного вывода */
static uint16_t g_taskExecutionOrder = 0;

/* Вспомогательная функция для преобразования числа в строку */
static void u32_to_str_simple(uint32_t v, char* buf, int bufsize) {
    if (bufsize < 2) {
        return;
    }
    
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        /* Сохраняем цифры в обратном порядке */
        char temp[16];
        int tempIdx = 0;
        while (v > 0 && tempIdx < 15) {
            temp[tempIdx++] = (char)('0' + (v % 10));
            v /= 10;
        }
        /* Переворачиваем в правильном порядке */
        while (tempIdx > 0 && i < bufsize - 1) {
            buf[i++] = temp[--tempIdx];
        }
    }
    buf[i] = '\0';
}

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

        //if (g_pxCurrentTCB_C761620F == (uint64_t*)&g_xCEcoTask1List_C761620F[g_indx] && g_xCEcoTask1List_C761620F[g_indx].pfunc != 0) {
            g_indx++;
            if (g_indx > 1) {
                g_indx = 0;
            }

          //  g_pxCurrentTCB_C761620F = (uint64_t*)&g_xCEcoTask1List_C761620F[g_indx];
        //}

   // g_xCEcoTaskScheduler1Lab_C761620F.m_pIArmTimer->pVTbl->Reset(g_xCEcoTaskScheduler1Lab_C761620F.m_pIArmTimer);
    g_xCEcoTask1List_C761620F[g_indx].pfunc();
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
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || pIBus == 0) {
        return -1;
    }

    /* Инициализация данных */
    pCMe->m_pTaskList = g_xCEcoTask1List_C761620F;
    pCMe->m_pIBus = pIBus;

    /* Получение интерфейса для работы с программируемым таймером */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoTimer1, 0, &IID_IEcoTimer1, (void**) &pCMe->m_pIArmTimer);
    /* Проверка */
    if (result != 0 || pCMe->m_pIArmTimer == 0) {
        return result;
    }

    /* Установка обработчика прерывания программируемого таймера */
    pCMe->m_pIArmTimer->pVTbl->set_Interval(pCMe->m_pIArmTimer, 1000000);
    pCMe->m_pIArmTimer->pVTbl->set_IrqHandler(pCMe->m_pIArmTimer, (voidptr_t*)CEcoTaskScheduler1Lab_C761620F_TimerHandler);

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
    int32_t indx = 0;
    int32_t reg = 30;
    uint64_t* pxTopOfStack = 0;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

    /* Проверяем указатель пула статических задач */
    for (indx = 0; indx < MAX_STATIC_TASK_COUNT; indx++) {
        if (g_xCEcoTask1List_C761620F[indx].pfunc == 0) {
            g_xCEcoTask1List_C761620F[indx].pfunc = address;
            g_xCEcoTask1List_C761620F[indx].m_cRef = 1;
            g_xCEcoTask1List_C761620F[indx].m_sp = (byte_t*)&g_xCEcoStackTask1List_C761620F[indx*4096];
            g_xCEcoTask1List_C761620F[indx].timeIn = 0; 
            pxTopOfStack = g_xCEcoTask1List_C761620F[indx].m_sp;
            while (reg > 0) {
                pxTopOfStack--;
                reg--;
            }
            *pxTopOfStack = (uint64_t)g_xCEcoTask1List_C761620F[indx].pfunc;
            g_xCEcoTask1List_C761620F[indx].m_pVTblITask = &g_x81A466F4C27540B1B33D0661E5470F1BVTbl_C761620F;
            *ppITask = (IEcoTask1*)&g_xCEcoTask1List_C761620F[indx];
            return 0;
            break;
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
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Start(/*in*/ IEcoTaskScheduler1Ptr_t me) {
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;

    int16_t i;
	int16_t earliestIn;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

	/*
    for (i = 0; pCMe->m_pTaskList[i].pfunc != 0; i++) {
        g_indx = i;
        if (g_indx >= MAX_STATIC_TASK_COUNT) {
            g_indx = 0;
        }
        else if (pCMe->m_pTaskList[g_indx].pfunc == 0) {
            g_indx = 0;
        }

        g_pxCurrentTCB_C761620F = (uint64_t*)&pCMe->m_pTaskList[g_indx];
        pCMe->m_pTaskList[g_indx].pfunc();
    }
	*/

	/*
	while (1) {
		if (g_indx >= MAX_STATIC_TASK_COUNT) {
			g_indx = 0;
		}
		if (pCMe->m_pTaskList[g_indx].pfunc != 0) {
			pCMe->m_pTaskList[g_indx].pfunc();
			pCMe->m_pTaskList[g_indx].pfunc = 0;
			g_indx++;
		}
		else {
			g_indx = 0;
		}
	}
	*/

	/* Строка для вывода информации о диспетчеризации */
	uint16_t schedulerLogRow = 35;
	char_t infoBuf[80] = {0};
	char_t numBuf[16] = {0};
	int16_t availableTasksCount = 0;
	
	/* Вывод заголовка информации о диспетчере FCFS */
	if (g_pIVideo != 0) {
		g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
			CHARACTER_ATTRIBUTE_FORE_COLOR_YELLOW, 
			"\xc4\xe8\xf1\xef\xe5\xf2\xf7\xe5\xf0\x3a\x20\xcf\xc5\xcf\xcf\x20\x28\xcf\xe5\xf0\xe2\xfb\xe9\x20\xef\xf0\xe8\xf8\xe5\xe4\x2c\x20\xef\xe5\xf0\xe2\xfb\xe9\x20\xee\xe1\xf1\xeb\xf3\xe6\xe5\xed\x29", 45);
		schedulerLogRow++; /* Пустая строка */
	}

	while (1) {
		earliestIn = -1;
		availableTasksCount = 0;
		
		/* Подсчет доступных задач и вывод информации о них */
		if (g_pIVideo != 0) {
			/* "Ожидающие выполнения: " */
			infoBuf[0] = '\xce'; infoBuf[1] = '\xe6'; infoBuf[2] = '\xe8'; infoBuf[3] = '\xe4';
			infoBuf[4] = '\xe0'; infoBuf[5] = '\xfe'; infoBuf[6] = '\xf9'; infoBuf[7] = '\xe8';
			infoBuf[8] = '\xe5'; infoBuf[9] = ' '; infoBuf[10] = '\xe2'; infoBuf[11] = '\xfb';
			infoBuf[12] = '\xef'; infoBuf[13] = '\xee'; infoBuf[14] = '\xeb'; infoBuf[15] = '\xed';
			infoBuf[16] = '\xe5'; infoBuf[17] = '\xed'; infoBuf[18] = '\xe8'; infoBuf[19] = '\xff';
			infoBuf[20] = ':'; infoBuf[21] = ' ';
			int pos = 22;
			
			/* Перечисление доступных задач */
			for (i = 0; i < MAX_STATIC_TASK_COUNT; i++) {
				if (pCMe->m_pTaskList[i].pfunc != 0) {
					if (availableTasksCount > 0) {
						infoBuf[pos++] = ',';
						infoBuf[pos++] = ' ';
					}
					infoBuf[pos++] = '#';
					u32_to_str_simple(i + 1, numBuf, sizeof(numBuf));
					for (int k = 0; numBuf[k] != '\0' && pos < 60; k++, pos++) {
						infoBuf[pos] = numBuf[k];
					}
					infoBuf[pos++] = '(';
					u32_to_str_simple(pCMe->m_pTaskList[i].timeIn, numBuf, sizeof(numBuf));
					for (int k = 0; numBuf[k] != '\0' && pos < 70; k++, pos++) {
						infoBuf[pos] = numBuf[k];
					}
					infoBuf[pos++] = ')';
					availableTasksCount++;
					for (int k = 0; k < sizeof(numBuf); k++) numBuf[k] = 0;
				}
			}
			infoBuf[pos] = '\0';
			
			if (availableTasksCount > 0) {
				g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
					CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, infoBuf, pos);
			}
			
			/* Очистка буферов */
			for (int k = 0; k < sizeof(infoBuf); k++) infoBuf[k] = 0;
		}
		

		for (i = 0; i < MAX_STATIC_TASK_COUNT; i++) {
			if (pCMe->m_pTaskList[i].pfunc != 0 && (earliestIn == -1 || pCMe->m_pTaskList[i].timeIn < pCMe->m_pTaskList[earliestIn].timeIn)) {
				earliestIn = i;
			}
		}

		/* Проверка наличия задач для выполнения */
		if (earliestIn == -1) {
			/* Нет задач для выполнения - выход из цикла планирования */
			if (g_pIVideo != 0) {
				g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
					CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, 
					"\xc2\xf1\xe5\x20\xe7\xe0\xe4\xe0\xf7\xe8\x20\xe7\xe0\xe2\xe5\xf0\xf8\xe5\xed\xfb\x2e\x20\xc4\xe8\xf1\xef\xe5\xf2\xf7\xe5\xf0\x20\xef\xf0\xe8\xee\xf1\xf2\xe0\xed\xee\xe2\xeb\xe5\xed\x2e", 38);
			}
			break;
		}

		/* Вывод подробной информации об активированной задаче */
		if (g_pIVideo != 0) {
			g_taskExecutionOrder++;
			
			/* Формирование строки с информацией о задаче */
			/* ">>> Активирована задача #M (Момент входа: T) - Последовательность: N" */
			infoBuf[0] = '>'; infoBuf[1] = '>'; infoBuf[2] = '>'; infoBuf[3] = ' ';
			/* "Активирована задача: " */
			infoBuf[4] = '\xc0'; infoBuf[5] = '\xea'; infoBuf[6] = '\xf2'; infoBuf[7] = '\xe8';
			infoBuf[8] = '\xe2'; infoBuf[9] = '\xe8'; infoBuf[10] = '\xf0'; infoBuf[11] = '\xee';
			infoBuf[12] = '\xe2'; infoBuf[13] = '\xe0'; infoBuf[14] = '\xed'; infoBuf[15] = '\xe0';
			infoBuf[16] = ' '; infoBuf[17] = '\xe7'; infoBuf[18] = '\xe0'; infoBuf[19] = '\xe4';
			infoBuf[20] = '\xe0'; infoBuf[21] = '\xf7'; infoBuf[22] = '\xe0'; infoBuf[23] = ' '; infoBuf[24] = '#';
			int pos = 25;
			/* Номер задачи (индекс + 1) */
			u32_to_str_simple(earliestIn + 1, numBuf, sizeof(numBuf));
			for (int j = 0; numBuf[j] != '\0' && pos < 25; j++, pos++) {
				infoBuf[pos] = numBuf[j];
			}
			infoBuf[pos++] = ' '; infoBuf[pos++] = '(';

			infoBuf[pos++] = '\xcc'; infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xec';
			infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xed'; infoBuf[pos++] = '\xf2';
			infoBuf[pos++] = ' '; infoBuf[pos++] = '\xef'; infoBuf[pos++] = '\xee';
			infoBuf[pos++] = '\xf1'; infoBuf[pos++] = '\xf2'; infoBuf[pos++] = '\xf3';
			infoBuf[pos++] = '\xf0'; infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xed';
			infoBuf[pos++] = '\xe8'; infoBuf[pos++] = '\xff'; infoBuf[pos++] = ':';
			infoBuf[pos++] = ' ';

			u32_to_str_simple(pCMe->m_pTaskList[earliestIn].timeIn, numBuf, sizeof(numBuf));
			for (int j = 0; numBuf[j] != '\0' && pos < 40; j++, pos++) {
				infoBuf[pos] = numBuf[j];
			}
			infoBuf[pos++] = ')'; infoBuf[pos++] = ' '; infoBuf[pos++] = '-';
			infoBuf[pos++] = ' '; 
			/* "Последовательность: " */
			infoBuf[pos++] = '\xcf'; infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xf1';
			infoBuf[pos++] = '\xeb'; infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xe4';
			infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xe2'; infoBuf[pos++] = '\xe0';
			infoBuf[pos++] = '\xf2'; infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xeb';
			infoBuf[pos++] = '\xed'; infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xf1';
			infoBuf[pos++] = '\xf2'; infoBuf[pos++] = '\xfc'; infoBuf[pos++] = ':';
			infoBuf[pos++] = ' ';
			/* Номер последовательности */
			u32_to_str_simple(g_taskExecutionOrder, numBuf, sizeof(numBuf));
			for (int j = 0; numBuf[j] != '\0' && pos < 70; j++, pos++) {
				infoBuf[pos] = numBuf[j];
			}
			infoBuf[pos] = '\0';
			
			g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
				CHARACTER_ATTRIBUTE_FORE_COLOR_LIGHT_GREEN, infoBuf, pos);
			
			/* Очистка буферов */
			for (int j = 0; j < sizeof(infoBuf); j++) infoBuf[j] = 0;
			for (int j = 0; j < sizeof(numBuf); j++) numBuf[j] = 0;
		}

		/* Выполнение активированной задачи (FCFS без вытеснения - задача выполняется до завершения) */
		pCMe->m_pTaskList[earliestIn].pfunc();
		
		/* Вывод информации о завершении задачи */
		if (g_pIVideo != 0) {
			/* "<<< Задача #N завершена (В очереди осталось: M)" */
			infoBuf[0] = '<'; infoBuf[1] = '<'; infoBuf[2] = '<'; infoBuf[3] = ' ';
			infoBuf[4] = '\xc7'; infoBuf[5] = '\xe0'; infoBuf[6] = '\xe4';
			infoBuf[7] = '\xe0'; infoBuf[8] = '\xf7'; infoBuf[9] = '\xe0';
			infoBuf[10] = ' '; infoBuf[11] = '\x23';
			u32_to_str_simple(earliestIn + 1, numBuf, sizeof(numBuf));
			int pos = 12;
			for (int j = 0; numBuf[j] != '\0' && pos < 18; j++, pos++) {
				infoBuf[pos] = numBuf[j];
			}
			infoBuf[pos++] = ' '; 
			infoBuf[pos++] = '\xe7'; infoBuf[pos++] = '\xe0'; infoBuf[pos++] = '\xe2';
			infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xf0'; infoBuf[pos++] = '\xf8';
			infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xed'; infoBuf[pos++] = '\xe0';
			infoBuf[pos++] = ' '; infoBuf[pos++] = '(';
			/* "В очереди осталось: " */
			infoBuf[pos++] = '\xc2'; infoBuf[pos++] = ' '; infoBuf[pos++] = '\xee';
			infoBuf[pos++] = '\xf7'; infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xf0';
			infoBuf[pos++] = '\xe5'; infoBuf[pos++] = '\xe4'; infoBuf[pos++] = '\xe8';
			infoBuf[pos++] = ' '; infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xf1';
			infoBuf[pos++] = '\xf2'; infoBuf[pos++] = '\xe0'; infoBuf[pos++] = '\xeb';
			infoBuf[pos++] = '\xee'; infoBuf[pos++] = '\xf1'; infoBuf[pos++] = '\xfc';
			infoBuf[pos++] = ':'; infoBuf[pos++] = ' ';
			/* Подсчет оставшихся задач */
			int remainingTasks = 0;
			for (int k = 0; k < MAX_STATIC_TASK_COUNT; k++) {
				if (pCMe->m_pTaskList[k].pfunc != 0 && k != earliestIn) {
					remainingTasks++;
				}
			}
			u32_to_str_simple(remainingTasks, numBuf, sizeof(numBuf));
			for (int j = 0; numBuf[j] != '\0' && pos < 50; j++, pos++) {
				infoBuf[pos] = numBuf[j];
			}
			infoBuf[pos++] = ')';
			infoBuf[pos] = '\0';
			
			g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
				CHARACTER_ATTRIBUTE_FORE_COLOR_CYAN, infoBuf, pos);
			
			/* Очистка буферов */
			for (int j = 0; j < sizeof(infoBuf); j++) infoBuf[j] = 0;
			for (int j = 0; j < sizeof(numBuf); j++) numBuf[j] = 0;
			
			schedulerLogRow++; /* Пустая строка для разделения */
		}
		
		/* Удаление выполненной задачи из очереди */
		pCMe->m_pTaskList[earliestIn].pfunc = 0;
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
