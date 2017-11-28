#include <iostream>
#include <windows.h>

#include "Semaphore.h"

using namespace std;


DWORD WINAPI someFunc(LPVOID param);

Semaphore sem(4);

bool ct;

int main(int argc, char *argv[])
{
	DWORD id;

	for (int i = 0; i < 8; i++)
	{
		HANDLE h = CreateThread(NULL, 0, someFunc, LPVOID(new int(i)), 0, &id);
	}

	ct = true;

	while (1)
		if (sem.ret_Count_dbg() > 10)
		{
			cout << "Error"; return 0;
		}
}

DWORD WINAPI someFunc(LPVOID a)
{
	while (!ct);

	int number = *(int *)(a);
	while (1)
	{
		sem.Enter();
		printf("%i Start\n", number);

		for (int p = 0; p < number + 1; p++) {
			int calc = 0;
			for (int i = 1; i < 501; i++)
			{
				calc += i / (number + 1); Sleep(10);
			}
			printf("Thread %i; res = %i\n", number, calc);
		}

		printf("%i End.", number);
		sem.Leave();

	}
}