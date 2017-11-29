#include <iostream>
#include <windows.h>

#include "Semaphore.h"

using namespace std;


DWORD WINAPI testFunc(LPVOID param);

Semaphore sem(4);

bool ct;

int main(int argc, char *argv[])
{
	DWORD id;

	for (int i = 0; i < 8; i++)
	{
		HANDLE h = CreateThread(NULL, 0, testFunc, LPVOID(new int(i)), 0, &id);
	}

	ct = true;

	while (1)
		if (sem.ret_Count_dbg() > 10)
		{
			cout << "Error"; return 0;
		}
}

DWORD WINAPI testFunc(LPVOID a)
{
	while (!ct);

	int number = *(int *)(a);
	while (1)
	{
		sem.Enter();
		printf("%i Start\n", number);

		auto mas = new int[(number+1)*20];
		for (int i = 0; i<(number + 1) * 20; i++)
		{
			mas[i] = rand() % 10;
		}
		bool changes = true;
		while (changes)
		{
			changes = false;
			for (int i = 0; i < (number + 1) * 20; i++)
				if (mas[i] > mas[i + 1])
				{
					int a = mas[i];
					mas[i] = mas[i + 1];
					mas[i + 1] = a;
					changes = true;
				}
			Sleep(number);
		}

		for (int i = 0; i < (number + 1) * 20; i++)
			printf("%i ", mas[i]);
		
		Sleep(1);

		printf("\n%i End.\n", number);
		sem.Leave();

	}
}