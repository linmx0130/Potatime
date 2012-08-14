#include <QtGui>
#include "Main.h"
#include "sysapi.h"
#include <fstream>
#include <iostream>
int __status=-1;
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
	mainlayout=new QHBoxLayout;
	rightlayout=new QVBoxLayout;
	leftlayout=new QVBoxLayout;
	StopWatch=new pQDigitStopWatch;
	ControlButton=new QPushButton("Start");
	Quit=new QPushButton("Quit");
	TastlistView=new QListWidget;
	nowTask=new QLabel("No task is be chosen");

	StopWatch->setFormat("<font size=7 color=\"red\">%m%:%s%</font>");
	StopWatch->reset();
	rightlayout->addWidget(StopWatch);
	rightlayout->addWidget(ControlButton);
	rightlayout->addWidget(Quit);
	leftlayout->addWidget(TastlistView);
	leftlayout->addWidget(nowTask);
	connect(Quit,SIGNAL(clicked()),this,SLOT(Quit_Click()));
	connect(ControlButton,SIGNAL(clicked()),this,SLOT(ControlButton_Click()));
	mainlayout->addLayout(leftlayout);
	mainlayout->addLayout(rightlayout);
	setLayout(mainlayout);
	loadFile();
}
void potatime::ControlButton_Click()
{
	if (__status==-1)
	{

	}
	if (__status==0)
	{
		this->StopWatch->start(1000);
		__status=1;
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
