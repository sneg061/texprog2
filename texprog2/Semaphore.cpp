#include "Semaphore.h"


void Semaphore::Enter()
{
	countCS.Enter();
	if (Count++ < maxCount)
	{
		count_dbg++;

		countCS.Leave();
		return;
	}

	countCS.Leave();

	ev.Wait();


	countCS.Enter();
	count_dbg++;
	countCS.Leave();
}

void Semaphore::Leave()
{
	countCS.Enter();            
	if (--Count >= maxCount)  
		ev.Set();               

	count_dbg--;                

	countCS.Leave();

}

DWORD WINAPI Test::testFunc(LPVOID param)
{
	Semaphore * smp = (Semaphore *)param;

	while (1)
	{
		if (abort_thread)
			return 0;

		smp->Enter();
		auto mas = new int[1000];
		for (int i = 0; i<1000; i++)
		{
			mas[i] = rand() % 1000 - 500;
		}
		bool changes = true;
		while (changes)
		{
			changes = false;
			for (int i = 0; i < 999; i++)
				if (mas[i] > mas[i + 1])
				{
					int a = mas[i];
					mas[i] = mas[i + 1];
					mas[i + 1] = a;
					changes = true;
				}
			Sleep(1);
		}
		delete[] mas;
		smp->Leave();
	}
}