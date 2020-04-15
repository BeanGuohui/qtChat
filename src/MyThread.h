#pragma once
#define WIN32_LEAN_AND_MEAN
#include <QObject>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#pragma comment (lib,"ws2_32.lib")
class MyThread : public QObject
{
	Q_OBJECT

public:
	MyThread(QObject *parent = 0);
	~MyThread();


//自己定义的函数
public:
	bool wsaIsStart();//初始化wsadata是否成功
	void runSocket();//跑起这个程序
	void recvOrSend();//接收消息并发送
	void setFlag(bool flag = true);
signals:
	void  send_showDlgText(QString);

private:
	SOCKET socketSrc;
	SOCKADDR_IN addrSrc;
	WSADATA wsaData;
	SOCKADDR_IN addrClient;
	fd_set allSocket;
	fd_set sendSocket;
	bool isStop;
};
