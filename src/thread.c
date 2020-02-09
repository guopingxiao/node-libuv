/*
 * 在libuv中涉及到线程的总共有三部分：
 * 1、从线程池中调度一个线程运行回调: uv_queue_work
 * 2、使用async Handle来“唤醒” event loop线程，并在别的线程上执行回调：uv_async_send
 * 3、使用uv_thread_create手动创建一个线程来执行
 * 本demo除了可以学到上面说的三种方式，还涉及到的知识点有：
 * 1、线程间通信
 * 2、线程池调度
 * 3、线程间读写数据同步原语
 */
#include <stdio.h>
#include <unistd.h>
#include "uv.h"
#include "common.h"

uv_rwlock_t numlock;
uv_barrier_t barrier;
uv_async_t async_handle;
char msg[50];

// work_cb是会从线程池中调度一个线程去执行
void work_cb(uv_work_t *req) {
  printf("I am work callback, calling in some thread in thread pool, pid=>%d\n", uv_os_getpid());
  printf("work_cb thread id 0x%lx\n", (unsigned long int) uv_thread_self());


  // 发送消息给eventloop线程
  int r = 0;
  sprintf(msg, "This msg from another thread: 0x%lx", (unsigned long int) uv_thread_self());
  async_handle.data = (void *) &msg;
  r = uv_async_send(&async_handle);
  CHECK(r, "uv_async_send");
}

// after_work_cb是在event loop线程中执行
void after_work_cb(uv_work_t *req, int status) {
  printf("I am after work callback, calling from event loop thread, pid=>%d\n", uv_os_getpid());
  printf("after_work_cb thread id 0x%lx\n", (unsigned long int) uv_thread_self());
}

void async_cb(uv_async_t *handle) {
  printf("I am async callback, calling from event loop thread, pid=>%d\n", uv_os_getpid());
  printf("async_cb thread id 0x%lx\n", (unsigned long int) uv_thread_self());

  char *msg = (char *)handle->data;

  printf("I am receiving msg: %s\n", msg);

}

void timer_cb(uv_timer_t *handle) {
  uv_print_active_handles(handle->loop, stderr);
}


int main() {
  uv_loop_t *loop = uv_default_loop();
  int r = 0;
  uv_rwlock_init(&numlock);
  // 这里是4个线程，包含读线程2个、写线程1个、以及event loop线程
  uv_barrier_init(&barrier, 5);
  printf("I am the master process, processId => %d\n", uv_os_getpid());

  // 首先示例uv_queue_wok的用法
  uv_work_t work_handle;

  // 演示当需要线程的事件个数超过线程池的大小的时候，也就是当前没有空闲线程处理最后一个uv_queue_work，最后的一个请求将被挂起，直到有空闲的线程
  r = uv_queue_work(loop, &work_handle, work_cb, after_work_cb);
  CHECK(r, "uv_queue_work");

  // 接着测试async_handle的用法,初始化这个方法之后，进程不会主动退出，只有close掉才会
  r = uv_async_init(loop, &async_handle, async_cb);
  CHECK(r, "uv_async_init");

  return uv_run(loop, UV_RUN_DEFAULT);
}
