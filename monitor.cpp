//монитор с двумя потоками: потоком-поставщиком и потоком-потребителем 
//лабораторная по операционным системам, полное описание - https://marigostra.ru/materials/oslab.html
//запускать на UNIX-подобных ос

#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool ready = 0;

void* producer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        if (ready == 1) {
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = 1;
        std::cout << "sent\n";
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
        sleep(1); 
    }
    return nullptr;
}

void* consumer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        while (ready == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        ready = 0;
        std::cout << "received\n";
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);
    pthread_join(producerThread, nullptr);

    return 0;
}
