#ifndef mirror_h
#define mirror_h
//does not edit the .h files name due to it's my femboy friend's name.qwq

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <mutex>

#define Mirror_Fast()                      \
    do {                                \
        std::ios_base::sync_with_stdio(false); \
        std::cin.tie(nullptr);           \
    } while (0)

#ifdef _WIN32
#include <windows.h>
inline void VTMode(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut,&dwMode))
        return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode (hOut,dwMode);
}//this function open the vtmode in win platform if it's not open yet.
#endif //winh



namespace mirror {
#ifdef _WIN32
inline void vtMODE(){
    static bool ok = []() {
        VTMode();
        return true;
    }();
    (void)ok;
} 
#else 
inline void vtMODE(){}
#endif    

std::string utcnow(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t Now = std::chrono::system_clock::to_time_t(now);
    std::tm Now_s;
#ifdef _WIN32
    gmtime_s(&Now_s, &Now);
#else
    gmtime_r(&Now, &Now_s);
#endif
    char mir[64];
    std::strftime(mir, sizeof(mir), "%Y-%m-%d %H:%M:%S", &Now_s);
    return std::string(mir) + " UTC";
}

enum class Mode {
  clearstrwithoutanother,
  timestrclear,
  nocolor,
  coloredstr,
};

enum class Lv {
  Info,
  Access,
  Warn,
  Error,
  Panic,
};

enum class Colors {
  green,
  blue,
  red,
  white,
  yellow,
  Auto,
};

inline const char* MatchLv(Lv lv){
   switch(lv){
   case Lv::Info:
   return "[INFO]";
   case Lv::Access:
   return "[ACCESS]";
   case Lv::Warn:
   return "[WARN]";
   case Lv::Error:
   return "[ERROR]";
   case Lv::Panic:
   return "[PANIC]";
   default:
   return "[UNKNOWN]";
}
}

inline const char* colorCode(Colors col) {
    switch(col) {
        case Colors::green:  return "\033[32m";
        case Colors::blue:   return "\033[34m";
        case Colors::red:    return "\033[31m";
        case Colors::white:  return "\033[37m";
        case Colors::yellow: return "\033[33m";
        default:             return "\033[0m";  
    }
}

inline void mirprint(Lv lv, const std::string& msg, Mode mode, Colors col = Colors::Auto) {
    vtMODE();
    switch (mode) {
        case Mode::clearstrwithoutanother:
            std::cout << msg << '\n';
            break;

        case Mode::timestrclear:
            std::cout << utcnow() << ' ' << msg << '\n';
            break;

        case Mode::nocolor:
            std::cout << utcnow() << ' ' << MatchLv(lv) << ' ' << msg << '\n';
            break;

        case Mode::coloredstr: {
            if (col == Colors::Auto) {
                switch (lv) {
                   case Lv::Info:   col = Colors::white;  break;
                   case Lv::Access: col = Colors::green;  break;
                   case Lv::Warn:   col = Colors::yellow; break;
                   case Lv::Error:
                   case Lv::Panic:  col = Colors::red;    break;
                   default:         col = Colors::white;  break;
                }
            }
            std::cout << colorCode(col) << utcnow() << ' ' << MatchLv(lv) << ' ' << msg << "\033[0m\n";
            break;
        }
    }
}

} // namespace mirror

#endif //mirror
