## Learning Libuv

You can use CLion to build this project, or other IDE support Cmake to build.

Base on libuv version v1.31.1

学习和借鉴了几篇文章:

[00.libuv学习之编译-句柄-请求](./docs/00.libuv学习之编译-句柄-请求)

[01.libuv学习之事件循环-线程池](./docs/01.libuv学习之事件循环-线程池)

[02.Libuv学习之线程池](./docs/02.Libuv学习之线程池)

[03.从libuv源码中学习最小二叉堆](./docs/03.从libuv源码中学习最小二叉堆)

## Demos

| 文件名     | 涉及到的知识点                                                                       |
| ------------- | ------------------------------------------------------------------------------------------- |
| hello_libuv.c | 如何启动Libuv事件循环                                                               |
| idle.c        | 了解idle句柄的使用，并查看idle句柄的回调和prepare句柄的回调执行顺序 |
| fs.c          | 掌握libuv是如何读写文件的一般思路                                             |
| process.c     | 掌握libuv是如何创建进程的一般步骤                                             |
| tcpserver.c   | 掌握libuv启动一个tcp服务器的基本步骤，并学习计时器句柄的使用，以及如何获取活跃的句柄 |
| udpserver.c   | 掌握libuv启动一个udp服务器的基本步骤，并学习信号句柄的使用         |
| thread.c      | 掌握libuv是如何使用线程的。包括线程创建、异步句柄的使用、线程池调度、以及读写锁和屏障的使用 |
| pipe          | 掌握libuv是如何使用管道的                                                         |


## Knowledge Points

  - ✅ 怎么启动libuv的事件循环
  - ✅ uv_loop_t — Event loop
  - ✅ Timer handle
  - ✅ Idle handle
  - ✅ Async handle
  - ✅ Signal handle
  - ✅ Process handle
  - ✅ Stream handle
  - ✅ TCP handle
  - ✅ Pipe handle
  - ✅ UDP handle
  - ✅ File system operations
  - ✅ Thread pool work scheduling
  - ✅ DNS utility functions
  - ✅ Threading and synchronization utilities
  - ☑️ Prepare handle
  - ☑️ Check handle
  - ☑️ Poll handle
  - ☑️ TTY handle
  - ☑️ FS Event handle
  - ☑️ FS Poll handle
  - ☑️ Shared library handling
  - ☑️
  - ☑️
  - ☑️
