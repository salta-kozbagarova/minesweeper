#ifndef TIMER_H
#define TIMER_H

#include <string>
using namespace std;

class timer
{
	int h, m, s;
public:
	timer(int h = 0, int m = 0, int s = 0)
	{
		this->h = h;
		this->m = m;
		this->s = s;
	}
	void Reset()
	{
		this->h = 0;
		this->m = 0;
		this->s = 0;
	}
	void AddHours(int c)
	{
		this->h += c;
		int tmp = this->h / 24;
		this->h = this->h - tmp * 24;
	}
	void AddMin(int c)
	{
		this->m += c;
		int tmp = this->m / 60;
		AddHours(tmp);
		this->m = this->m - tmp * 60;
	}
	void AddSec(int c)
	{
		this->s += c;
		int tmp = s / 60;
		AddMin(tmp);
		this->s = this->s - tmp * 60;
	}
	wstring GetSec()
	{
		wstring str = to_wstring(s);
		return str;
	}
	wstring GetMin()
	{
		wstring str = to_wstring(m);
		return str;
	}
	wstring GetHour()
	{
		wstring str = to_wstring(h);
		return str;
	}
	wstring GetTime()
	{
		wstring tmp;
		if (h < 10)tmp += L"0";
		tmp += to_wstring(h);
		tmp += L":";
		if (m < 10)tmp += L"0";
		tmp += to_wstring(m);
		tmp += L":";
		if (s < 10)tmp += L"0";
		tmp += to_wstring(s);
		return tmp;
	}
};

#endif