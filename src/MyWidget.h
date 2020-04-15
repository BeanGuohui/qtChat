#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyWidget.h"
#include "MyThread.h"
#include <QString>
class MyWidget : public QWidget
{
	Q_OBJECT

public:
	MyWidget(QWidget *parent = Q_NULLPTR);

	void startMyThread();
	void stopMyThread();
signals:
	void startThread();


public slots:

	void recvShowDlgText(QString str);//�����̷߳���������

private:
	Ui::MyWidgetClass ui;
	MyThread *myThread;
	QThread *thread;
};
