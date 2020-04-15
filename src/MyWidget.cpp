#include "MyWidget.h"
#include <QThread>
MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("MySrc");
	myThread = new MyThread;
	thread = new QThread;
	myThread->moveToThread(thread);
	connect(this, &MyWidget::startThread, myThread, &MyThread::runSocket);
	connect(ui.pb_start, &QPushButton::clicked, this, &MyWidget::startMyThread);
	connect(ui.pb_stop, &QPushButton::clicked, this, &MyWidget::stopMyThread);
	connect(myThread, &MyThread::send_showDlgText, this, &MyWidget::recvShowDlgText);
	connect(ui.te_showLog, &QTextEdit::textChanged, ui.te_showLog, [=]()
	{  ui.te_showLog->moveCursor(QTextCursor::End);
	});
}
void MyWidget::startMyThread()
{
	//�����߳�,����û�������̴߳�����
	thread->start();
	myThread->setFlag(false);
	//����ֱ�ӵ����̴߳�����,
	//ֱ�ӵ���,�����̴߳����������߳�����ͬһ���߳�
	//myT->timeout();
	emit startThread();
}
void MyWidget::stopMyThread()
{
	myThread->setFlag(true);
	thread->quit();
	thread->wait();
}
void MyWidget::recvShowDlgText(QString str)
{
	QString oldStrt = this->ui.te_showLog->toPlainText();
	if (oldStrt != "")
		str = oldStrt + '\n' + str;
	this->ui.te_showLog->setText(str);
}