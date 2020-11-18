/**
 * @file cevent.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c event
 * @version 1.0.0
 * @date 2020-11-15
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __CEVENT_H__
#define __CEVENT_H__

#define     CEVENT_VERSION          "1.0.0"

#ifndef SECTION
    #if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
        #define SECTION(x)                  __attribute__((used, section(x)))
    #elif defined(__ICCARM__) || defined(__ICCRX__)
        #define SECTION(x)                  @ x
    #elif defined(__GNUC__)
        #define SECTION(x)                  __attribute__((section(x)))
    #else
        #define SECTION(x)
    #endif
#endif

/**
 * @brief 导出事件
 * 
 * @param _event 事件
 * @param _func 注册函数
 * @param ... 参数
 */
#define CEVENT_EXPORT(_event, _func, ...) \
        const void *cEventParam##_func[] = {(void *)_func, ##__VA_ARGS__}; \
        const CEvent SECTION("cEvent") cEvent##_func= \
        { \
            .param = cEventParam##_func, \
            .paramNum = sizeof(cEventParam##_func) / sizeof(void *), \
            .event = _event, \
        }

/**
 * @brief CEvent
 */
typedef struct 
{
    const void **param;                         /**< 参数(包括函数) */
    const unsigned char paramNum;               /**< 参数数量 */
    const unsigned short event;                 /**< 监听事件 */
} CEvent;


void ceventInit(void);

void ceventPost(unsigned short event);

#endif

