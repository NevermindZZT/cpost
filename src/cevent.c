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
    extern const unsigned int cEvent$$Base;
    extern const unsigned int cEvent$$Limit;
#elif defined(__ICCARM__) || defined(__ICCRX__)
    #pragma section="cEvent"
#elif defined(__GNUC__)
    extern const unsigned int _cevent_start;
    extern const unsigned int _cevent_end;
#endif

/**
 * @brief cevent注册表
 */
struct 
{
    CEvent *base;                       /**< 表基址 */
    size_t count;                       /**< 表大小 */
} ceventTable;

/**
 * @brief cevent初始化
 */
void ceventInit(void)
{
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    ceventTable.base = (CEvent *)(&cEvent$$Base);
    ceventTable.count = ((unsigned int)(&cEvent$$Limit)
                            - (unsigned int)(&cEvent$$Base))
                            / sizeof(CEvent);
#elif defined(__ICCARM__) || defined(__ICCRX__)
    ceventTable.base = (CEvent *)(__section_begin("cEvent"));
    ceventTable.count = ((unsigned int)(__section_end("cEvent"))
                            - (unsigned int)(__section_begin("cEvent")))
                            / sizeof(CEvent);
#elif defined(__GNUC__)
    ceventTable.base = (CEvent *)(&_cevent_start);
    ceventTable.count = ((unsigned int)(&_cevent_end)
                            - (unsigned int)(&_cevent_start))
                            / sizeof(CEvent);
#else
    #error not supported compiler, please use command table mode
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
    for (size_t i = 0; i < ceventTable.count; i++)
    {
        if (ceventTable.base[i].event == event)
        {
            ceventRun(&(ceventTable.base[i]));
        }
    }
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

