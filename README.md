# TCPOnlineGameServer
自定义V2版本  Actor模型 模块化编程
=========================================================
特点：

1.使用类似actor模型可以大大减少代码模块间的耦合度 更易维护与扩展。
网络处理部分也是一个特殊模块，默认使用epoll处理io数据

2.无需专门调用注册函数，new出来的模块对象自动注册

结构:
===

ModuleManager:  管理Module的类(全局单例) 

ModuleBase: 模块基类 写模块时继承该类 需要覆写Init(初始化运行函数),update(每循环运行),parseMessage(解析消息函数)。new出来的对象会自动注册到ModuleManager中进行管理。

ThreadManager：线程管理类 管理处理模块的线程 通过该类开启整个模块功能

ConsoleCmd: 控制台指令类  用于注册控制台指令



![QQ截图20220411165223](https://user-images.githubusercontent.com/60800578/162700685-fa832bbe-8ca5-43aa-871d-7bb5e66f1835.png)

![QQ截图20220411171206](https://user-images.githubusercontent.com/60800578/162704451-40054d3e-9f8f-488f-85b8-2d2560dfaf3f.png)

![QQ截图20220411183456](https://user-images.githubusercontent.com/60800578/162723114-4493eea8-2cab-4a86-9568-b963e7e1946a.png)




早期构思图
![Actor游戏服务器架构思路](https://user-images.githubusercontent.com/60800578/161270995-b96a6d8a-6488-479b-8222-21a9ec12e351.png)







自定义版本V1版 （WINDOWS IOCP多线程TCP自定义游戏联机服务器框架 配合DB服务器 ）
===========================================================================

特点：

使用iocp高效处理并发数据

使用消费者 生产者模式（io线程只负责收取与简单处理数据（如解密）不负责处理业务逻辑，将简单处理后数据推送给业务队列中，业务线程将从队列中读取并处理数据）

使用高效的多线程安全队列

使用哈希数组进行客户端玩家信息保存，查找读取客户端信息很高效

部分使用预开辟内存 内存池 减少new带来的消耗

数据进行简单加密传输

可自定义数据协议   （默认按照 ：头-大小-数据）

利用信号量唤醒业务线程 避免一直循环



v1版服务器结构


![TCP游戏服务器架构](https://user-images.githubusercontent.com/60800578/135739241-7277575f-26ab-4210-9521-9185c0da1b95.png)

=========================================================

当前已包含简单的登录 注册功能

配合DB服务器使用https://github.com/FutureXXH/DBSERVER





