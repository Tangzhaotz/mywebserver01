//创建线程同步机制
#ifndef LOCKER_H
#define LOCKER_H

#include<exception>
#include<pthread.h>
#include<semaphore.h>

//互斥锁类创建
class locker
{
public:
    locker()
    {
        if(pthread_mutex_init(&m_mutex,NULL)!=0)
        {
            throw std::exception();  //初始化不成功就抛出异常
        }
    }

    ~locker()  //析构函数
    {
        pthread_mutex_destroy(&m_mutex);  //删除互斥量
    }

    bool lock()
    {
        return pthread_mutex_lock(&m_mutex)==0;  //上锁
    }

    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex)==0;  //解锁
    }

    pthread_mutex_t *get()  //获取互斥量
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;  //定义互斥量
};


//条件变量类
class cond
{
public:
    cond()
    {
        if(pthread_cond_init(&m_cond,NULL)!=0)
        {
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_cond_destroy(&m_cond);  //删除条件变量
    }

    bool wait(pthread_mutex_t *m_mutex)
    {
        int ret =0;
        ret = pthread_cond_wait(&m_cond,m_mutex);
        return ret==0;
    }

    bool timewait(pthread_mutex_t *m_mutex,struct timespec t)
    {
        int ret =0;
        ret = pthread_cond_timedwait(&m_cond,m_mutex,&t);
        return ret==0;
    }

    bool signal()
    {
        return pthread_cond_signal(&m_cond)==0;
    }

    bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond)==0;  //通知所有的线程
    }

private:
    pthread_cond_t m_cond;  //定义条件变量
};


//信号量类,主要用于线程池中任务队列的
class sem
{
public:
    sem()
    {
        if(sem_init(&m_sem,0,0)!=0)
        {
            throw std::exception();
        }
    }

    ~sem()
    {
        sem_destroy(&m_sem);
    }

    //等待信号量
    bool wait()
    {
        return sem_wait(&m_sem)==0;
    }

    //增加信号量
    bool post()
    {
        return sem_post(&m_sem)==0;
    }

private:
    sem_t m_sem;  //定义信号量
};


#endif