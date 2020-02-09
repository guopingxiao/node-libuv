#include <stdio.h>
#include <pthread.h> // 线程相关的api

#define NUM_THREADS 5 //线程数

int sum = 0;
pthread_mutex_t mutex;

// 线程执行,将输入的int参数作为max，将0，1，2，3，...，max依次加到sum上
void * thread_task(void * args) {
    int max = (int)args;

    for (int i = 0; i <= max; ++i) {
        pthread_mutex_lock(&mutex);
        sum += i;
        pthread_mutex_unlock(&mutex);
    }
    printf("sum: %i\n", sum);
    pthread_exit(NULL); // pthread_exist结束线程
}

int main() {

    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[NUM_THREADS]; // 定义了5个代表线程的数组threads

    pthread_attr_t attr; // 定义线程属性变量attr
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 8192); // 将线程的栈设为8192个字节

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_t thread = threads[i];  // 创建线程
        // 每个线程将会执行thread_task函数，并通过第3个参数将10传递给thread_task
        int result = pthread_create(&thread, &attr, thread_task, (void *)10);
        if (result) {
            printf("线程创建失败 errCode：%i", result);
            return -1;
        }
    }
    pthread_attr_destroy(&attr);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_t thread = threads[i];
        int result = pthread_join(thread, NULL); // 通过pthread_join告诉main函数等到所有线程执行完之后再继续执行
        if (result == 3) {
            printf("线程%i已经结束了\n", i);
            continue;
        }
    }

    printf("main函数运行结束, sum: %i\n", sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}