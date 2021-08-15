# TCPOnlineGameServer
WINDOWS IOCP多线程TCP游戏联机服务器
===========================================================================

特点：

使用iocp高效处理并发数据

使用消费者 生产者模式（io线程只负责收取与简单处理数据（如解密）不负责处理业务逻辑，将简单处理后数据推送给业务队列中，业务线程将从队列中读取并处理数据）

使用无锁高效的多线程安全队列

使用hashmap进行客户端信息保存，查找读取客户端信息很高效（使用读写锁来保障数据安全处理）

数据进行简单加密传输

可自定义数据协议   

服务器结构
========================================================
![服务器结构](https://user-images.githubusercontent.com/60800578/129468779-25a470cb-f661-4f9e-bcbe-f0389a8636a8.png)


未来实现
=========================================================
利用信号量优化部分逻辑

使用预开辟内存，安全队列等技术 管理各种Socket等数据 更加高效与稳定

服务器日志系统


