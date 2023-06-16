#include <windows.h>
#include <iostream>
#include <time.h>
using namespace std;

#define P 5 // Кол-во философов
clock_t total = 0;
clock_t timep = 0;
HANDLE threads[P];
clock_t start = 0;
CRITICAL_SECTION cs[P];
HANDLE mutex;

int LEFT(int f)
{
    /*if (f == 0)
        return P - 1;
    else*/
        return f % P;
}

int RIGHT(int f)
{
    /*if (f == P - 1)
        return 0;
    else*/
        return (f + 1) % P;
}

void getForks(int f)
{
    EnterCriticalSection(&cs[LEFT(f)]);
    EnterCriticalSection(&cs[RIGHT(f)]);
}

void putDown(int f)
{
    LeaveCriticalSection(&cs[RIGHT(f)]);
    LeaveCriticalSection(&cs[LEFT(f)]);
}

DWORD WINAPI Start(CONST LPVOID philID)
{
    
    while (1){

        // Ждёт пока не освободится вилка
        getForks(int(philID));

        // Естc
        if (clock() - start >= clock_t(total))
        {
            putDown(int(philID));
            return 0;
        }
        WaitForSingleObject(mutex, INFINITE);
        cout << clock() - start << ":" << int(philID) + 1 << ":" << "T->E\n";
        ReleaseMutex(mutex);
        Sleep(timep);

        // Кладёт вилки
        WaitForSingleObject(mutex, INFINITE);
        cout << clock() - start << ":" << int(philID) + 1 << ":" << "E->T\n";
        ReleaseMutex(mutex);
        putDown(int(philID));
        if (clock() - start >= clock_t(total)) return 0;
        // Думает
        Sleep(timep);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    total = atoi(argv[1]);
    timep = atoi(argv[2]);
    for (int i = 0; i < P; i++)
    {
        InitializeCriticalSection(&cs[i]);
    }
    mutex = CreateMutex(NULL, FALSE, NULL);
    start = clock();
    
    for (int i = 0; i < P; i++)
    {
        threads[i] = CreateThread(NULL, 0, &Start, (LPVOID)i, 0, NULL);
    }
    WaitForMultipleObjects(P, threads, TRUE, INFINITE);
    
    for (int i = 0; i < P; i++)
        CloseHandle(threads[i]);
    for (int i = 0; i < P; i++){
        DeleteCriticalSection(&cs[i]);
    }
    CloseHandle(mutex);
    return 0;
}