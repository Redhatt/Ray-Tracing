#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "globals.h"

struct ThreadData
{
    int id, b, a, w, h, width, height;
    Camera *camera;
    ShapeSet *shapes;
    vector<vector<string>> *v;
};

struct Task
{
    int id;
    ThreadData *data;
    void *(*f)(void *args);
};

inline void *executeTask(Task *task)
{
    void *r = (*task->f)(task->data);
    cout<<"Task id: "<< task->id<<" has been executed.\n";
    return r;
}

queue<Task*> taskQueue;
pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void submitTask(Task *task)
{
    pthread_mutex_lock(&mutexQueue);
    taskQueue.push(task);
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
    // cout<<"Task id: "<<task->id<<" submitted. Current queue size: "<<taskQueue.size()<<endl;
}

void *startThread(void *args)
{
    int *id = (int *) args;
    while (!taskQueue.empty()) {
        Task *task;

        pthread_mutex_lock(&mutexQueue);

        // if (taskQueue.empty()) break;

        // while (taskQueue.empty()) {
        //     pthread_cond_wait(&condQueue, &mutexQueue);
        // }

        task = taskQueue.front();
        taskQueue.pop();

        pthread_mutex_unlock(&mutexQueue);

        // cout<<"Task id: "<<task->id<<" aquired on thread id: "<<*id<<". Starting execution.\n";
        executeTask(task);
    }
    return NULL;
}

#endif