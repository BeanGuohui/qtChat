#include "MyThread.h"
#include <string>
MyThread::MyThread(QObject *parent)
	: QObject(parent)
{
	SOCKET socketSrc = INVALID_SOCKET;
	SOCKADDR_IN addrSrc = {};
	SOCKADDR_IN addrClient = {};
	FD_ZERO(&allSocket);//����
	FD_ZERO(&sendSocket);//����
	isStop = false;
}

MyThread::~MyThread()
{
}
bool MyThread::wsaIsStart()
{
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		emit send_showDlgText(QString::fromLocal8Bit("ERROR:WSAStartupʧ��!"));
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		emit send_showDlgText(QString::fromLocal8Bit("ERROE:WSADATA�汾��һ��!"));
		return false;		
	}
	return true;

}

void MyThread::recvOrSend()
{
	SOCKET socketSrc = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrc;
	addrSrc.sin_family = AF_INET;
	addrSrc.sin_port = htons(2345);
	addrSrc.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(socketSrc, (SOCKADDR*)&addrSrc, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		int n = GetLastError();
		emit send_showDlgText(QString::fromLocal8Bit("IP��ʧ��!"));
		return;
	}
	else
	{
		emit send_showDlgText(QString::fromLocal8Bit("IP�󶨳ɹ�!"));
	}
	listen(socketSrc, 10);//������10������
	FD_ZERO(&allSocket);//����
	FD_SET(socketSrc, &allSocket);//�ѷ�����װ��ȥ
	while (isStop == false)
	{
		char recvBuf[500];
		fd_set tempSocket = allSocket;
		fd_set writeFds = allSocket;
		timeval st;
		st.tv_sec = 3;
		st.tv_usec = 0;
		int nRes = select(0, &tempSocket, &writeFds, NULL, &st);
		if (nRes == 0)
		{
			continue;
		}
		else if (nRes > 0)
		{
			//����Ӧ
			for (u_int i = 0; i < tempSocket.fd_count; i++)
			{
				if (tempSocket.fd_array[i] == socketSrc)
				{
					int len = sizeof(SOCKADDR);
					SOCKET socketClient = accept(socketSrc, (SOCKADDR*)&addrClient, &len);

					if (socketClient == INVALID_SOCKET)
					{
						emit send_showDlgText(QString::fromLocal8Bit("�¿ͻ�������ʧ��!"));
						continue;
					}
					else
					{
						char newBufofClient[100];
						FD_SET(socketClient, &allSocket);
						sprintf(newBufofClient, "�¿ͻ���SOCKET: %d\tIP:%s���������!", (int)socketClient, inet_ntoa(addrClient.sin_addr));
						emit send_showDlgText(QString::fromLocal8Bit(newBufofClient));//+ );//:%d\tIP:����\n
					}

				}
				else {
					int nRecv = recv(tempSocket.fd_array[i], recvBuf, sizeof(recvBuf), 0);

					{if (nRecv == 0 || nRecv == -1)
					{//�ͻ�������
						char BufofClient[100];
					    SOCKET tempSock = tempSocket.fd_array[i];
						FD_CLR(tempSocket.fd_array[i], &allSocket);
						closesocket(tempSock);
						sprintf(BufofClient, "�ͻ���SOCKET: %d\tIP:%s���˳�!", (int)tempSocket.fd_array[i], inet_ntoa(addrClient.sin_addr));

						emit send_showDlgText(QString::fromLocal8Bit(BufofClient));
					}
					else if (nRecv > 0)
					{//���յ���Ϣ
						char BufofClient[100];
						for (int i = 0; i < allSocket.fd_count; i++)
						{
							send(allSocket.fd_array[i], recvBuf, strlen(recvBuf) + 1, 0);
						}
						sprintf(BufofClient, "SOCKET: %d˵:%s", (int)tempSocket.fd_array[i], recvBuf);
						emit send_showDlgText(QString::fromLocal8Bit(BufofClient));
					}
					else {//ϵͳ����
						GetLastError();
					}
					}
				}
			}
		}
		else
		{
			//����������
		}

	}
	isStop = true;
	WSACleanup();
}
void MyThread::setFlag(bool flag)
{
	this->isStop = flag;
}
void MyThread::runSocket()
{
	if (wsaIsStart() == true)
	{
			recvOrSend();
	}

}