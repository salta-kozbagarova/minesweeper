#ifndef PLAYERDATA_H
#define PLAYERDATA_H
#pragma once

#include <string>
#include "timer.h"
#include <fstream>
#include <list>
using namespace std;

class playerData
{
	wstring playerName;
	timer t;
	string allData;
	list<wstring> listOfNames;
	list<wstring> listOfTimer;
	int level;
public:
	playerData(){}
	void SetPlayerName(wstring playerName)
	{
		this->playerName = playerName;
	}
	void SetTime(timer t)
	{
		this->t = t;
	}
	void SetLevel(int level)
	{
		this->level = level;
	}
	void SetData(wstring playerName, timer t)
	{
		this->playerName = playerName;
		this->t = t;
		wstring sec, min, hour;
		sec = t.GetSec();
		min = t.GetMin();
		hour = t.GetHour();
		string name(playerName.begin(), playerName.end());
		string strSec(sec.begin(), sec.end());
		string strMin(min.begin(), min.end());
		string strHour(hour.begin(), hour.end());
		allData.clear();
		allData += name;
		allData += ";";
		if (stoi(hour) < 10)allData += "0";
		allData += strHour;
		allData += ":";
		if (stoi(min) < 10)allData += "0";
		allData += strMin;
		allData += ":";
		if (stoi(sec) < 10)allData += "0";
		allData += strSec;
		allData += "@@";
	}
	wstring GetPlayerName()
	{
		return this->playerName;
	}
	wstring GetTime()
	{
		return t.GetTime();
	}
	list<wstring> GetListOfNames()
	{
		return this->listOfNames;
	}
	list<wstring> GetListOfTimer()
	{
		return this->listOfTimer;
	}
	
	ofstream& SaveToFile()
	{
		string path;
		if (level == 1) path = "playerDataEasy.csv";
		if (level == 2) path = "playerDataMedium.csv";
		if (level == 3) path = "playerDataDifficult.csv";
		ofstream oF(path, ios::app);
		oF << this->allData;
		oF.close();
		return oF;
	}
	ifstream& ReadFromFile()
	{
		string path;
		if (level == 1) path = "playerDataEasy.csv";
		if (level == 2) path = "playerDataMedium.csv";
		if (level == 3) path = "playerDataDifficult.csv";
		ifstream iF(path, ios::in);
		string ttmp, tmp, name, time;
		while (!iF.eof())
		{
			iF >> ttmp;
			tmp += ttmp;
		}
		while(tmp.find("@@") != -1)
		{
			size_t ind = tmp.find(";");
			name = tmp.substr(0, ind);
			wstring wtmp(name.begin(), name.end());
			listOfNames.push_back(wtmp);
			tmp = tmp.substr(ind + 1);
			ind = tmp.find("@@");
			time = tmp.substr(0, ind);
			wstring wtmp1(time.begin(), time.end());
			listOfTimer.push_back(wtmp1);
			tmp = tmp.substr(ind + 2);
		}
		iF.close();
		return iF;
	}
	void Clear()
	{
		this->listOfNames.clear();
		this->listOfTimer.clear();
	}
};


#endif