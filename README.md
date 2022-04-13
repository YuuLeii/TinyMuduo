# TinyMuduo

* v1 基本网络编程模型
* v2 增加 Tcpserver 类
* v3 增加 Channel、EventLoop, 回调机制
* v4 增加 Acceptor 类
* v5 增加 TcpConnection 类
	- 只需定义 TcpConnection 里的 handle* 函数即可实现一个简单的应用程序，如 echo 等