#ifndef SAPER_H
#define SAPER_H
#pragma once

#include <ctime>

class Mines
{
	int countOfMines=0;
	int size=10;
	int minesMas[30][30];
	int value = 0;
public:
	Mines()
	{
		this->countOfMines = 0;
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				minesMas[i][j] = 0;
			}
		}
	}
	void SetSize(int size=10)
	{
		this->size = size;
	}

	void SetMines(int _countOfMines)
	{
		int i, j;
		srand(time(NULL));
		while (this->countOfMines < _countOfMines)
		{
			i = rand() % size;
			j = rand() % size;
			if (!IsMine(i, j))
			{
				minesMas[i][j] = 9;
				this->countOfMines++;
			}
		}
	}

	void FillTheFields()
	{
		int ib, ie, jb, je;
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				value = 0;
				if (!IsMine(i,j))
				{
					ib = i - 1; ie = i + 2;
					jb = j - 1; je = j + 2;
					if (i == 0)ib = i;
					if (i == (size - 1))ie = (i + 1);
					if (j == 0)jb = j;
					if (j == (size - 1))je = (j + 1);
					for (size_t k = ib; k < ie; k++)
					{
						for (size_t l = jb; l < je; l++)
						{
							if (IsMine(k, l))value++;
						}
					}
					minesMas[i][j] = value;
				}
			}
		}
	}

	void Reset()
	{
		this->countOfMines = 0;
		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				minesMas[i][j] = 0;
			}
		}
	}
	int GetValue(int i, int j)
	{
		return minesMas[i][j];
	}
	bool IsMine(int i, int j)
	{
		return minesMas[i][j] == 9;
	}
};



#endif