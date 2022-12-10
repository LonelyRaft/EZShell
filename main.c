
#include "ezshell.h"

#ifdef _WIN32
#include <windows.h>
int main(int argc, char *argv[])
{
    ezshell_init();
    while (1)
        Sleep(1000);
    return 0;
}
#endif // _WIN32

#ifdef __linux__
#include <unistd.h>
int main(int argc, char *argv[])
{
    ezshell_init();
    while (1)
        sleep(1);
    return 0;
}
#endif // __linux__
