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
	//启动线程,但是没有启动线程处理函数
	thread->start();
	myThread->setFlag(false);
	//不能直接调用线程处理函数,
	//直接调用,导致线程处理函数和主线程是在同一个线程
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