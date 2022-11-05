# cpost

c语言程序上下文切换和解耦的工具

- [cpost](#cpost)
  - [简介](#简介)
  - [使用](#使用)
    - [cpost使用](#cpost使用)
  - [cevent使用](#cevent使用)
  - [Api](#api)
    - [post](#post)
    - [event post](#event-post)
    - [event export](#event-export)

## 简介

[cpost](https://github.com/NevermindZZT/cpost)是一个c语言编写的，用于c语言程序上下文切换和解耦的工具

`cpost`包含`cpost`和`cevent`两个工具，`cpost`用于c语言的上下文切换，`cevent`用于程序模块之间的解耦

`cpost`借鉴了Android的`Handler`机制，在c语言环境中，可以通过调用`cpost`接口，将函数抛出到另外的线程(上下文)中运行，对于某些场景，尤其是嵌入式编程无操作系统环境下的中断延迟处理

`cevent`借鉴了Android的广播机制，在c语言环境中，当程序运行至相应的位置，可以通过`cevent`接口抛出一个事件，其他模块可以通过注册的方式，监听这个事件，当事件发生时，调用注册的函数，能很大程度上实现模块间的解耦

## 使用

`cpost`和`cevent`可以同时使用，也可以独立使用，特别针对于嵌入式无操作系统的环境，使用`cpost`和`cevent`，可以简化编程，更容易实现模块化的编程

### cpost使用

以使用在嵌入式无操作系统中为例，主要用作中断延迟处理的情况

1. 配置系统tick

    配置`cpost.h`中的宏`CPOST_GET_TICK()`，配置成获取系统tick

2. 配置处理进程

    在main loop调用`cpostProcess`函数，通过`cpost`执行的函数都会在`cpostProcess`中执行

## cevent使用

`cevent`使用注册的方式监听事件，会依赖于编译环境，目前支持keil，iar，和gcc，对于gcc，需要修改链接文件(.ld)，在只读数据区添加：

```ld
_cevent_start = .;
KEEP (*(cEvent))
_cevent_end = .;
```

1. 初始化cevent

    系统初始化时，调用`ceventInit`

    ```c
    ceventInit();
    ```

2. 注册cevent事件监听

    在c文件中，调用`CEVENT_EXPORT`导出事件监听

    ```c
    CEVENT_EXPORT(0, handler, (void *)param);
    ```

3. 发送cevent事件

    在事件发生的地方，调用`ceventPost`抛出事件

    ```c
    ceventPost(0);
    ```

## Api

`cpost`提供了简单的接口用于延迟处理函数，其接口设计和Android的`Handler`设计类似，`cevnet`提供了一个事件发送接口，用于抛出事件

### post

`cpost` 提供一个统一接口 `cpost(...)` 进行事件抛出，此接口通过宏定义，需要在 c99 以上的标准运行

```c
#define cpost(...) \
        cpostAddHandler(&((CpostParam){__VA_ARGS__}))
```

- 参数

  - `...` 抛出的参数，用于构造 `CpostParam` 对象

  ```c
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
  ```

  - `handler` 被抛出执行的函数
  - `param` 传递给函数的参数
  - `delay` 延迟时间
  - `attrs.flag` 任务冲突时处理 flag
  - `attrs.paramDiff` 比较任务时是否区分参数

### event post

event post接口会抛出事件，事件定义为一个`unsigned short`整形，建议通过宏定义所有事件

```c
void ceventPost(unsigned short event);
```

- 参数

  - `event` 事件

### event export

event post接口会静态注册一个事件监听，注意，此接口实际上为定义一个全局的常量，所以需要写在函数体外

```c
CEVENT_EXPORT(_event, _func, ...)
```

- 参数

  - `_event` 被监听的事件
  - `_func` 事件发生时，执行的函数
  - `...` 需要传递给`_func`函数的参数
