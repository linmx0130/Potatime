#ifndef MAIN_H
#define MAIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QCloseEvent>
#include <vector>
#include <string>
#include "pQDigitStopWatch.h"
struct TaskNode
{
	std::string name;
	int fail;
};
extern std::vector<TaskNode> tasklist;
class potatime:public QWidget
{
Q_OBJECT
private slots:
	void Quit_Click();
	void ControlButton_Click();
	void TasklistView_Click(int);
	void Wakefile_Scan();
	void Alarm_Slot();
signals:

private:
	QHBoxLayout *mainlayout;
	QVBoxLayout *rightlayout,*leftlayout;
	pQDigitStopWatch *StopWatch;
	QPushButton *ControlButton,*Quit;
	QListWidget *TastlistView;
	QLabel *nowTask;
	void loadFile();
	void saveFile();
	void startRest();
	void endRest();
	int chosen,rested;
protected:
	void closeEvent(QCloseEvent *event);
public:
	potatime(QWidget *parent=0);

};

#endif
