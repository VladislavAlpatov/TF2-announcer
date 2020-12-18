#include <Windows.h>
#include <iostream>
#include <vector>
#include "tf2.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    DWORD serverbase = (DWORD)GetModuleHandle(L"server.dll");

    int lastHeadShotKill = *(int*)(serverbase + signatures::headShotedkills);
    int lastStreakKill = *(int*)(serverbase + signatures::streakKills);
    int lastDomination = *(int*)(serverbase + signatures::dominations);
    while (!GetAsyncKeyState(VK_END))
    {
        if ((lastHeadShotKill != *(int*)(serverbase + signatures::headShotedkills) and *(int*)(serverbase + signatures::headShotedkills) != 0))
        {
            PlaySound(L"sound\\headshot.wav", NULL, SND_ASYNC);
            lastHeadShotKill = *(int*)(serverbase + signatures::headShotedkills);
        }
        if ((lastDomination != *(int*)(serverbase + signatures::dominations) and *(int*)(serverbase + signatures::dominations) != 0))
        {
            PlaySound(L"sound\\dominating.wav", NULL, SND_ASYNC);
            lastDomination = *(int*)(serverbase + signatures::dominations);
        }
        if (lastStreakKill != *(int*)(serverbase + signatures::streakKills))
        {
            switch (*(int*)(serverbase + signatures::streakKills))
            {
            case 1:
                PlaySound(L"sound\\firstblood.wav", NULL, SND_ASYNC);
                break;
            case 5:
                PlaySound(L"sound\\humiliation.wav", NULL, SND_ASYNC);
                break;
            case 10:
                PlaySound(L"sound\\perfect.wav", NULL, SND_ASYNC);
                break;
            case 15:
                PlaySound(L"sound\\holyshit.wav", NULL, SND_ASYNC);
                break;
            case 25:
                PlaySound(L"sound\\godlike.wav", NULL, SND_ASYNC);

            }
            lastStreakKill = *(int*)(serverbase + signatures::streakKills);
        }

        else if ( (*(int*)(serverbase + signatures::streakKills) == 0) and (lastStreakKill != 0) )
        {
            signatures::streakKills = 0;
        }
        Sleep(100);
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {

    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;

    }
    return TRUE;
}

