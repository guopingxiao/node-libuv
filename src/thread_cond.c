#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int i = 0;

pthread_mutex_t mutex; // 互斥锁mutex
pthread_cond_t cond0; // 条件变量cond0
pthread_cond_t cond1; // 条件变量cond1

void * thread_task0(void * args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (i != 0) {
            // pthread_cond_wait会放弃当前线程获得的锁，并进入挂起状态。
            // pthread_cond_signal通知该线程时
            pthread_cond_wait(&cond0, &mutex);
        }
        sleep(1);

        printf("**************thread_task0 i: %i\n", i);
        i = 1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond1);
    }
}

void * thread_task1(void * args) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (i != 1) {
            pthread_cond_wait(&cond1, &mutex);

        }
        sleep(1);
        printf("################thread_task1 i: %i\n", i);
        i = 0;

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond0);

    }
}

int main() {
    pthread_t thread0;
    pthread_t thread1;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond0, NULL);
    pthread_cond_init(&cond1, NULL);

    pthread_create(&thread0, NULL, thread_task0, NULL);
    pthread_create(&thread1, NULL, thread_task1, NULL);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond0);
    pthread_cond_destroy(&cond1);
    return 0;
}