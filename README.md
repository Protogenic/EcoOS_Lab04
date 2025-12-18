# EcoLab04
---

## 1. Постановка задачи

Цель: В рамках данной лабораторной работы необходимо реализовать компонент для операционной системы Eco OS, который обеспечивает планирование задач по алгоритму FCFS (First-Come, First-Served) без вытеснения. 

---

## 2. Алгоритм First-Come, First-Served

Алгоритм FCFS (First-Come, First-Served) представляет собой одну из базовых дисциплины планирования задач в операционных системах.

### Основные характеристики:

- **Порядок обслуживания**: Задачи обрабатываются строго в последовательности их поступления в систему. Каждой задаче присваивается момент входа `timeIn`. Диспетчер всегда выбирает задачу с минимальным значением `timeIn` среди еще не обработанных задач.

- **Без вытеснения (non-preemptive)**: После начала выполнения задача работает до полного завершения, не может быть прервана другими задачами по таймеру или приоритету.

---

## 3. Реализация

### 3.1. Структура компонента

Компонент `CEcoTaskScheduler1Lab` реализует алгоритм планирования задач FCFS без вытеснения для платформы Eco OS. Компонент состоит из двух основных частей:

- **Диспетчер задач (CEcoTaskScheduler1Lab)** - основной компонент, управляющий очередью задач и их выполнением
- **Задача (CEcoTask1Lab)** - структура данных, представляющая отдельную задачу в системе

#### Структура диспетчера

```c
typedef struct CEcoTaskScheduler1Lab_C761620F {
    /* Таблица функций интерфейса IEcoTaskScheduler1Lab */
    IEcoTaskScheduler1VTbl* m_pVTblIScheduler;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейсная Шина */
    IEcoInterfaceBus1* m_pIBus;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Данные экземпляра */
    IEcoTimer1Ptr_t m_pIArmTimer;
    CEcoTask1Lab_C761620F* m_pTaskList;

} CEcoTaskScheduler1Lab_C761620F, *CEcoTaskScheduler1Lab_C761620FPtr;
```

#### Структура задачи

```c
typedef struct CEcoTask1Lab_C761620F {
    /* Таблица функций интерфейса IEcoTask1 */
    IEcoTask1VTbl* m_pVTblITask;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Стэк */
    volatile byte_t* m_sp;

    /* Данные экземпляра */
    void (*pfunc) (void);

    /* Параметры задачи */ 
    uint16_t timeIn;

} CEcoTask1Lab_C761620F, *CEcoTask1Lab_C761620F_Ptr;
```

### 3.2. Интерфейс диспетчера

Интерфейс `IEcoTaskScheduler1` содержит следующие методы:
- `Init` - инициализация диспетчера
- `InitWith` - инициализация с параметрами
- `NewTask` - создание новой задачи
- `Start` - запуск диспетчера
- `Stop` - остановка диспетчера
- `DeleteTask` - удаление задачи
- `SuspendTask` - приостановка задачи
- `ResumeTask` - возобновление задачи

#### Интерфейс задачи

Интерфейс `IEcoTask1` расширен методами для работы с моментом входа:
- `SetTimeIn` - установка момента входа задачи
- `GetTimeIn` - получение момента входа задачи

### 3.3. Ключевые функции реализации

#### Функция создания задачи (NewTask)

Функция `NewTask` создает новую задачу и добавляет её в список задач:

```c
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_NewTask(
    /*in*/ IEcoTaskScheduler1Ptr_t me, 
    /*in*/ voidptr_t address, 
    /*in*/ voidptr_t data, 
    /*in*/ uint32_t stackSize, 
    /* out */ IEcoTask1** ppITask) {
    
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
```

#### Функция установки момента входа (SetTimeIn)

Функция `SetTimeIn` устанавливает момент входа задачи в систему:

```c
uint16_t ECOCALLMETHOD CEcoTask1Lab_C761620F_SetTimeIn(
    IEcoTask1Ptr_t me, 
    uint16_t time) {
    
    CEcoTask1Lab_C761620F* pCMe = (CEcoTask1Lab_C761620F*)me;
    if (me == 0 ) {
        return -1;
    }

    pCMe->timeIn = time;
    return 0;
}
```

#### Функция запуска диспетчера (Start)

Функция `Start` реализует основной алгоритм FCFS - поиск задачи с минимальным моментом входа и её выполнение:

```c
int16_t ECOCALLMETHOD CEcoTaskScheduler1Lab_C761620F_Start(
    /*in*/ IEcoTaskScheduler1Ptr_t me) {
    
    CEcoTaskScheduler1Lab_C761620F* pCMe = (CEcoTaskScheduler1Lab_C761620F*)me;
    int16_t i;
    int16_t earliestIn;

    /* Проверка указателей */
    if (me == 0 ) {
        return -1;
    }

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
        schedulerLogRow++;
    }

    while (1) {
        earliestIn = -1;
        availableTasksCount = 0;
        
        /* Поиск задачи с наименьшим моментом поступления (FCFS) */
        for (i = 0; i < MAX_STATIC_TASK_COUNT; i++) {
            if (pCMe->m_pTaskList[i].pfunc != 0 && 
                (earliestIn == -1 || 
                 pCMe->m_pTaskList[i].timeIn < pCMe->m_pTaskList[earliestIn].timeIn)) {
                earliestIn = i;
            }
        }

        /* Проверка наличия задач для выполнения */
        if (earliestIn == -1) {
            /* Нет задач для выполнения - выход из цикла */
            if (g_pIVideo != 0) {
                g_pIVideo->pVTbl->WriteString(g_pIVideo, 0, 0, 0, schedulerLogRow++, 
                    CHARACTER_ATTRIBUTE_FORE_COLOR_WHITTE, 
                    "\xc2\xf1\xe5\x20\xe7\xe0\xe4\xe0\xf7\xe8\x20\xe7\xe0\xe2\xe5\xf0\xf8\xe5\xed\xfb\x2e\x20\xc4\xe8\xf1\xef\xe5\xf2\xf7\xe5\xf0\x20\xef\xf0\xe8\xee\xf1\xf2\xe0\xed\xee\xe2\xeb\xe5\xed\x2e", 38);
            }
            break;
        }

        /* Выполнение активированной задачи (FCFS без вытеснения) */
        pCMe->m_pTaskList[earliestIn].pfunc();
        
        /* Удаление выполненной задачи из очереди */
        pCMe->m_pTaskList[earliestIn].pfunc = 0;
    }

    return 0;
}
```

### 3.4. Пример использования

В функции `EcoMain` создаются задачи и устанавливаются их моменты входа:

```c
/* Создание статических задач */
pIScheduler->pVTbl->NewTask(pIScheduler, Task1, 0, 0x100, &pITask1);
pIScheduler->pVTbl->NewTask(pIScheduler, Task2, 0, 0x100, &pITask2);
pIScheduler->pVTbl->NewTask(pIScheduler, Task3, 0, 0x100, &pITask3);
pIScheduler->pVTbl->NewTask(pIScheduler, Task4, 0, 0x100, &pITask4);

/* Инициализация момента входа для алгоритма планирования FCFS */
pITask1->pVTbl->SetTimeIn(pITask1, 15);
pITask2->pVTbl->SetTimeIn(pITask2, 8);
pITask3->pVTbl->SetTimeIn(pITask3, 12);
pITask4->pVTbl->SetTimeIn(pITask4, 3);

/* Связываем длительность задач с их моментом входа */
g_taskDuration1 = pITask1->pVTbl->GetTimeIn(pITask1);
g_taskDuration2 = pITask2->pVTbl->GetTimeIn(pITask2);
g_taskDuration3 = pITask3->pVTbl->GetTimeIn(pITask3);
g_taskDuration4 = pITask4->pVTbl->GetTimeIn(pITask4);
```

Пример реализации задачи:

```c
void Task1() {
    uint64_t currentTime = g_pISysTimer->pVTbl->get_SingleTimerCounter(g_pISysTimer);
    
    uint64_t baseStep = 20000ul;
    uint64_t factor = 1ull + (uint64_t)g_taskDuration1 / 10ull;
    uint64_t step = baseStep * factor;
    uint64_t changeTime = currentTime + step;
    
    LogTaskStart("\xd1\xf2\xe0\xf0\xf2\x20\xef\xee\xeb\xf3\xf7\xe5\xed\xe8\xff\x20\xe7\xe0\xe4\xe0\xf7\xe8\x20\x23\x31", g_taskDuration1);
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
```

### 3.5. Порядок выполнения задач

При заданных моментах входа:
- Задача #1: `timeIn = 15`
- Задача #2: `timeIn = 8`
- Задача #3: `timeIn = 12`
- Задача #4: `timeIn = 3`

Порядок выполнения будет следующим:
1. Задача #4 (минимальный `timeIn = 3`)
2. Задача #2 (`timeIn = 8`)
3. Задача #3 (`timeIn = 12`)
4. Задача #1 (`timeIn = 15`)

Это соответствует принципу FCFS: задачи выполняются в порядке возрастания момента входа.
