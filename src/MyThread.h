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


//�Լ�����ĺ���
public:
	bool wsaIsStart();//��ʼ��wsadata�Ƿ�ɹ�
	void runSocket();//�����������
	void recvOrSend();//������Ϣ������
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
