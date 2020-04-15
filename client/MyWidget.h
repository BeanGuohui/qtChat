#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyWidget.h"
#include "MyThread.h"
#include <QThread>
#include <QString>

class MyWidget : public QWidget
{
	Q_OBJECT

public:
	MyWidget(QWidget *parent = Q_NULLPTR);
	void startMyThread();
	void stopMyThread();
signals:
	void sendLeMsg2();
	void startThread();
	
public slots:
	void recvShowDlgText(QString str);//接收线程发来的数据
	//发送消息模块
	void sendLeMsg();
private:
	Ui::MyWidgetClass ui;
	MyThread *myThread;
	QThread *thread;
	QString strSend;
};
