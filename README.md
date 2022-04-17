# TinyMuduo

* v1 基本网络编程模型
* v2 增加 Tcpserver 类
* v3 增加 Channel、EventLoop, 回调机制
* v4 增加 Acceptor 类
* v5 增加 TcpConnection 类
	- 只需定义 TcpConnection 里的 handle* 函数即可实现一个简单的应用程序，如 echo 等
* v6 增加 Epoll 类
* v7 增加 Buffer 类
* v8 增加 onWriteCompleteCallback，eventfd
	- onWriteCompleteCallback 用于当 TcpConnection 输出缓冲区中的数据发送出去之后，通知库的使用者
	- eventfd. 
		1. 使网络库内部在单线程的情况下具备异步处理事件的能力
		2. 其他线程能够唤醒 IO 线程，处理一些简单的事件, 比如完成某个用户任务回调