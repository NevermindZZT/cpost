/**
 * @file cevent.c
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c event
 * @version 1.0.0
 * @date 2020-11-15
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "cevent.h"

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    extern const size_t cEvent$$Base;
    extern const size_t cEvent$$Limit;
#elif defined(__ICCARM__) || defined(__ICCRX__)
    #pragma section="cEvent"
#elif defined(__GNUC__)
    extern const size_t _cevent_start;
    extern const size_t _cevent_end;
#endif

#if CEVENT_SPEED_OPTIMIZE == 1 && CEVENT_BUFFER_SIZE > 0
static size_t ceventBuffer[CEVENT_BUFFER_SIZE] = {0};
#endif

/**
 * @brief cevent注册表
 */
struct 
{
#if CEVENT_SPEED_OPTIMIZE == 1
    size_t **eventBase;                 /**< 事件基址 */
#else
    CEvent *base;                       /**< 表基址 */
    size_t count;                       /**< 表大小 */
#endif
} ceventTable;

/**
 * @brief cevent初始化
 */
void ceventInit(void)
{
    CEvent *base;
    size_t count;
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    base = (CEvent *)(&cEvent$$Base);
    count = ((size_t)(&cEvent$$Limit) - (size_t)(&cEvent$$Base)) / sizeof(CEvent);
#elif defined(__ICCARM__) || defined(__ICCRX__)
    base = (CEvent *)(__section_begin("cEvent"));
    count = ((size_t)(__section_end("cEvent")) - (size_t)(__section_begin("cEvent")))
            / sizeof(CEvent);
#elif defined(__GNUC__)
    base = (CEvent *)(&_cevent_start);
    count = ((size_t)(&_cevent_end) - (size_t)(&_cevent_start)) / sizeof(CEvent);
#else
    #error not supported compiler, please use command table mode
#endif

#if CEVENT_SPEED_OPTIMIZE == 1
    size_t maxEvent = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (base[i].event > maxEvent) {
            maxEvent = base[i].event;
        }
    }
    maxEvent += 1;
#if CEVENT_BUFFER_SIZE <= 0
    size_t *ceventBuffer = CEVENT_MALLOC(((maxEvent << 1) + count) * sizeof(size_t));
#endif /** CEVENT_BUFFER_SIZE <= 0 */
    ceventTable.eventBase = (size_t **) ceventBuffer;
    size_t *cur = ceventBuffer + maxEvent;
    for (size_t i = 0; i < maxEvent; i++)
    {
        ceventTable.eventBase[i] = cur;
        for (size_t j = 0; j < count; j++)
        {
            if (base[j].event == i) {
                *cur++ = (size_t) &base[j];
            }
        }
        *cur++ = NULL;
    }
#else /** CEVENT_SPEED_OPTIMIZE == 1 */
    ceventTable.base = base;
    ceventTable.count = count;
#endif
}

/**
 * @brief cevent运行
 * 
 * @param cevent cevent
 * 
 */
static void ceventRun(CEvent *cevent)
{
    if (cevent->paramNum < 1)
    {
        return;
    }
    void (*function)() = (void (*)())(cevent->param[0]);
    switch (cevent->paramNum)
    {
    case 1:
        function();
        break;
    case 2:
        function(cevent->param[1]);
        break;
    case 3:
        function(cevent->param[1], cevent->param[2]);
        break;
    case 4:
        function(cevent->param[1], cevent->param[2], cevent->param[3]);
        break;
    case 5:
        function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4]);
        break;
    case 6:
        function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
                 cevent->param[5]);
        break;
    case 7:
        function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
                 cevent->param[5], cevent->param[6]);
        break;
    case 8:
        function(cevent->param[1], cevent->param[2], cevent->param[3], cevent->param[4],
                 cevent->param[5], cevent->param[6], cevent->param[7]);
        break;
    default:
        break;
    }
}

/**
 * @brief cevent处理
 * 
 * @param event 事件
 * 
 */
static void ceventHandler(unsigned short event)
{
#if CEVENT_SPEED_OPTIMIZE == 1
    CEvent **cevent = (CEvent **) ceventTable.eventBase[event];
    while (*cevent != NULL)
    {
        ceventRun(*cevent++);
    }
#else
    for (size_t i = 0; i < ceventTable.count; i++)
    {
        if (ceventTable.base[i].event == event)
        {
            ceventRun(&(ceventTable.base[i]));
        }
    }
#endif
}

/**
 * @brief 广播event事件
 * 
 * @param event 事件
 * 
 */
void ceventPost(unsigned short event)
{
    ceventHandler(event);
}

