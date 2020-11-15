# cpost

c语言上下文切换工具

## 简介

[cpost](https://github.com/NevermindZZT/cpost)是一个c语言编写的，用于c语言上下文切换的工具

`cpost`借鉴了Android的`Handler`机制，在c语言环境中，可以通过调用`cpost`接口，将函数抛出到另外的线程(上下文)中运行，对于某些场景，尤其是嵌入式编程无操作系统环境下的中断延迟处理

## 使用

以使用在嵌入式无操作系统中为例，主要用作中断延迟处理的情况

1. 配置系统tick

    配置`cpost.h`中的宏`CPOST_GET_TICK()`，配置成获取系统tick

2. 配置处理进程

    在main loop调用`cpostProcess`函数，通过`cpost`执行的函数都会在`cpostProcess`中执行

## Api

`cpost`提供了简单的接口用于延迟处理函数，其接口设计和Android的`Handler`设计类似

### post

post接口会将函数抛出，然后在`cpostProcess`中立刻执行

```c
size_t cpost(void *handler);
```

- 参数

  - `handler` 被抛出的函数

- 返回值

  - `size_t` 0 post成功 -1 post失败

```c
size_t cpostEx(void *handler, void *param);
```

- 参数

  - `handler` 被抛出的函数
  - `param` 传递给被抛出函数的参数

- 返回值

  - `size_t` 0 post成功 -1 post失败

### post delay

post delay接口会将函数抛出，然后在挂起在`cpostProcess`中，当延时时间达到后执行

```c
size_t cpostDelay(void *handler, size_t delay);
```

- 参数

  - `handler` 被抛出的函数
  - `delay` 延时时间(tick)

- 返回值

  - `size_t` 0 post成功 -1 post失败

```c
size_t cpostDelayEx(void *handler, void *param, size_t delay);
```

- 参数

  - `handler` 被抛出的函数
  - `param` 传递给被抛出函数的参数
  - `delay` 延时时间(tick)

- 返回值

  - `size_t` 0 post成功 -1 post失败
