#ifndef MAIN_H
#define MAIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <vector>
#include <string>
#include <QPoint>
#include "pQDigitStopWatch.h"
struct TaskNode
{
	std::string name;
	int fail,success;
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
	void TasklistView_ContextMenu(const QPoint &pos);
	void addTaskAction_Click();
	void editTaskAction_Click();
	void removeTaskAction_Click();
	void AnalysisButton_Click();
	void HelpButton_Click();
signals:

private:
	QHBoxLayout *mainlayout;
	QVBoxLayout *rightlayout,*leftlayout;
	pQDigitStopWatch *StopWatch;
	QPushButton *ControlButton,*Quit,*HelpButton,*AnalysisButton;
	QListWidget *TasklistView;
	QLabel *nowTask;
	QMenu *listMenu;
	QAction *addTaskAction,*editTaskAction,*TaskSuccessTip,*TaskFailTip,*removeTaskAction;
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
