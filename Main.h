#ifndef MAIN_H
#define MAIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
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
public:
	potatime(QWidget *parent=0);

};

#endif
