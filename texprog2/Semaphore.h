#pragma once

#ifndef H_SEMOPHORE
#define H_SEMOPHORE

#include <windows.h>
#include <iostream>

using namespace std;

class CS {
	CRITICAL_SECTION CS_;

public:
	CS() { InitializeCriticalSection(&CS_); }
	void Enter() { EnterCriticalSection(&CS_); }
	void Leave() { LeaveCriticalSection(&CS_); }
};

class Event {
	HANDLE hEvent;
public:
	Event() { hEvent = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("Event")); }
	~Event() { CloseHandle(hEvent); }

	void Set() { SetEvent(hEvent); }
	void Reset() { ResetEvent(hEvent); }

	DWORD Wait(int ms) { return WaitForSingleObject(hEvent, ms); }
	DWORD Wait() { return Wait(INFINITE); }
};

class Semaphore {
	int maxCount;
	int Count;
	int count_dbg;

	CS countCS;
	Event ev;


public:
	Semaphore(int count) : maxCount(count), Count(0), count_dbg(0) { /*f = fopen( "out.txt", "wt" );*/ }
	~Semaphore() { /*fclose( f );*/ }

	void Enter();
	void Leave();

	int ret_Count_dbg() { return count_dbg; }
};

#endif