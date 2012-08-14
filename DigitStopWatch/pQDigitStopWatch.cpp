/*
 Copyright (c) 2012 Sweetdumplings <linmx0130@163.com>
 All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#include <QtGui>
#include "pQDigitStopWatch.h"

pQDigitStopWatch::pQDigitStopWatch(QWidget *parent):
		QWidget(parent)
{
	hour=minute=second=msec=0;
	alarmmark=0;
	view=new QLabel;
	timer=new QTimer(this);
	format="%h%:%m%:%s%:%ms%";
	connect(timer,SIGNAL(timeout()),this,SLOT(_timeout_doing()));
	connect(timer,SIGNAL(timeout()),this,SIGNAL(timeout()));
	redrawview();
	QHBoxLayout *mainlayout=new QHBoxLayout;
	mainlayout->addWidget(view);
	setLayout(mainlayout);
	QSizePolicy forme(QSizePolicy::Preferred,QSizePolicy::Preferred);
	this->setSizePolicy(forme);
	view->setSizePolicy(forme);
}
void pQDigitStopWatch::start(int msec){
	this->timer->start(msec);
};
void pQDigitStopWatch::reset(){
	hour=minute=second=msec=0;
	this->stop();
	redrawview();
}
void pQDigitStopWatch::stop(){
	this->timer->stop();
}
int pQDigitStopWatch::interval(){
	return this->timer->interval();
}
void pQDigitStopWatch::_timeout_doing(){
	msec+=this->interval();
	if (msec>=1000){
		second+=msec/1000;
		msec%=1000;
	}
	if (second>=60){
		minute+=second/60;
		second%=60;
	}
	if (minute>=60){
		hour+=minute/60;
	}
	redrawview();
	if (alarmmark) {
	//	if (QTime(hour,minute,second,msec)==alarmrec){
		if ((alarmrec.hour()==this->hour)&&
			(alarmrec.minute()==this->minute)&&
			(alarmrec.second()==this->second)&&
			(alarmrec.msec()==this->msec)){
			alarmmark=0;
			emit alarm();
		}
	}
}
void pQDigitStopWatch::redrawview(){
	char twodig[3];
	twodig[2]='\0';
	QString hour_s,minute_s,second_s,msec_s;
	twodig[0]=hour/10+'0';
	twodig[1]=hour%10+'0';
	hour_s=twodig;
	twodig[0]=minute/10+'0';
	twodig[1]=minute%10+'0';
	minute_s=twodig;
	twodig[0]=second/10+'0';
	twodig[1]=second%10+'0';
	second_s=twodig;
	twodig[1]=(msec/10)%10+'0';
	twodig[0]=msec/100+'0';
	msec_s=twodig;
	QString newview=format;
	if (format.indexOf("%h%")!=-1) newview.replace("%h%",hour_s);
	if (format.indexOf("%m%")!=-1) newview.replace("%m%",minute_s);
	if (format.indexOf("%s%")!=-1) newview.replace("%s%",second_s);
	if (format.indexOf("%ms%")!=-1) newview.replace("%ms%",msec_s);
	view->setText(newview);
}
void pQDigitStopWatch::setFormat(QString formatstring){
	if (this->isActive()) return ; 
	this->format=formatstring;
}
bool pQDigitStopWatch::isActive(){
	return this->timer->isActive();
}
void pQDigitStopWatch::setInterval(int msec){
	this->setInterval(msec);
}
QTime pQDigitStopWatch::currentTime(){
	return QTime(this->hour,this->minute,this->second,this->msec);
}
void pQDigitStopWatch::alarmAtTime(QTime time){
	this->alarmmark=1;
	this->alarmrec=time;
}
void pQDigitStopWatch::setTime(QTime time){
	if (this->isActive()) return;
	this->hour=time.hour();
	this->minute=time.minute();
	this->second=time.second();
	this->msec=time.msec();
	redrawview();
};
