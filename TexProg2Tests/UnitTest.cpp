#include "stdafx.h"
#include "../texprog2/Semaphore.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace TexProg2Tests
{
	int abort_thread = 0;

	void testFunc(LPVOID param)
	{
		Semaphore * smp = (Semaphore *)param;

		while (1)
		{
			if (abort_thread)
				return;

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

	[TestClass]
	public ref class UnitTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Получает или устанавливает контекст теста, в котором предоставляются
		///сведения о текущем тестовом запуске и обеспечивается его функциональность.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//При написании тестов можно использовать следующие дополнительные атрибуты:
		//
		//ClassInitialize используется для выполнения кода до запуска первого теста в классе
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//ClassCleanup используется для выполнения кода после завершения работы всех тестов в классе
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//TestInitialize используется для выполнения кода перед запуском каждого теста
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//TestCleanup используется для выполнения кода после завершения каждого теста
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		void TestBody(int threads, int semaphore, int wt)
		{

			abort_thread = 0;

			Semaphore test_sm(semaphore);


			HANDLE *h = new HANDLE[threads];
			DWORD id;
			for (int i = 0; i < threads; i++)
			{
				h[i] = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(testFunc), &test_sm, 0, &id);
			}

			Sleep(wt);

			abort_thread = 1;
			WaitForMultipleObjects(threads, h, TRUE, INFINITY);

			delete h;
		}

		[TestMethod]
		void Th10S2T1000()
		{
			TestBody(10, 2, 1000);
		};

		[TestMethod]
		void Th100S2T10000()
		{
			TestBody(100, 2, 10000);
		};

		[TestMethod]
		void Th5S1T1000()
		{
			TestBody(5, 1, 1000);
		};

		[TestMethod]
		void Th1000S2T1000000()
		{
			TestBody(1000, 2, 1000000);
		};

		[TestMethod]
		void Th100S10T1000000()
		{
			TestBody(100, 10, 1000000);
		};
	};
}
