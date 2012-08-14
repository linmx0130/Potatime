#include <QtGui>
#include "Main.h"
#include "sysapi.h"
#include <fstream>
#include <iostream>
int __status=0;
std::vector<TaskNode> tasklist;
void potatime::Quit_Click()
{
	saveFile();
	Sysapi::deleteLock();
	Sysapi::Quit(0);
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
	TastlistView=new QListWidget;
	nowTask=new QLabel("No task is chosen.");

	StopWatch->setFormat("<font size=7 color=\"red\">%m%:%s%</font>");
	StopWatch->reset();
	rightlayout->addWidget(StopWatch);
	rightlayout->addWidget(ControlButton);
	rightlayout->addWidget(Quit);
	leftlayout->addWidget(TastlistView);
	leftlayout->addWidget(nowTask);
	connect(Quit,SIGNAL(clicked()),this,SLOT(Quit_Click()));
	connect(ControlButton,SIGNAL(clicked()),this,SLOT(ControlButton_Click()));
	connect(TastlistView,SIGNAL(currentRowChanged(int)),this,SLOT(TasklistView_Click(int)));
	connect(StopWatch,SIGNAL(timeout()),this,SLOT(Wakefile_Scan()));
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
	if (__status==0)
	{
		this->TastlistView->setEnabled(0);
		this->StopWatch->start(1000);
		__status=1;
		this->ControlButton->setText("Give up");
		return ;
	}
	if (__status==1)
	{
		__status=0;
		this->ControlButton->setText("Start");
		this->StopWatch->stop();
		this->StopWatch->reset();
		this->nowTask->setText("You have give up the task!");
		tasklist[this->chosen].fail++;
		this->TastlistView->setEnabled(1);
		return ;
	}
}
void potatime::loadFile()
{
	std::string filename=Sysapi::getHomePath();
	filename+="/.potalist";
	std::ifstream fin(filename.c_str());
	std::string buf;
	while (fin >>buf)
	{
		TaskNode willi;
		willi.name=buf;
		fin>>willi.fail;
		tasklist.push_back(willi);
		TastlistView->addItem(willi.name.c_str());
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
		fout<<tasklist[i].name<<std::endl<<tasklist[i].fail<<std::endl;
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
		__status=0;
	}
	else 
	{
		nowTask->setText("No task is chosen.");
		__status=-1;
	}
}
void potatime::closeEvent(QCloseEvent *event)
{
	if (__status==1)
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
