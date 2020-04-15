#include "MyThread.h"
#include <string>
MyThread::MyThread(QObject *parent)
	: QObject(parent)
{
	SOCKET socketSrc = INVALID_SOCKET;
	SOCKADDR_IN addrSrc = {};
	SOCKADDR_IN addrClient = {};
	FD_ZERO(&allSocket);//清零
	FD_ZERO(&sendSocket);//清零
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
		emit send_showDlgText(QString::fromLocal8Bit("ERROR:WSAStartup失败!"));
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		emit send_showDlgText(QString::fromLocal8Bit("ERROE:WSADATA版本不一致!"));
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
		emit send_showDlgText(QString::fromLocal8Bit("IP绑定失败!"));
		return;
	}
	else
	{
		emit send_showDlgText(QString::fromLocal8Bit("IP绑定成功!"));
	}
	listen(socketSrc, 10);//最多监听10个链接
	FD_ZERO(&allSocket);//清零
	FD_SET(socketSrc, &allSocket);//把服务器装进去
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
			//有响应
			for (u_int i = 0; i < tempSocket.fd_count; i++)
			{
				if (tempSocket.fd_array[i] == socketSrc)
				{
					int len = sizeof(SOCKADDR);
					SOCKET socketClient = accept(socketSrc, (SOCKADDR*)&addrClient, &len);

					if (socketClient == INVALID_SOCKET)
					{
						emit send_showDlgText(QString::fromLocal8Bit("新客户端连接失败!"));
						continue;
					}
					else
					{
						char newBufofClient[100];
						FD_SET(socketClient, &allSocket);
						sprintf(newBufofClient, "新客户端SOCKET: %d\tIP:%s加入服务器!", (int)socketClient, inet_ntoa(addrClient.sin_addr));
						emit send_showDlgText(QString::fromLocal8Bit(newBufofClient));//+ );//:%d\tIP:加入\n
					}

				}
				else {
					int nRecv = recv(tempSocket.fd_array[i], recvBuf, sizeof(recvBuf), 0);

					{if (nRecv == 0 || nRecv == -1)
					{//客户端下线
						char BufofClient[100];
					    SOCKET tempSock = tempSocket.fd_array[i];
						FD_CLR(tempSocket.fd_array[i], &allSocket);
						closesocket(tempSock);
						sprintf(BufofClient, "客户端SOCKET: %d\tIP:%s已退出!", (int)tempSocket.fd_array[i], inet_ntoa(addrClient.sin_addr));

						emit send_showDlgText(QString::fromLocal8Bit(BufofClient));
					}
					else if (nRecv > 0)
					{//接收到消息
						char BufofClient[100];
						for (int i = 0; i < allSocket.fd_count; i++)
						{
							send(allSocket.fd_array[i], recvBuf, strlen(recvBuf) + 1, 0);
						}
						sprintf(BufofClient, "SOCKET: %d说:%s", (int)tempSocket.fd_array[i], recvBuf);
						emit send_showDlgText(QString::fromLocal8Bit(BufofClient));
					}
					else {//系统报错
						GetLastError();
					}
					}
				}
			}
		}
		else
		{
			//发生错误了
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