le cpost.h
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

#define     CPOST_VERSION               "1.0.0"

/**
 * @brief 最大handler数量
 */
#define     CPOST_MAX_HANDLER_SIZE      16

/**
 * @brief 获取系统tick函数
 */
#define     CPOST_GET_TICK()            0

typedef struct
{
    size_t time;
    void (*handler)(void *);
    void *param;
} CpostHandler;

size_t cpost(void *handler);

size_t cpostDelay(void *handler, size_t delay);

size_t cpostEx(void *handler, void *param);

size_t cpostDelayEx(void *handler, void *param, size_t delay);

void cpostProcess(void);

void cpostRemove(void *handler);

void cpostRemoveAll(void);

#endif

