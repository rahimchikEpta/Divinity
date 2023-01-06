#pragma once
#include "../utilities/utilities.hpp"
#include <cstdarg>
#include <xlocinfo>

class c_hud_chat {
public:
    enum chat_filters {
        CHAT_FILTER_NONE = 0,
        CHAT_FILTER_JOINLEAVE = 0x000001,
        CHAT_FILTER_NAMECHANGE = 0x000002,
        CHAT_FILTER_PUBLICCHAT = 0x000004,
        CHAT_FILTER_SERVERMSG = 0x000008,
        CHAT_FILTER_TEAMCHANGE = 0x000010,
        CHAT_FILTER_ACHIEVEMENT = 0x000020,
    };

    void chat_print_f(const char* fmt, ...) {
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsprintf_s(msg, fmt, args);
        utilities::call_virtual<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, 0, 0, fmt);
        va_end(args);
    }
};