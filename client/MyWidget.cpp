#include "MyWidget.h"
#include <QDebug>
MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("MySlient");
	myThread = new MyThread;
	thread = new QThread;
	myThread->moveToThread(thread);
	connect(myThread, &MyThread::send_showDlgText, this, &MyWidget::recvShowDlgText);
	connect(ui.pb_connectSrc, &QPushButton::clicked, this, &MyWidget::startMyThread);//链接按钮敲击
	//connect(this, &QWidget::destroyed, this, &MyWidget::ondestroy);//窗口销毁
	connect(this, &MyWidget::startThread, myThread, &MyThread::runSock);//启动线程
	connect(ui.td_showchat, &QTextEdit::textChanged, ui.td_showchat, [=]()
	{  ui.td_showchat->moveCursor(QTextCursor::End);
	});//让聊天窗口跟随信息往下翻动
	  
	   
	   //新家信和槽机制,用来发送消息
	connect(ui.pb_send, &QPushButton::clicked, this, &MyWidget::sendLeMsg);
	//connect(this, &MyWidget::sendLeMsg2, myThread, &MyThread::recvLeMsg2);
	connect(this, &MyWidget::sendLeMsg2, this, [=]() {
		qDebug() << "this is sendmessage";
		myThread->setInformationSend(strSend);
		myThread->setSendIsStop(false); });

	//connect(myThread, &MyThread::sendFun, myThread, [=]() {myThread->setSendIsStop(true); });

}
void MyWidget::sendLeMsg()
{
	strSend = ui.le_sendText->text();
	//strSend.toLocal8Bit();
	emit sendLeMsg2();
}
void MyWidget::startMyThread()
{
	thread->start();
	myThread->setrecvIsStop(false);
	emit startThread();
}
void MyWidget::stopMyThread()
{
	myThread->setrecvIsStop(true);
	thread->quit();
	thread->wait();
}
void MyWidget::recvShowDlgText(QString str)
{
	QString oldStrt = this->ui.td_showchat->toPlainText();
	if (oldStrt != "")
		str = oldStrt + '\n' + str;
	this->ui.td_showchat->setText(str);
}