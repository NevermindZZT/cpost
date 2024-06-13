/**
 * @file cpost.h
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c post
 * @version 1.0.0
 * @date 2020-10-31
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#ifndef __CPOST_H__
#define __CPOST_H__

#ifdef CPOST_CFG_USER
    #include CPOST_CFG_USER
#endif

#define     CPOST_VERSION               "1.0.3"

#define     CPOST_FLAG_CLEAR_FRONT      0           // post列表中，存在相同handler, 清除之前的post
#define     CPOST_FLAG_CANCEL_CURRENT   1           // post列表中，存在相同handler, 取消当前post
#define     CPOST_FLAG_ADD_NEW          2           // 添加新post，不论post列表中是否存在相同handler

#define     CPOST_PARAM_DIFF_DISALBE    0           // 判断是否相同的handler时，忽略参数
#define     CPOST_PARAM_DIFF_ENABLE     1           // 判断是否相同的handler时，包括参数的比较

#ifndef CPOST_MAX_HANDLER_SIZE
    /**
    * @brief 最大handler数量
    */
    #define     CPOST_MAX_HANDLER_SIZE      16
#endif

#ifndef CPOST_GET_TICK
    /**
    * @brief 获取系统tick函数
    */
    #define     CPOST_GET_TICK()            0
#endif

#ifndef CPOST_MAX_TICK
    /**
    * @brief tick最大值
    */
    #define     CPOST_MAX_TICK              0xFFFFFFFF
#endif

typedef struct
{
    size_t startTime;
    size_t delay;
    void (*handler)(void *);
    void *param;
} CpostHandler;

typedef struct
{
    void *handler;
    void *param;
    size_t delay;
    struct {
        unsigned char flag : 2;
        unsigned char paramDiff: 1;
    } attrs;
} CpostParam;

/**
 * @brief cpsot
 * 
 * @param ... 参数列表
 * @note 参数按照`CpostParam`结构体进行传递
 *       按顺序传递时，直接按照`cpost(handler, param，delay)`传参
 *       不按顺序时，需要指定成员名，比如`cpost(handler, .delay=5000)`
 *       属性参数必须指定成员名，比如`cpost(handler, .attrs.flag=CPOST_FLAG_CANCEL_CURRENT)`
 * 
 * @return signed char 0 成功 -1 失败
 */
#define cpost(...) \
        cpostAddHandler(&((CpostParam){__VA_ARGS__}))

#define cpostDelay(_handler, _delay) \
        cpost(_handler, .delay=_delay)

#define cpostEx(_handler, _param) \
        cpost(_handler, _param)

#define cpostDelayEx(_handler, _param, _delay) \
        cpost(_handler, _param, _delay)

signed char cpostAddHandler(CpostParam *param);

void cpostProcess(void);

void cpostRemove(void *handler, void *param);

void cpostRemoveAll(void);

signed char cpostIsInList(void *handler, void *param);
#endif
