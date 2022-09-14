/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#include "cli_runnable.h"
#include "util.h"
#include "program.h"

#if RENDU_PLATFORM != RENDU_PLATFORM_WINDOWS
#include <readline/readline.h>
#include <readline/history.h>
#else
#include <Windows.h>
#endif

static constexpr char CLI_PREFIX[] = "RD> ";

static inline void PrintCliPrefix()
{
    printf("%s", CLI_PREFIX);
}

#if RENDU_PLATFORM != RENDU_PLATFORM_WINDOWS
namespace rendu::Impl::Readline
{
    static std::vector<std::string> vec;
    char* cli_unpack_vector(char const*, int state)
    {
        static size_t i=0;
        if (!state)
            i = 0;
        if (i < vec.size())
            return strdup(vec[i++].c_str());
        else
            return nullptr;
    }

    char** cli_completion(char const* text, int /*start*/, int /*end*/)
    {
        ::rl_attempted_completion_over = 1;
//        vec = rendu::ChatCommands::GetAutoCompletionsFor(CliHandler(nullptr,nullptr), text);
        return ::rl_completion_matches(text, &cli_unpack_vector);
    }

    int cli_hook_func()
    {
        if (sProgram.IsStopped())
            ::rl_done = 1;
        return 0;
    }
}
#endif

void utf8print(void* /*arg*/, std::string_view str)
{
#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
    WriteWinConsole(str);
#else
{
    printf(STRING_VIEW_FMT, STRING_VIEW_FMT_ARG(str));
    fflush(stdout);
}
#endif
}

void commandFinished(void*, bool /*success*/)
{
    PrintCliPrefix();
    fflush(stdout);
}

#ifdef linux
// Non-blocking keypress detector, when return pressed, return 1, else always return 0
int kb_hit_return()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, nullptr, nullptr, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
#endif

/// %Thread start
void CliThread()
{
#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
    // print this here the first time
    // later it will be printed after command queue updates
    PrintCliPrefix();
#else
    ::rl_attempted_completion_function = &rendu::Impl::Readline::cli_completion;
    {
        static char BLANK = '\0';
        ::rl_completer_word_break_characters = &BLANK;
    }
    ::rl_event_hook = &rendu::Impl::Readline::cli_hook_func;
#endif

//    if (sConfigMgr->GetBoolDefault("BeepAtStart", true))
//        printf("\a");                                       // \a = Alert

#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
    //if (sConfigMgr->GetBoolDefault("FlashAtStart", true))
    //{
    //    FLASHWINFO fInfo;
    //    fInfo.cbSize = sizeof(FLASHWINFO);
    //    fInfo.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
    //    fInfo.hwnd = GetConsoleWindow();
    //    fInfo.uCount = 0;
    //    fInfo.dwTimeout = 0;
    //    FlashWindowEx(&fInfo);
    //}
#endif
    ///- As long as the World is running (no World::m_stopEvent), get the command line and handle it
    while (!sProgram.IsStopped())
    {
        fflush(stdout);

        std::string command;

#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
        if (!ReadWinConsole(command))
            continue;
#else
        char* command_str = readline(CLI_PREFIX);
        ::rl_bind_key('\t', ::rl_complete);
        if (command_str != nullptr)
        {
            command = command_str;
            free(command_str);
        }
#endif

        if (!command.empty())
        {
            Optional<std::size_t> nextLineIndex = RemoveCRLF(command);
            if (nextLineIndex && *nextLineIndex == 0)
            {
#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
                PrintCliPrefix();
#endif
                continue;
            }

            fflush(stdout);
//            sWorld->QueueCliCommand(new CliCommandHolder(nullptr, command.c_str(), &utf8print, &commandFinished));
#if RENDU_PLATFORM != RENDU_PLATFORM_WINDOWS
            add_history(command.c_str());
#endif
        }
        else if (feof(stdin))
        {
            sProgram.Exit();
        }
    }
}
