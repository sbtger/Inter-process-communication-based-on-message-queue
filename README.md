# 基于消息队列的进程间通信 Inter-process-communication-based-on-message-queue

利用消息队列的基本函数，实现Linux系统下进程A和进程B之间的消息收发。
- 进程A可以发送消息给进程B，并且进程B可以确认消息；
- 进程B可以发送消息给进程A，并且进程A可以确认消息；
- 当一方发出退出信息之后，双方进程退出。

The basic functions of the message queue are used to implement message sending and receiving between process A and process B in Linux OS.
- Process A can send a message to Process B, and Process B can acknowledge the message.
- Process B can send a message to Process A, and Process A can acknowledge the message.
- After one party sends out the exit message, both processes exit.
