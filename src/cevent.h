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

#define     CEVENT_VERSION          "1.0.2"

/**
 * @brief cevent 速度优化
 *        打开这个开关需要同时配置 `CEVENT_BUFFER_SIZE` 或者 `CEVENT_MALLOC`
 *        开启后，cevent 会使用一部分内存，将所有注册的监听进行重排序，建立索引表，
 *        这样每次触发事件时就不需要进行遍历，能节省调用时间
 */
#define     CEVENT_SPEED_OPTIMIZE   1

/**
 * @brief cevent buffer
 *        `CEVENT_SPEED_OPTIMIZE` 开启时生效，这块内存用作速度优化
 *        buffer 大小计算为 `(maxEvent + 1) * 2 + ceventNum`
 *        (定义的最大事件号 + 1) * 2 + 注册的监听数量
 *        需要保证定义的数量大于这个数
 *        如果 `CEVENT_BUFFER_SIZE` 设置为 `0`, 则表示使用 `CEVENT_MALLOC` 分配内存
 */
#define     CEVENT_BUFFER_SIZE      64

/**
 * @brief 内存分配函数
 *        `CEVENT_SPEED_OPTIMIZE` 打开且 `CEVENT_BUFFER_SIZE` 配置为 `0` 时，
 *        需要配置这个宏
 */
#define     CEVENT_MALLOC(x)        0

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

