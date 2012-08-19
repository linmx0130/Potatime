#include <QtGui>
#include "Main.h"
#include "sysapi.h"
#include <fstream>
#include <iostream>
#include <algorithm>
int __status=0;
#define POTA_WAIT 0
#define POTA_WORK 1
#define POTA_REST 2

std::vector<TaskNode> tasklist;
void potatime::Quit_Click()
{
	saveFile();
	Sysapi::deleteLock();
	Sysapi::Quit(0);
}
std::string int2string(int d)
{
	if (d==0)
	{
		return "0";
	}
	std::string ret;
	while (d)
	{
		char c=(d%10+'0');
		ret+=c;
		d/=10;
	}
	std::reverse(ret.begin(),ret.end());
	return ret;
}
potatime::potatime(QWidget *parent)
		:QWidget(parent)
{
	chosen=-1;
	mainlayout=new QHBoxLayout;
	rightlayout=new QVBoxLayout;
	leftlayout=new QVBoxLayout;
	StopWatch=new pQDigitStopWatch;
	ControlButton=new QPushButton("Start");
	Quit=new QPushButton("Quit");
	TasklistView=new QListWidget;
	nowTask=new QLabel("No task is chosen.");
	listMenu=new QMenu();
	addTaskAction=new QAction("Add task",0);
	editTaskAction=new QAction("Edit task",0);
	removeTaskAction=new QAction("Remove task",0);

	TaskSuccessTip=new QAction("",0);
	TaskFailTip=new QAction("",0);
	listMenu->addAction(addTaskAction);
	listMenu->addAction(editTaskAction);
	listMenu->addAction(removeTaskAction);
	listMenu->addAction(TaskSuccessTip);
	listMenu->addAction(TaskFailTip);

	TasklistView->setContextMenuPolicy(Qt::CustomContextMenu);
	TaskSuccessTip->setEnabled(0);
	TaskFailTip->setEnabled(0);
	connect(TasklistView,SIGNAL(customContextMenuRequested(const QPoint&)),
			this,SLOT(TasklistView_ContextMenu(const QPoint&)));
	StopWatch->setFormat("<font size=7 color=\"red\">%m%:%s%</font>");
	StopWatch->reset();
	rightlayout->addWidget(StopWatch);
	rightlayout->addWidget(ControlButton);
	rightlayout->addWidget(Quit);
	leftlayout->addWidget(TasklistView);
	leftlayout->addWidget(nowTask);
	
	connect(Quit,SIGNAL(clicked()),this,SLOT(Quit_Click()));
	connect(ControlButton,SIGNAL(clicked()),this,SLOT(ControlButton_Click()));
	connect(TasklistView,SIGNAL(currentRowChanged(int)),this,SLOT(TasklistView_Click(int)));
	connect(StopWatch,SIGNAL(timeout()),this,SLOT(Wakefile_Scan()));
	connect(addTaskAction,SIGNAL(triggered()),this,SLOT(addTaskAction_Click()));
	connect(editTaskAction,SIGNAL(triggered()),this,SLOT(editTaskAction_Click()));
	connect(removeTaskAction,SIGNAL(triggered()),this,SLOT(removeTaskAction_Click()));
	mainlayout->addLayout(leftlayout);
	mainlayout->addLayout(rightlayout);
	setLayout(mainlayout);
	loadFile();
}
void potatime::ControlButton_Click()
{
	if (__status==-1)
	{
		return;
	}
	if (__status==POTA_WAIT)
	{
		this->TasklistView->setEnabled(0);
		this->StopWatch->start(1000);
		__status=POTA_WORK;
		this->ControlButton->setText("Give up");
		this->nowTask->setText("Working on the task!");
		QTime _25min(0,25,0);
		this->StopWatch->alarmAtTime(_25min);
		connect(StopWatch,SIGNAL(alarm()),this,SLOT(Alarm_Slot()));
		return ;
	}
	if (__status==POTA_WORK)
	{
		__status=POTA_WAIT;
		this->ControlButton->setText("Start");
		this->StopWatch->stop();
		this->StopWatch->reset();
		this->nowTask->setText("You have give up the task!");
		tasklist[this->chosen].fail++;
		this->TasklistView->setEnabled(1);
		return ;
	}
	if (__status==POTA_REST)
	{
		endRest();
	}
}
void potatime::loadFile()
{
	std::string filename=Sysapi::getHomePath();
	filename+="/.potalist";
	std::ifstream fin(filename.c_str());
	std::string buf;
	if (!fin)
	{
		fin.close();
		std::ofstream fout(filename.c_str());
		fout.close();
		fin.open(filename.c_str());
	}
	while (fin >>buf)
	{
		TaskNode willi;
		willi.name=buf;
		fin>>willi.fail;
		fin>>willi.success;
		tasklist.push_back(willi);
		TasklistView->addItem(willi.name.c_str());
	}
	fin.close();
}
void potatime::saveFile()
{
	std::string filename=Sysapi::getHomePath();
	filename+="/.potalist";
	std::ofstream fout(filename.c_str());
	for (size_t i=0;i!=tasklist.size();++i)
	{
		fout<<tasklist[i].name<<std::endl
			<<tasklist[i].fail<<std::endl
			<<tasklist[i].success<<std::endl;
	}
	fout.close();
}
void potatime::TasklistView_Click(int choose)
{
	this->chosen=choose;
	if (choose!=-1)
	{
		std::string newTasktip("\"");
		newTasktip+=tasklist[choose].name;
		newTasktip+="\" is chosen!";
		nowTask->setText(newTasktip.c_str());
		__status=POTA_WAIT;
	}
	else 
	{
		nowTask->setText("No task is chosen.");
		__status=-1;
	}
}
void potatime::closeEvent(QCloseEvent *event)
{
	if (__status==POTA_WORK)
	{
		event->ignore();
		this->hide();
	}
	else
	{
		Quit_Click();
	}
}
void potatime::Wakefile_Scan()
{
	if (Sysapi::wakeFileExist())
	{
		this->show();
	}
}
void potatime::startRest()
{
	rested++;
	QApplication::beep();
	this->StopWatch->stop();
	this->StopWatch->reset();
	tasklist[this->chosen].success++;
	__status=POTA_REST;
	this->show();
	this->ControlButton->setText("Stop Rest");
	if (rested==4)
	{
		rested=0;
		nowTask->setText("You will have a long rest!(15min)");
		QTime _15min(0,15);
		this->StopWatch->alarmAtTime(_15min);
	}
	else 
	{
		nowTask->setText("You will have a short rest!(5min)");
		QTime _5min(0,5);
		this->StopWatch->alarmAtTime(_5min);
	}
	this->StopWatch->start(1000);
}
void potatime::endRest()
{
	this->show();
	this->StopWatch->stop();
	this->StopWatch->reset();
	this->ControlButton->setText("Start");
	__status=POTA_WAIT;
	nowTask->setText("Rest is end! Please choose a task.");
	this->TasklistView->setEnabled(1);
}
void potatime::Alarm_Slot()
{
	if (__status==POTA_WORK)
	{
		startRest();
		return;
	}
	if (__status==POTA_REST)
	{
		endRest();
		return;
	}
}
int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	Sysapi::creatLock();
	
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	potatime *MainWin=new potatime;
	MainWin->show();
	app.setQuitOnLastWindowClosed(0);
	return app.exec();
}
void potatime::TasklistView_ContextMenu(const QPoint& pos)
{
	if (TasklistView->itemAt(pos)!=NULL)
	{
		editTaskAction->setEnabled(1);
		removeTaskAction->setEnabled(1);
		TaskSuccessTip->setVisible(1);
		TaskFailTip->setVisible(1);
		std::string buf;
		buf="Success:";
		buf+=int2string(tasklist[chosen].success);
		TaskSuccessTip->setText(buf.c_str());
		buf="Fail:";
		buf+=int2string(tasklist[chosen].fail);
		TaskFailTip->setText(buf.c_str());
	}
	else 
	{
		editTaskAction->setEnabled(0);
		removeTaskAction->setEnabled(0);
		TaskSuccessTip->setVisible(0);
		TaskFailTip->setVisible(0);
	}
	this->listMenu->exec(QCursor::pos());
}
void potatime::addTaskAction_Click()
{
	bool trueadd;
	QString taskname;
	taskname=QInputDialog::getText(this,"Add Task",
					"Please input the name of the task",
					QLineEdit::Normal,
					"",&trueadd);
	if (trueadd&&!taskname.isEmpty())
	{
		TasklistView->addItem(taskname);
		TaskNode newn;
		newn.name=taskname.toStdString();
		newn.fail=newn.success=0;
		tasklist.push_back(newn);
	}
}
void potatime::editTaskAction_Click()
{
	bool truechange;
	QString taskname;
	taskname=QInputDialog::getText(this,"Edit Task",
					"Please input the new name of the task",
					QLineEdit::Normal,
					TasklistView->item(this->chosen)->text(),
					&truechange);
	if (truechange&&!taskname.isEmpty())
	{
		TasklistView->item(this->chosen)->setText(taskname);
		tasklist[this->chosen].name=taskname.toStdString();
	}

}
void potatime::removeTaskAction_Click()
{
	int ans=QMessageBox::warning(this,"Delete task",
					"This action will changed the result of the analyze.\n"
					"Do you really want to delete it?",
					QMessageBox::Yes|QMessageBox::Cancel);
	if (ans==QMessageBox::Yes)
	{
		std::vector<TaskNode>::iterator iter=tasklist.begin()+this->chosen;
		tasklist.erase(iter);
		QListWidgetItem* willd=TasklistView->item(this->chosen);
		TasklistView->removeItemWidget(willd);
		delete willd;
		this->TasklistView_Click(TasklistView->currentRow());
	}
}
