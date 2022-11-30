#include "ezshell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef EZSHELL_CMD_BUFFSZ
// Maximum number of commands
#define EZSHELL_CMD_BUFFSZ 24
#endif // EZSHELL_CMD_BUFFSZ

#ifndef EZSHELL_CMD_NAMESZ
// Maximum length of command name
#define EZSHELL_CMD_NAMESZ 23
#endif // EZSHELL_CMD_NAMESZ

#ifndef EZSHELL_CMD_PARAMSZ
// Maximum length of command parameter
#define EZSHELL_CMD_PARAMSZ 64
#endif // EZSHELL_CMD_PARAMSZ

#ifndef EZSHELL_CMD_PARAMNU
// Maximum number of command parameters
#define EZSHELL_CMD_PARAMNU 8
#endif // EZSHELL_CMD_PARAMNU

typedef struct s_action
{
    char name[EZSHELL_CMD_NAMESZ];            // cmd name
    void (*action)(int _argc, char *_argv[]); // cmd action
} action_t;
static action_t g_cmd_list[EZSHELL_CMD_BUFFSZ] = {0};
static int g_cmd_listsz = 0;
static pthread_mutex_t g_cmd_listmut = PTHREAD_MUTEX_INITIALIZER;

// #ifdef __GNUC__
// __attribute__((constructor(101))) static void ezshell_mutex_init(void)
// {
//     pthread_mutex_init(&g_cmd_listmut, 0);
// }
// #endif

int ezshell_cmd_register(const char *_name, void (*_action)(int, char **))
{
    if (0 == _name || 0 == *_name)
        return EZSHELL_NAME;
    if (0 == _action)
        return EZSHELL_ACTION;
    pthread_mutex_lock(&g_cmd_listmut);
    for (int idx = 0; idx < g_cmd_listsz; idx++)
    {
        if (0 == strncmp(
                     _name, g_cmd_list[idx].name,
                     EZSHELL_CMD_NAMESZ))
        {
            pthread_mutex_unlock(&g_cmd_listmut);
            return EZSHELL_DUPLICATE;
        }
    }
    if (EZSHELL_CMD_BUFFSZ > g_cmd_listsz)
    {
        strncpy(g_cmd_list[g_cmd_listsz].name,
                _name, EZSHELL_CMD_NAMESZ);
        g_cmd_list[g_cmd_listsz].action = _action;
        g_cmd_listsz++;
    }
    else
    {
        pthread_mutex_unlock(&g_cmd_listmut);
        return EZSHELL_SPACE;
    }
    pthread_mutex_unlock(&g_cmd_listmut);
    return 0;
}

static void execute_cmd(int argc, char *argv[])
{
    const action_t *act = g_cmd_list;
    if (0 >= argc)
        return;
    while (act->name[0])
    {
        if (0 == strcmp(act->name, argv[0]))
        {
            act->action(argc - 1, argv + 1);
            break;
        }
        act++;
    }
    return;
}

static int argv_split(const char *_str, const char _delim, char *_list[])
{
    unsigned int result;
    unsigned int start;
    unsigned int end;
    result = start = end = 0;
    while (_str[start])
    {
        if (_delim == _str[end] || 0 == _str[end])
        {
            if (start)
                start++;
            if (_delim != _str[start] && end > start)
            {
                unsigned int size = end - start;
                strncpy(_list[result], _str + start, size);
                _list[result][size] = 0;
                result++;
                if (EZSHELL_CMD_PARAMNU <= result)
                    break;
            }
            start = end;
        }
        end++;
    }
    return result;
}

static void *shell_thread(void *arg)
{
    char cmd[256];
    int size;
    size = sizeof(char *) + EZSHELL_CMD_PARAMSZ;
    size *= EZSHELL_CMD_PARAMNU;
    while (1)
    {
        int arg_count;
        char **arg_vector;
        // get cmd srting from stdin
        scanf(" %[^\n]s", cmd);
        getchar();
        // alloc mem for arguments
        arg_vector = (char **)malloc(size);
        if (0 == arg_vector)
            continue;
        char *buff = (char *)(arg_vector + EZSHELL_CMD_PARAMNU);
        for (int idx = 0; idx < EZSHELL_CMD_PARAMNU; idx++)
        {
            arg_vector[idx] = buff;
            buff += EZSHELL_CMD_PARAMSZ;
        }
        // parse arguments
        arg_count = argv_split(cmd, 0x20, arg_vector);
        // execute cmd
        execute_cmd(arg_count, arg_vector);
        // free arguments mem
        free(arg_vector);
    }
    return 0;
}

int sheltsk_init(void)
{
    pthread_t thread;
    pthread_create(&thread, 0, shell_thread, 0);
    return 0;
}
