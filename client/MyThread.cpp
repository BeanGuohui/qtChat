#include "MyThread.h"
#include <WinSock2.h>
#include <QThread>
MyThread::MyThread(QObject *parent)
	: QObject(parent)
{

	SOCKET socketClient = INVALID_SOCKET;
	SOCKADDR_IN addrClient = {};
	char recvBuf[100] = {};
	WSADATA wsaData;
	sendIsStop = true;
	sendIsStopOut = false;
	recvIsStop = false;
}

MyThread::~MyThread()
{
}
bool MyThread::wsaIsCreat()
{
	
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		emit send_showDlgText(QString::fromLocal8Bit("ERROR:WSAStartupʧ��!"));
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		emit send_showDlgText(QString::fromLocal8Bit("ERROE:WSADATA�汾��һ��!"));
		WSACleanup();
		return false;
	}
	return true;
}


void MyThread::recvInformation()
{
	socketClient = socket(AF_INET, SOCK_STREAM, 0);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(2345);
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (0 == (::connect(socketClient, (SOCKADDR*)&addrClient, sizeof(SOCKADDR)))) {
		emit send_showDlgText(QString::fromLocal8Bit("���ӳɹ�!"));
	}
	else {
		emit send_showDlgText(QString::fromLocal8Bit("����ʧ��!"));
		WSACleanup();
		return;
	}
	FD_ZERO(&allsockets);
	FD_SET(socketClient, &allsockets);
	while (recvIsStop == false)
	{
		
		if (sendIsStop == false)
		{
			//informationSend.toLocal8Bit();
			sendingFun(informationSend);
			sendIsStop = true;
			informationSend = true;
		}
		fd_set readSockets = allsockets;
		struct timeval st;
		st.tv_sec = 3;
		st.tv_usec = 0;
		int nRes = select(0, &readSockets, NULL, NULL, &st);
		if (0 == nRes) //û����Ӧ��socket
		{
			continue;
		}
		else if (nRes > 0)
		{
			for (u_int i = 0; i < readSockets.fd_count; i++)
			{
				int nRecv = recv(readSockets.fd_array[i], recvBuf, sizeof(recvBuf), 0);

				if (nRecv > 0)
				{
					char tempBuf[200];
					sprintf(tempBuf, "SOCKET:%d������Ϣ:%s",(int)readSockets.fd_array[i], recvBuf);
					emit send_showDlgText(QString::fromLocal8Bit(tempBuf));
				}
				else {//ϵͳ����
					GetLastError();
				}
			}
			
		}
	}
	sendIsStop = true;
	WSACleanup();
}
void MyThread::runSock()
{
	if (true == wsaIsCreat())
	{
		recvInformation();
	}
}
void MyThread::setrecvIsStop(bool flage)
{
	this->recvIsStop = flage;
}
void MyThread::sendingFun(QString str)
{
	QByteArray ba = str.toLocal8Bit();
	char *sendBuf = ba.data();
	send(socketClient, sendBuf, strlen(sendBuf) + 1, 0);
}
void MyThread::setSendIsStop(bool flag)
{
	this->sendIsStop = flag;

}
void MyThread::setInformationSend(QString str)
{
	this->informationSend = str;
}