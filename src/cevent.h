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
        const void *cEventParam##_event##_func[] = {(void *)_func, ##__VA_ARGS__}; \
        const CEvent SECTION("cEvent") cEvent##_event##_func = \
        { \
            .param = cEventParam##_event##_func, \
            .paramNum = sizeof(cEventParam##_event##_func) / sizeof(void *), \
            .event = _event, \
        }

/**
 * @brief 导出事件(解决命名冲突)
 *        一般情况下不需要调用这个宏导出事件，当需要对同一个事件调用同一个函数时
 *        需要使用这个宏解决命名冲突
 * 
 * @param _alias 命名
 * @param _event 事件
 * @param _func 注册函数
 * @param ... 参数
 */
#define CEVENT_EXPORT_ALIAS(_alias, _event, _func, ...) \
        const void *cEventParam##_event##_func##_alias[] = {(void *)_func, ##__VA_ARGS__}; \
        const CEvent SECTION("cEvent") cEvent##_event##_func##_alias = \
        { \
            .param = cEventParam##_event##_func##_alias, \
            .paramNum = sizeof(cEventParam##_event##_func##_alias) / sizeof(void *), \
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

