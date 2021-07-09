//线程池的实现
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include "locker.h"


//创建线程池类，定义模板是因为代码复用，模板参数T是任务类
template<typename T>  //定义模板
class threadpool
{
public:
    threadpool(int thread_number=8,int max_requests=10000);  //两个参数分别表示线程池中线程的数量和请求队列中最多允许的、等待处理的请求的数量
    ~threadpool();
    bool append(T * request);  //往线程池中添加任务的函数 

private:
    //工作线程运行的函数，他不断从工作队列中取出任务并执行之
    static void *worker(void *arg);
    void run();

private:
    //线程池内的属性
    int m_thread_number;  //线程的数量

    //用来装线程的数组，大小为线程的个数
    pthread_t * m_threads;

    //请求队列中最多允许的请求的数量
    int m_max_requests;

    //请求队列
    std::list<T *>m_workqueue;

    //保护请求队列的互斥锁
    locker m_queuelocker;  //是一个对象

    //信号量用来标记是否需要任务需要处理
    sem m_queuestat;
    
    //是否需要结束线程
    bool m_stop;
    
};

template<typename T>
//构造函数，并通过列表初始化参数
threadpool<T>::threadpool(int thread_number,int max_requests):m_thread_number(thread_number),m_max_requests(max_requests),m_stop(false),m_threads(NULL)   //
{
    if((thread_number <=0) || max_requests <=0)  //线程池中没有线程或者请求允许的数量小于0
    {
        throw std::exception();  //抛出异常
    }

    //创建容纳线程的数组
    m_threads=new pthread_t[m_thread_number];
    if(!m_threads)
    {
        throw std::exception();
    }

    //创建thread_number个线程，并将其设置为脱离线程
    for(int i=0;i<thread_number;i++)
    {
        printf("create the %dth thread\n",i);
        if(pthread_create(m_threads+i,NULL,worker,this)!=0)  //判断线程是否创建成功
        {
            delete [] m_threads;  //创建失败的话就删除数组
            throw std::exception();
        }

        //设置线程脱离
        if(pthread_detach(m_threads[i]))
        {
            delete [] m_threads;  //删除数组
            throw std::exception();
        }
    }

}



//析构函数的类外实现
template<typename T>
threadpool<T>::~threadpool()
{
    delete [] m_threads;  //删除数组
    m_stop=true;  //将线程设置为停止
}


//向工作任务中添加任务
template<typename T>
bool threadpool<T>::append(T* request)
{
    //因为所有的线程都需要到工作队列中取出任务，所以工作队列的操作应该加互斥锁
    m_queuelocker.lock();  //先加锁
    if(m_workqueue.size() > m_max_requests)  //表示工作队列中的数据已经满了，不能添加任务
    {
        m_queuelocker.unlock(); //解锁
        return false;
    }
    m_workqueue.push_back(request);  //往工作队列中添加一个任务
    m_queuelocker.unlock();  //解锁
    m_queuestat.post();  //信号量加一
    return true;
}


//工作线程工作的函数
template<typename T>
void* threadpool< T >::worker(void *arg)
{
    threadpool * pool =(threadpool *)arg;  //将传入的参数进行类型转换
    pool->run();
    return pool;
}

//定义run函数，从任队列中取出任务
template<typename T>
void threadpool< T >::run()
{
    while(!m_stop)  //只要线程不停止就一直循环
    {
        m_queuestat.wait();  //信号量等待
        m_queuelocker.lock();  //上锁
        if(m_workqueue.empty())   //工作队列为空
        {
            m_queuelocker.unlock();
            continue;
        }

        T * request = m_workqueue.front();   //取出第一个任务
        m_workqueue.pop_front();  //将取出的任务弹出
        m_queuelocker.unlock();   //解锁
        if(!request)  //任务异常
        {
            continue;
        }
        request->process();  //处理任务的函数
    }
}

#endif