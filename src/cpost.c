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


CpostHandler cpostHandlers[CPOST_MAX_HANDLER_SIZE] = {0};


/**
 * @brief cpost 添加handler
 * 
 * @param param 参数
 * 
 * @return signed char 0 添加成功 -1 添加失败
 */
signed char cpostAddHandler(CpostParam *param)
{
    if (cpostIsInList(param->handler, param->attrs.paramDiff ? param->param : NULL) == 0)
    {
        switch (param->attrs.flag)
        {
        case CPOST_FLAG_CLEAR_FRONT:
            cpostRemove(param->handler, param->attrs.paramDiff ? param->param : NULL);
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
        if (cpostHandlers[i].handler == NULL)
        {
        #if CPOST_MULTI_PROCESS == 1
            cpostHandlers[i].process = param->process;
        #endif
            cpostHandlers[i].startTime = CPOST_GET_TICK();
            cpostHandlers[i].delay = param->delay;
            cpostHandlers[i].handler = (void (*)(void *))(param->handler);
            cpostHandlers[i].param = param->param;
            return 0;
        }
    }
    return -1;
}


/**
 * @brief 移除handler
 * 
 * @param handler handler
 * @param param 参数，传 `NULL` 表示不比较参数
 * 
 */
void cpostRemove(void *handler, void *param)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cpostHandlers[i].handler == handler
            && (param == NULL || param == cpostHandlers[i].param))
        {
            cpostHandlers[i].handler = NULL;
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
        cpostHandlers[i].handler = NULL;
    }
}


/**
 * @brief 判断handler是否已经在列表中
 * 
 * @param handler handler
 * @param param 参数，传 `NULL` 表示不比较参数
 * 
 * @return signed char 0 存在 -1 不存在
 */
signed char cpostIsInList(void *handler, void *param)
{
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
        if (cpostHandlers[i].handler == handler
            && (param == NULL || param == cpostHandlers[i].param))
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
#if CPOST_MULTI_PROCESS == 1
void cpostProcess(size_t process)
#else
void cpostProcess(void)
#endif
{
    size_t tick;
    for (size_t i = 0; i < CPOST_MAX_HANDLER_SIZE; i++)
    {
    #if CPOST_MULTI_PROCESS == 1
        if (cpostHandlers[i].handler &&  cpostHandlers[i].process == process)
    #else
        if (cpostHandlers[i].handler)
    #endif
        {
            tick = CPOST_GET_TICK();
            if (cpostHandlers[i].delay == 0 || 
                (CPOST_MAX_TICK - cpostHandlers[i].startTime > cpostHandlers[i].delay
                    ? tick - cpostHandlers[i].startTime >= cpostHandlers[i].delay
                    : CPOST_MAX_TICK - cpostHandlers[i].startTime + tick >= cpostHandlers[i].delay))
            {
                cpostHandlers[i].handler(cpostHandlers[i].param);
                cpostHandlers[i].handler = NULL;
            }
        }
    }
}
