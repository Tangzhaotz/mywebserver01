# mywebserver01
## 项目的主要内容：实现一个web服务器项目
## 主要的原理和实现的步骤：是一个get+epoll下同步模拟的proactor模式实现并发服务器
1、创建线程同步机制：
  主要原理：互斥锁、条件变量、信号量
 2、线程池的实现：根据电脑的配置设置线程池的数量，因为是用同步模拟proactor模式，所以线程池中主要包括的是一个线程的
 数组和一个任务队列，信号量和互斥锁就是用来实现任务队列的操作，主线程根据io获取的数据全部一次性读取出来，再将数据包
 装成一个个任务放入任务队列中，在线程池中的子线程被唤醒来对任务进行处理
 ## 服务器网络编程基本框架
 ![image](https://user-images.githubusercontent.com/53050401/125061816-4fbcc700-e0e0-11eb-8d1b-818583422134.png)
## 工作流程
![image](https://user-images.githubusercontent.com/53050401/125061872-5f3c1000-e0e0-11eb-9804-53c10a05b781.png)
## 实现
![image](https://user-images.githubusercontent.com/53050401/125061935-6e22c280-e0e0-11eb-87be-598167a8c758.png)
![image](https://user-images.githubusercontent.com/53050401/125062115-a2967e80-e0e0-11eb-90ce-9021235a4339.png)
## 测试并发量
![image](https://user-images.githubusercontent.com/53050401/125062430-f4d79f80-e0e0-11eb-8b2c-fadf99e1a400.png)
![image](https://user-images.githubusercontent.com/53050401/125062451-fa34ea00-e0e0-11eb-8c92-f7b2314dfe7b.png)

 
