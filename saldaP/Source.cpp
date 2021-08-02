#include<Windows.h>
#include<tchar.h>
#include"timer.h"
#include<string>
#include"resource.h"
#include"Saper.h"
#include"playerData.h"
#include "russian.h"
#include <locale>
#include <fstream>
#include <list>
#include <iterator>
#include <CommCtrl.h>
#pragma comment(lib,"comctl32")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
using namespace std;

#define IDC_S1 4100

//************************Все глобальные переменные
HWND hBut[30][30], hStatus, hStat;
HBITMAP hBmp[13];
wstring str;
wstring name;
Mines m;
playerData pData;
russian r;
int LEFT = 10, TOP = 10, WIDTH = 20, HEIGHT = 20;
int l,top;
int x, y;
int iCheck[30][30];
bool bCheck[30][30];
bool rClicked[30][30], lClicked[30][30];
int checkPoint[30][30];
bool pointChecked[30][30];
int size = 10, minesCount = 10;
bool gameOver;
int statusMas[3] = { 60, 120, -1 };
timer t;
//*******************************************************

BOOL CALLBACK MainPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK GamePrc1(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK GamePrc2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK GamePrc3(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK LevelPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK MinesPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL CALLBACK RecordsPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR cmdLine, INT cmdShow)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainPrc);
	return 0;
}

//Установка параметров
VOID UpdateSettings()
{
	m.SetSize(size);
	m.SetMines(minesCount);
	m.FillTheFields();
}

//Сброс всех параметров
VOID ResetAll()
{
	for (size_t i = 0; i < 30; i++)
	{
		for (size_t j = 0; j < 30; j++)
		{
			iCheck[i][j] = 0;
			bCheck[i][j] = false;
			lClicked[i][j] = false;
			rClicked[i][j] = false;
			checkPoint[i][j] = 0;
			pointChecked[i][j] = false;
		}
	}
	gameOver = false;
	m.Reset();
	t.Reset();
	if (size == 10)pData.SetLevel(1);
	if (size == 20)pData.SetLevel(2);
	if (size == 30)pData.SetLevel(3);
}

//Главное диалоговое окно
BOOL CALLBACK MainPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static HWND hEdit;
	switch (msg)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
		for (size_t i = 0; i < 13; i++)
		{
			hBmp[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1 + i));
			hEdit = GetDlgItem(hWnd, IDC_EDITNAME);
			hStat = GetDlgItem(hWnd, IDC_STATICNAME);
		}
		name = L"Игрок 1";
		SetWindowText(hStat, name.c_str());
		ResetAll();
		return TRUE;
	case WM_COMMAND:
		if (wp == IDC_BUTTON1)
		{
			if (size == 10) DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_GAMEDIALOG1), hWnd, GamePrc1);
			if (size == 20) DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_GAMEDIALOG2), hWnd, GamePrc2);
			if (size == 30) DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_GAMEDIALOG3), hWnd, GamePrc3);
			return TRUE;
		}
		if (wp == IDC_BUTTON2)
		{
			DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_LEVELDIALOG), hWnd, LevelPrc);
			return TRUE;
		}
		if (wp == IDC_BUTTON3)
		{
			DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MINESDIALOG), hWnd, MinesPrc);
			return TRUE;
		}
		if (wp == IDC_BUTTON4)
		{
			EndDialog(hWnd, 0);
			return TRUE;
		}
		if (wp == IDC_BUTTON6)
		{
			DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_RECORDDIALOG), hWnd, RecordsPrc);
			return TRUE;
		}
		if (wp == IDC_BUTTON5)
		{
			int len;
			len = GetWindowTextLength(hEdit) + 1;
			if (len < 2)return TRUE;
			else
			{
				name.clear();
				SendMessage(hEdit, WM_GETTEXT, len, (LPARAM)name.c_str());
				name = name.c_str();
				name = r.MakeRussian(name);
				SetWindowText(hStat, name.c_str());
			}
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

VOID CALLBACK TmPrc(HWND hWnd, UINT msg, UINT_PTR id, DWORD id2)
{
	t.AddSec(1);
	SendMessage(hStatus, SB_SETTEXT, 2, LPARAM(t.GetTime().c_str()));
}

//Функция создающая ячейки
VOID WinCreater(HWND hWnd, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		top = TOP + (i * WIDTH);
		for (size_t j = 0; j < size; j++)
		{
			l = LEFT + (j * WIDTH);
			hBut[i][j] = CreateWindowEx(0, TEXT("STATIC"), 0, WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER, l, top, WIDTH, HEIGHT, hWnd, (HMENU)(IDC_S1 + (i*size) + j), GetModuleHandle(0), NULL);
			SendMessage(hBut[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp[9]);
		}
		hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, 0, hWnd, 1025);
		SendMessage(hStatus, SB_SETPARTS, 3, LPARAM(statusMas));
		SetTimer(hWnd, 1, 1000, TmPrc);
		SendMessage(hStatus, SB_SETTEXT, 2, LPARAM(t.GetTime().c_str()));
	}
}

//Функция раскрытия пустых ячеек если была нажата одна из них, чтобы при этом раскрывались первые цифры по краям
VOID OpenEmptyCells(LPARAM lp)
{
	x = LOWORD(lp);
	y = HIWORD(lp);
	int k, l, kb, ke, lb, le;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (x >(10 + (j * 20)) && x < (30 + (j * 20)) && y >(10 + (i * 20)) && y < (30 + (i * 20)))
			{
				if (m.GetValue(i, j) == 0)
				{
					checkPoint[i][j] = 1;
				}
				int ch = 0;
				while (ch < size)
				{
					for (size_t k = 0; k < size; k++)
					{
						for (size_t l = 0; l < size; l++)
						{
							if (checkPoint[k][l] == 1 && pointChecked[k][l] == false)
							{
								kb = k - 1; ke = k + 2;
								lb = l - 1; le = l + 2;
								if (k == 0)kb = k;
								if (k == (size - 1))ke = k + 1;
								if (l == 0)lb = l;
								if (l == (size - 1))le = l + 1;
								for (size_t kn = kb; kn < ke; kn++)
								{
									for (size_t ln = lb; ln < le; ln++)
									{
										if (m.GetValue(kn, ln) == 0)checkPoint[kn][ln] = 1;
										else checkPoint[kn][ln] = 2;
									}
								}
								pointChecked[k][l] = true;
							}
						}
					}
					ch++;
				}
			}
		}
	}
}

//Функция обрабатывающая нажатие левой кнопки мыши
VOID LeftButtonClicked(HWND hWnd, LPARAM lp)
{
	x = LOWORD(lp);
	y = HIWORD(lp);
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (x >(10 + (j * 20)) && x < (30 + (j * 20)) && y >(10 + (i * 20)) && y < (30 + (i * 20)))
			{
				if (!rClicked[i][j])
				{
					lClicked[i][j] = true;
					if (m.IsMine(i, j))
					{
						for (size_t y = 0; y < size; y++)
						{
							for (size_t z = 0; z < size; z++)
							{
								lClicked[y][z] = true;
								if (m.IsMine(y, z))SendMessage(hBut[y][z], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp[8]);
								if (m.GetValue(y, z) == 0)SendMessage(hBut[y][z], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp[10]);
								else SendMessage(hBut[y][z], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp[m.GetValue(y, z) - 1]);
							}
						}
						gameOver = true;
						KillTimer(hWnd, 1);
						MessageBox(hWnd, TEXT("Вы не смогли обезвредить все мины!"), TEXT("Ты ЛУЗЕР! ААХАХАХАХАА!!))"), 0);
					}
					else if (m.GetValue(i, j) == 0)
					{
						OpenEmptyCells(lp);
						for (size_t k = 0; k < size; k++)
						{
							for (size_t l = 0; l < size; l++)
							{
								if (checkPoint[k][l] == 1)
								{
									lClicked[k][l] = true;
									rClicked[k][l] = false;
									SendMessage(hBut[k][l], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[10]);
								}
								else if (checkPoint[k][l] == 2)
								{
									lClicked[k][l] = true;
									rClicked[k][l] = false;
									SendMessage(hBut[k][l], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[m.GetValue(k, l) - 1]);
								}
							}
						}
					}
					else
					{
						SendMessage(hBut[i][j], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[m.GetValue(i, j)-1]);
					}
				}
			}
		}
	}
}

//Функция обрабатывающая нажатие правой кнопки мыши
VOID RightButtonClicked(LPARAM lp)
{
	x = LOWORD(lp);
	y = HIWORD(lp);
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (x >(10 + (j * 20)) && x < (30 + (j * 20)) && y >(10 + (i * 20)) && y < (30 + (i * 20)))
			{
				if (!lClicked[i][j])
				{
					rClicked[i][j] = true;
					if (iCheck[i][j] == 0)bCheck[i][j] = false;
					if (iCheck[i][j] == 2)bCheck[i][j] = true;
					if (!bCheck[i][j])iCheck[i][j]++;
					if (bCheck[i][j])
					{
						iCheck[i][j] = 0;
						rClicked[i][j] = false;
					}
					if (iCheck[i][j] == 0) SendMessage(hBut[i][j], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[9]);
					if (iCheck[i][j] == 1) SendMessage(hBut[i][j], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[11]);
					if (iCheck[i][j] == 2) SendMessage(hBut[i][j], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp[12]);
				}
			}
		}
	}
}

//Функция определяющая выиграл ли игрок
VOID WINNER(HWND hWnd)
{
	int counter = 0;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (rClicked[i][j])counter++;
			if (lClicked[i][j])counter++;
		}
	}
	if (counter == (size*size) && gameOver == false)
	{
		KillTimer(hWnd, 1);
		MessageBox(hWnd, TEXT("Да ты красавчик, брат! От души!"), TEXT("ТЫ ПОБЕДИТЕЛЬ ПО ЖИЗНИ!!!!"), 0);
	}
}

BOOL CALLBACK GamePrc1(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		pData.SetData(name, t);
		pData.SaveToFile();
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		ResetAll();
		WinCreater(hWnd, size);
		UpdateSettings();
		return TRUE;
	case WM_LBUTTONDOWN:
		LeftButtonClicked(hWnd, lp);
		WINNER(hWnd);
		return TRUE;
	case WM_RBUTTONDOWN:
		RightButtonClicked(lp);
		WINNER(hWnd);
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK GamePrc2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		pData.SetData(name, t);
		pData.SaveToFile();
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		ResetAll();
		WinCreater(hWnd, size);
		UpdateSettings();
		return TRUE;
	case WM_LBUTTONDOWN:
		LeftButtonClicked(hWnd, lp);
		return TRUE;
	case WM_RBUTTONDOWN:
		RightButtonClicked(lp);
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK GamePrc3(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		pData.SetData(name, t);
		pData.SaveToFile();
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		ResetAll();
		WinCreater(hWnd, size);
		UpdateSettings();
		return TRUE;
	case WM_LBUTTONDOWN:
		LeftButtonClicked(hWnd, lp);
		return TRUE;
	case WM_RBUTTONDOWN:
		RightButtonClicked(lp);
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK LevelPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_COMMAND:
		for (size_t i = 0; i < 3; i++)
		{
			if (wp == IDC_BUTTON1+i)
			{
				if ((10 * (i + 1)*10*(i+1)) < minesCount)
				{
					MessageBox(hWnd, TEXT("Размер поля меньше количества мин!"), TEXT("Ошибка!"), MB_ICONERROR | MB_OK);
					return TRUE;
				}
				size = 10 * (i + 1);
				pData.SetLevel(i + 1);
				EndDialog(hWnd, 0);
				return TRUE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK MinesPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static HWND hEdit;
	int len;
	switch (msg)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		hEdit = GetDlgItem(hWnd, IDC_EDIT1);
		return TRUE;
	case WM_COMMAND:
		if (wp == IDC_BUTTON1)
		{
			len = GetWindowTextLength(hEdit)+1;
			SendMessage(hEdit, WM_GETTEXT, len, (LPARAM)str.c_str());
			int a = stoi(str);
			if (a > (size*size))
			{
				MessageBox(hWnd, TEXT("Количество мин превышает размер поля!"), TEXT("Ошибка!"), MB_ICONERROR | MB_OK);
				return TRUE;
			}
			minesCount = stoi(str);
			EndDialog(hWnd, 0);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK RecordsPrc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static HWND hList;
	static int tabStops[2] = { 100, 200 };
	list <wstring> lName, lTime;
	switch (msg)
	{
	case WM_CLOSE:
		//SendMessage(hList, LB_DELETESTRING, 0, 0);
		/*lName.clear();
		lTime.clear();*/

		for (size_t i = 0; i < lName.size(); i++)
		{
			SendMessage(hList, LB_DELETESTRING, 0, 0);
		}

		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		pData.Clear();
		pData.ReadFromFile();
		hList = GetDlgItem(hWnd, IDC_RECORDLIST);SendMessage(hList, LB_ADDSTRING, 0, 0);
		SendMessage(hList, LB_SETTABSTOPS, 2, (LPARAM)&tabStops);
		lName = pData.GetListOfNames();
		lTime = pData.GetListOfTimer();
		list<wstring>::iterator itN;
		list<wstring>::iterator itT;
		for (itN = lName.begin(), itT = lTime.begin(); itN != lName.end(), itT != lTime.end(); itN++, itT++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)(*itN+TEXT("\t")+*itT).c_str());
		}
		return TRUE;
	}
	return FALSE;
}