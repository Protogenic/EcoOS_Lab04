/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoVFB1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


#include <stdarg.h>
/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoMemoryManager1Lab.h"
#include "IEcoVirtualMemory1.h"
#include "IEcoTaskScheduler1.h"
#include "IdEcoTaskScheduler1Lab.h"
#include "IdEcoTimer1.h"
#include "IEcoTaskPriority1.h"
#include "IEcoSystemTimer1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoIPCCMailbox1.h"
#include "IdEcoVFB1.h"
#include "IEcoVBIOS1Video.h"
#include "IdEcoMutex1Lab.h"
#include "IdEcoSemaphore1Lab.h"

/*
#include "IEcoCGI1.h"
#include "IEcoCGI1VirtualDevice.h"
#include "IEcoCGI1OneWayOutput.h"
*/

/* Начало свободного участка памяти */
extern char_t __heap_start__;

/* Указатель на интерфейс для работы c мьютекс */
IEcoMutex1* g_pIMutex = 0;
/* Указатель на интерфейс для работы c семафор */
IEcoSemaphore1* g_pISemaphore = 0;

/* Указатель на интерфейсы */
IEcoVFB1* g_pIVFB = 0;
IEcoVBIOS1Video* g_pIVideo = 0;
IEcoSystemTimer1* g_pISysTimer = 0;

char_t g_strTask[2] = {0};
uint8_t g_taskSteps = 0; 

/* Глобальная позиция для журнала старта задач */
uint16_t g_logRow = 2;

/* Длительность задач (в условных единицах из TimeIn) */
uint16_t g_taskDuration1 = 0;
uint16_t g_taskDuration2 = 0;
uint16_t g_taskDuration3 = 0;
uint16_t g_taskDuration4 = 0;

/* Локальная утилита: преобразование числа в строку, чтобы не тянуть Eco.String1 */
static void u32_to_str(uint32_t v, char* buf, int bufsize) {
    int i = bufsize - 1;
    buf[i] = '\0';
    if (bufsize < 2) {
        return;
    }
    if (v == 0) {
        buf[--i] = '0';
    } else {
        while (i > 0 && v > 0) {
            buf[--i] = (char)('0' + (v % 10));
            v /= 10;
        }
    }
    /* сдвиг к началу */
    int j = 0;
    while (i < bufsize && buf[i] != '\0') {
        buf[j++] = buf[i++];
    }
    buf[j] = '\0';
}

/* Печать строки журнала «Пришла задача #N» в следующей строке экрана */
static void LogTaskStart(char_t* text) {
    if (g_pIVideo == 0) {
        return;
    }
    g_pIVideo->pVTbl->WriteString(g_pIVideo,
                                  0,
                                  0,
                                  0,
                                  g_logRow,
                                  CHARACTER_ATTRIBUTE_FORE_COLOR_CYAN,
                                  text,
                                  16);
    g_logRow++;
}

void TimerHandler(void) {
    g_pIMutex->pVTbl->Lock(g_pIMutex);
    //g_pISemaphore->pVTbl->Wait(g_pISemaphore, 0);
    if (g_strTask[0] == '\\') {
        g_strTask[0] = '|';
    }
    else if (g_strTask[0] == '|') {
        g_strTask[0] = '/';
    }
    else if (g_strTask[0] == '/') {
        g_strTask[0] = '-';
    }
    else  {
        g_strTask[0] = '\\';
    }
    g_pIMutex->pVTbl->UnLock(g_pIMutex);
    //g_pISemaphore->pVTbl->Post(g_pISemaphore);
}

void printProgress() {
	char numbuf[8];
	if (g_taskSteps > 0 && g_taskSteps % 10 == 0) {
		g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 4 + g_taskSteps / 10, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_LIGHT_GREEN, "\x23", 1);
	}
	if (g_taskSteps > 0 && g_taskSteps % 5 == 0) {
		u32_to_str(g_taskSteps, numbuf, sizeof(numbuf));
		g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 17, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, numbuf, 3);
	}

    if (g_strTask[0] == '\\') {
        g_strTask[0] = '|';
    }
    else if (g_strTask[0] == '|') {
        g_strTask[0] = '/';
    }
    else if (g_strTask[0] == '/') {
        g_strTask[0] = '-';
    }
    else  {
        g_strTask[0] = '\\';
    }
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 2, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, g_strTask, 1);
}

void Task1() {
    uint64_t currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    /* Шаг обновления зависит от заданного времени выполнения (чем больше executionTime, тем медленнее задача) */
    uint64_t baseStep = 20000ul;
    uint64_t factor = 1ull + (uint64_t)g_taskDuration1 / 10ull;
    uint64_t step = baseStep * factor;
    uint64_t changeTime = currentTime + step;
    LogTaskStart("\xcf\xf0\xe8\xf8\xeb\xe0\x20\xe7\xe0\xe4\xe0\xf7\xe0\x20\x23\x31"); /* «Пришла задача #1» */
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "1", 1);
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 4, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "\x7c\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x7c\x20\x20\x30\x20\x25", 17);
	g_taskSteps = 0;
    while (g_taskSteps < 100) {
        if (currentTime >= changeTime) {
            g_taskSteps++;
            printProgress();
            changeTime += step;
        }
        currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    }
}

void Task2() {
    uint64_t currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    uint64_t baseStep = 20000ul;
    uint64_t factor = 1ull + (uint64_t)g_taskDuration2 / 10ull;
    uint64_t step = baseStep * factor;
    uint64_t changeTime = currentTime + step;
	g_taskSteps = 0;
    LogTaskStart("\xcf\xf0\xe8\xf8\xeb\xe0\x20\xe7\xe0\xe4\xe0\xf7\xe0\x20\x23\x32"); /* «Пришла задача #2» */
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "2", 1);
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 4, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "\x7c\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x7c\x20\x20\x30\x20\x25", 17);
    while (g_taskSteps < 100) {
        if (currentTime >= changeTime) {
            g_taskSteps++;
            printProgress();
            changeTime += step;
        }
        currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    }
}

void Task3() {
    uint64_t currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    uint64_t baseStep = 20000ul;
    uint64_t factor = 1ull + (uint64_t)g_taskDuration3 / 10ull;
    uint64_t step = baseStep * factor;
    uint64_t changeTime = currentTime + step;
	g_taskSteps = 0;
    LogTaskStart("\xcf\xf0\xe8\xf8\xeb\xe0\x20\xe7\xe0\xe4\xe0\xf7\xe0\x20\x23\x33"); /* «Пришла задача #3» */
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "3", 1);
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 4, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "\x7c\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x7c\x20\x20\x30\x20\x25", 17);
    while (g_taskSteps < 100) {
        if (currentTime >= changeTime) {
            g_taskSteps++;
            printProgress();
            changeTime += step;
        }
        currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    }
}

void Task4() {
    uint64_t currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    uint64_t baseStep = 20000ul;
    uint64_t factor = 1ull + (uint64_t)g_taskDuration4 / 10ull;
    uint64_t step = baseStep * factor;
    uint64_t changeTime = currentTime + step;
	g_taskSteps = 0;
    LogTaskStart("\xcf\xf0\xe8\xf8\xeb\xe0\x20\xe7\xe0\xe4\xe0\xf7\xe0\x20\x23\x34"); /* «Пришла задача #4» */
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "4", 1);
    g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 4, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, "\x7c\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x2e\x7c\x20\x20\x30\x20\x25", 17);
    while (g_taskSteps < 100) {
        if (currentTime >= changeTime) {
            g_taskSteps++;
            printProgress();
            changeTime += step;
        }
        currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    }
}


/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    IEcoMemoryManager1* pIMemMgr = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoVirtualMemory1* pIVrtMem = 0;
    /* Указатель на интерфейс для работы с планировщиком */
    IEcoTaskScheduler1* pIScheduler = 0;
    IEcoTask1* pITask1 = 0;
    IEcoTask1* pITask2 = 0;
    IEcoTask1* pITask3 = 0;
	IEcoTask1* pITask4 = 0;
    /* Указатель на интерфейс для работы c буфером кадров видеоустройства */
    IEcoVFB1* pIVFB = 0;
    IEcoVBIOS1Video* pIVideo = 0;
    /* Указатель на интерфейс для работы c системным таймером */
    IEcoSystemTimer1* pISysTimer = 0;
    /* Указатель на интерфейс для работы c таймером */
    IEcoTimer1* pITimer = 0;

    uint16_t offset = 0;
    uint16_t x1 = 0;
    uint16_t y1 = 32;
    uint16_t x2 = 170;
    byte_t color = 170; /* 3-3-2 bit RGB */

    /* Создание экземпляра интерфейсной шины */
    result = GetIEcoComponentFactoryPtr_00000000000000000000000042757331->pVTbl->Alloc(GetIEcoComponentFactoryPtr_00000000000000000000000042757331, 0, 0, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    /* Проверка */
    if (result != 0 && pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с памятью */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoMemoryManager1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_0000000000000000000000004D656D31);
    //result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoMemoryManager1Lab, (IEcoUnknown*)GetIEcoComponentFactoryPtr_81589BFED0B84B1194524BEE623E1838);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с ящиком прошивки */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoIPCCMailbox1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_F10BC39A4F2143CF8A1E104650A2C302);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Запрос расширения интерфейсной шины */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        /* Установка расширения менаджера памяти */
        pIMemExt->pVTbl->set_Manager(pIMemExt, &CID_EcoMemoryManager1);
      //  pIMemExt->pVTbl->set_Manager(pIMemExt, &CID_EcoMemoryManager1Lab);
        /* Установка разрешения расширения пула */
        pIMemExt->pVTbl->set_ExpandPool(pIMemExt, 1);
    }

    /* Получение интерфейса управления памятью */
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryManager1, (void**) &pIMemMgr);
    //pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1Lab, 0, &IID_IEcoMemoryManager1, (void**) &pIMemMgr);
    if (result != 0 || pIMemMgr == 0) {
        /* Возврат в случае ошибки */
        return result;
    }

    /* Выделение области памяти 512 КБ */
    pIMemMgr->pVTbl->Init(pIMemMgr, &__heap_start__, 0x080000);

    /* Получение интерфейса для работы с виртуальной памятью */
    result = pIMemMgr->pVTbl->QueryInterface(pIMemMgr, &IID_IEcoVirtualMemory1, (void**)&pIVrtMem);
    if (result == 0 && pIVrtMem != 0) {
        /* Инициализация виртуальной памяти */
        result = pIVrtMem->pVTbl->Init(pIVrtMem);
        /* TO DO */
    }
    /* Регистрация статического компонента для работы с планировщиком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTaskScheduler1Lab, (IEcoUnknown*)GetIEcoComponentFactoryPtr_902ABA722D34417BB714322CC761620F);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с таймером */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTimer1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8DB93F3DF5B947D4A67F7C100B569723);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с VBF */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoVFB1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_939B1DCDB6404F7D9C072291AF252188);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

	/* Регистрация статического компонента для работы с мьютекс */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoMutex1Lab, (IEcoUnknown*)GetIEcoComponentFactoryPtr_2F48BBCBE4884CC08ECFC45990017215);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Регистрация статического компонента для работы с семафор */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoSemaphore1Lab, (IEcoUnknown*)GetIEcoComponentFactoryPtr_0741985B8FD0476C867CAE177CD26E7C);
    /* Проверка */
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса для работы с планировщиком */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTaskScheduler1Lab, 0, &IID_IEcoTaskScheduler1, (void**) &pIScheduler);
    /* Проверка */
    if (result != 0 || pIScheduler == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса для работы с видео сервисами VBF */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoVFB1, 0, &IID_IEcoVFB1, (void**) &pIVFB);
    if (result != 0 || pIVFB == 0) {
        goto Release;
    }
    /* Запрос интерфейса VBIOS */
    result = pIVFB->pVTbl->QueryInterface(pIVFB, &IID_IEcoVBIOS1Video, (void**)&pIVideo);
    if (result != 0 || pIVideo == 0) {
        goto Release;
    }

    /* Инициализация */
    pIScheduler->pVTbl->InitWith(pIScheduler, pIBus, &__heap_start__+0x090000, 0x080000);

    /* Создание статических задач (сообщения теперь выводятся при старте в самих задачах) */
	pIScheduler->pVTbl->NewTask(pIScheduler, Task1, 0, 0x100, &pITask1);
    pIScheduler->pVTbl->NewTask(pIScheduler, Task2, 0, 0x100, &pITask2);
    pIScheduler->pVTbl->NewTask(pIScheduler, Task3, 0, 0x100, &pITask3);
    pIScheduler->pVTbl->NewTask(pIScheduler, Task4, 0, 0x100, &pITask4);

	/* Инициализация времени выполнения для алгоритма планирования в порядке FIFO (FCFS)  */
    const uint16_t TASK1_EXEC_TIME = 20;
    const uint16_t TASK2_EXEC_TIME = 5;
    const uint16_t TASK3_EXEC_TIME = 10;
    const uint16_t TASK4_EXEC_TIME = 7;
    
    pITask1->pVTbl->SetExecutionTime(pITask1, TASK1_EXEC_TIME);
    pITask2->pVTbl->SetExecutionTime(pITask2, TASK2_EXEC_TIME);
    pITask3->pVTbl->SetExecutionTime(pITask3, TASK3_EXEC_TIME);
    pITask4->pVTbl->SetExecutionTime(pITask4, TASK4_EXEC_TIME);

    /* Связываем длительность задач с их временем выполнения */
    g_taskDuration1 = pITask1->pVTbl->GetExecutionTime(pITask1);
    g_taskDuration2 = pITask2->pVTbl->GetExecutionTime(pITask2);
    g_taskDuration3 = pITask3->pVTbl->GetExecutionTime(pITask3);
    g_taskDuration4 = pITask4->pVTbl->GetExecutionTime(pITask4);

	/* Получение интерфейса для работы с мьютекс */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMutex1Lab, 0, &IID_IEcoMutex1, (void**) &g_pIMutex);
    /* Проверка */
    if (result != 0 || g_pIMutex == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса для работы с семафор */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoSemaphore1Lab, 0, &IID_IEcoSemaphore1, (void**) &g_pISemaphore);
    /* Проверка */
    if (result != 0 || g_pISemaphore == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса для работы с системным таймером */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTimer1, 0, &IID_IEcoSystemTimer1, (void**) &pISysTimer);
    /* Проверка */
    if (result != 0 || pISysTimer == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    g_pISysTimer = pISysTimer;

    /* Установка обработчика прерывания программируемого таймера */
    result = pISysTimer->pVTbl->QueryInterface(pISysTimer, &IID_IEcoTimer1, (void**)&pITimer);
    /* Проверка */
    if (result != 0 || pITimer == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    pITimer->pVTbl->set_Interval(pITimer, 100000);
    pITimer->pVTbl->set_IrqHandler(pITimer, TimerHandler);
    pITimer->pVTbl->Start(pITimer);

    /* Вывод 1 строки "Эко ОС!!!" - кодовая страница 1251 */
    pIVideo->pVTbl->WriteString(pIVideo, 0, 0, 0, 0, CHARACTER_ATTRIBUTE_FORE_COLOR_YELLOW, "\xdd\xea\xee\x20\xce\xd1\x21\x21\x21", 9);

    /* Рисуем линию - подчеркивание */
    for (offset = x1; offset <= x2; offset++) {
        pIVideo->pVTbl->WriteDot(pIVideo, color, 0, offset, y1);
    }

    /* Вывод 4 строки "Привет Мир!" */
    //pIVFB->pVTbl->WriteString(pIVFB, 0, 0, 4, 4, CHARACTER_ATTRIBUTE_FORE_COLOR_GREEN, "\xcf\xf0\xe8\xe2\xe5\xf2\x20\xcc\xe8\xf0\x21", 11);
    //pIVFB->pVTbl->WriteString(pIVFB, 0, 0, 4, 5, CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, strHello, 13);
    g_pIVFB = pIVFB;
    g_pIVideo = pIVideo;
    pIScheduler->pVTbl->Start(pIScheduler);

	pIScheduler->pVTbl->NewTask(pIScheduler, Task4, 0, 0x100, &pITask4);

    while(1) {
        asm volatile ("NOP\n\t" ::: "memory");
    }

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение интерфейса VFB */
    if (pIVideo != 0) {
        pIVideo->pVTbl->Release(pIVideo);
    }
    if (pIVFB != 0) {
        pIVFB->pVTbl->Release(pIVFB);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}


