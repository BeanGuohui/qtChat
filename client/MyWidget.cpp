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
	connect(ui.pb_connectSrc, &QPushButton::clicked, this, &MyWidget::startMyThread);//���Ӱ�ť�û�
	//connect(this, &QWidget::destroyed, this, &MyWidget::ondestroy);//��������
	connect(this, &MyWidget::startThread, myThread, &MyThread::runSock);//�����߳�
	connect(ui.td_showchat, &QTextEdit::textChanged, ui.td_showchat, [=]()
	{  ui.td_showchat->moveCursor(QTextCursor::End);
	});//�����촰�ڸ�����Ϣ���·���
	  
	   
	   //�¼��źͲۻ���,����������Ϣ
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