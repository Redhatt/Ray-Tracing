#include "globals.h"

bool END = false;
struct ThreadData
{
    int id, b, a, w, h, width, height;
    Camera *camera;
    ShapeSet *shapes;
    // vector<vector<string>> *v;
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
}

void *startThread(void *args)
{
    int *id = (int *) args;
    while (!END) {
        Task *task;

        pthread_mutex_lock(&mutexQueue);

        while (taskQueue.empty()) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue.front();
        taskQueue.pop();

        pthread_mutex_unlock(&mutexQueue);
        executeTask(task);
    }
    return NULL;
}