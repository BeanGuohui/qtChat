#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <QObject>
#pragma comment (lib,"ws2_32.lib")

class MyThread : public QObject
{
	Q_OBJECT

public:
	MyThread(QObject *parent = 0);
	~MyThread();
	//自己定义的函数
public:
	bool wsaIsCreat();
	void runSock();
	void recvInformation();
	void setrecvIsStop(bool flag = true);
	void setInformationSend(QString str);
	void sendingFun(QString str);
	void setSendIsStop(bool flag = true);
signals:
	void send_showDlgText(QString str);
signals:	
	void sendFun(QString);

private:
	SOCKET socketClient;
	fd_set allsockets;
	SOCKADDR_IN addrClient;
	char recvBuf[100];
	WSADATA wsaData;
	bool sendIsStop;
	bool sendIsStopOut;
	bool recvIsStop;
	QString informationSend;
};
