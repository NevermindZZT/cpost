le cpost.c
 * @author Letter (nevermindzzt@gmail.com)
 * @brief c post
 * @version 1.0.0
 * @date 2020-10-31
 * 
 * @copyright (c) 2020 Letter
 * 
 */
#include "cpost.h"


CpostHandler cposhHandlers[CPOST_MAX_HANDLER_SIZE] = {0};

/**
 * @brief cpost 添加 handler
 * 
 * @param time 执行时间
 * @param handler handler
 * 
 * @return size_t 0 添加成功 -1 添加失败
 */
size_t cpostAddHandler(size_t time, void *handler, void *param)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == NULL)
        {
            cposhHandlers[i].time = time;
            cposhHandlers[i].handler = (void (*)(void *))handler;
            cposhHandlers[i].param = param;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief post handler
 * 
 * @param handler handler 
 * 
 * @return size_t 0 添加成功 -1 添加失败
 */
size_t cpost(void *handler)
{
    return cpostAddHandler(0, handler, NULL);
}

/**
 * @brief 延迟 post handler
 * 
 * @param handler handler
 * @param delay 延时时间(tick)
 * 
 * @return size_t 0 添加成功 -1 添加失败
 */
size_t cpostDelay(void *handler, size_t delay)
{
    return cpostAddHandler(CPOST_GET_TICK() + delay, handler, NULL);
}

/**
 * @brief 带参数post
 * 
 * @param handler handler
 * @param param 参数
 * 
 * @return size_t 0 添加成功 -1 添加失败
 */
size_t cpostEx(void *handler, void *param)
{
    return cpostAddHandler(0, handler, param);
}

/**
 * @brief 带参数延迟post
 * 
 * @param handler handler
 * @param param 参数
 * @param delay 延时时间(tick)
 * 
 * @return size_t 0 添加成功 -1 添加失败
 */
size_t cpostDelayEx(void *handler, void *param, size_t delay)
{
    return cpostAddHandler(CPOST_GET_TICK() + delay, handler, param);
}

/**
 * @brief 移除handler
 * 
 * @param handler handler
 * 
 */
void cpostRemove(void *handler)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == handler)
        {
            cposhHandlers[i].handler = NULL;
        }
    }
}

/**
 * @brief 移除所有handler
 * 
 */
void cpostRemoveAll(void)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        cposhHandlers[i].handler = NULL;
    }
}

/**
 * @brief cpost 处理
 * 
 */
void cpostProcess(void)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler)
        {
            if (cposhHandlers[i].time == 0 || CPOST_GET_TICK() >= cposhHandlers[i].time)
            {
                cposhHandlers[i].handler(cposhHandlers[i].param);
                cposhHandlers[i].handler = NULL;
            }
        }
    }
}

