
#ifndef _EZSHELL_H
#define _EZSHELL_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __GNUC__
#ifdef __INTELLISENSE__
#pragma diag_suppress 1094
#endif // __INTELLISENSE__
#define EZSHELL_REGISTER(ezshell_cmd)                    \
    __attribute__((constructor(110))) static void        \
        ezshell_register_##ezshell_cmd(void)             \
    {                                                    \
        ezshell_cmd_register(#ezshell_cmd, ezshell_cmd); \
    }
#else
#define EZSHELL_REGISTER
#endif // __GNU__

// register cmd successfully
#define EZSHELL_OKAY 0
// invalid cmd name
#define EZSHELL_NAME -1
// invalid cmd action
#define EZSHELL_ACTION -2
// out of space
#define EZSHELL_SPACE -3
// duplicate cmd (name)
#define EZSHELL_DUPLICATE -4

    int ezshell_cmd_register(
        const char *_name,
        void (*_action)(int, char **));

    extern int ezshell_init(void);

#ifdef _cplusplus
}
#endif

#endif // _EZSHELL_H
