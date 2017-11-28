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