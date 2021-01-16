/**
 * @file cpost.c
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
 * @brief cpost 添加handler
 * 
 * @param param 参数
 * 
 * @return signed char 0 添加成功 -1 添加失败
 */
signed char cpostAddHandler(CpostParam *param)
{
    if (cpostIsInList(param->handler) == 0)
    {
        switch (param->attrs.flag)
        {
        case CPOST_FLAG_CLEAR_FRONT:
            cpostRemove(param->handler);
            break;

        case CPOST_FLAG_CANCEL_CURRENT:
            return 0;
            // break;

        case CPOST_FLAG_ADD_NEW:
            // do nothint
            break;

        default:
            break;
        }
    }

    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == NULL)
        {
            cposhHandlers[i].time = 
                param->delay ? CPOST_GET_TICK() + param->delay : 0;
            cposhHandlers[i].handler = (void (*)(void *))(param->handler);
            cposhHandlers[i].param = param->param;
            return 0;
        }
    }
    return -1;
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
 * @brief 判断handler是否已经在列表中
 * 
 * @param handler handler
 * 
 * @return signed char 0 存在 -1 不存在
 */
signed char cpostIsInList(void *handler)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler == handler)
        {
            return 0;
        }
    }
    return -1;
}

/**
 * @brief cpost 处理
 * 
 */
void cpostProcess(void)
{
    size_t tick;
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cposhHandlers[i].handler)
        {
            tick = CPOST_GET_TICK();
            if (cposhHandlers[i].time == 0 || 
                (tick >= cposhHandlers[i].time 
                    ? CPOST_GET_TICK() >= cposhHandlers[i].time
                    : CPOST_MAX_TICK - cposhHandlers[i].time + CPOST_GET_TICK()))
            {
                cposhHandlers[i].handler(cposhHandlers[i].param);
                cposhHandlers[i].handler = NULL;
            }
        }
    }
}

