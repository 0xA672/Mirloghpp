#ifndef mirror_h
#define mirror_h
//does not edit the .h files name due to it's my femboy friend's name.qwq

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <mutex>
#include <cstdint>

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
  black,
  green,
  blue,
  magenta,
  cyan,
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
        case Colors::black:  return "\033[30m";
        case Colors::green:  return "\033[32m";
        case Colors::blue:   return "\033[34m";
        case Colors::magenta:return "\033[35m";
        case Colors::cyan:   return "\033[36m";        
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

enum class Style : uint8_t {
    None      = 0,
    Bold      = 1 << 0,
    Dim       = 1 << 1,
    Italic    = 1 << 2,
    Underline = 1 << 3,
    Blink     = 1 << 4,
    Reverse   = 1 << 5,
    Hidden    = 1 << 6,
    Strikethrough = 1 << 7
};

inline Style operator|(Style a, Style b) {
    return static_cast<Style>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline bool operator&(Style a, Style b) {
    return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

class ColoredString {
public:
    explicit ColoredString(std::string s) : str_(std::move(s)) {}

    ColoredString& black()   { fg_ = Colors::black;  return *this; }
    ColoredString& red()     { fg_ = Colors::red;    return *this; }
    ColoredString& green()   { fg_ = Colors::green;  return *this; }
    ColoredString& yellow()  { fg_ = Colors::yellow; return *this; }
    ColoredString& blue()    { fg_ = Colors::blue;   return *this; }
    ColoredString& magenta() { fg_ = Colors::magenta;return *this; }
    ColoredString& cyan()    { fg_ = Colors::cyan;   return *this; }
    ColoredString& white()   { fg_ = Colors::white;  return *this; }

    ColoredString& on_black()   { bg_ = Colors::black;  return *this; }
    ColoredString& on_red()     { bg_ = Colors::red;    return *this; }
    ColoredString& on_green()   { bg_ = Colors::green;  return *this; }
    ColoredString& on_yellow()  { bg_ = Colors::yellow; return *this; }
    ColoredString& on_blue()    { bg_ = Colors::blue;   return *this; }
    ColoredString& on_magenta() { bg_ = Colors::magenta;return *this; }
    ColoredString& on_cyan()    { bg_ = Colors::cyan;   return *this; }
    ColoredString& on_white()   { bg_ = Colors::white;  return *this; }

    ColoredString& bold()          { style_ = style_ | Style::Bold;          return *this; }
    ColoredString& dim()           { style_ = style_ | Style::Dim;           return *this; }
    ColoredString& italic()        { style_ = style_ | Style::Italic;        return *this; }
    ColoredString& underline()     { style_ = style_ | Style::Underline;     return *this; }
    ColoredString& blink()         { style_ = style_ | Style::Blink;         return *this; }
    ColoredString& reverse()       { style_ = style_ | Style::Reverse;       return *this; }
    ColoredString& hidden()        { style_ = style_ | Style::Hidden;        return *this; }
    ColoredString& strikethrough() { style_ = style_ | Style::Strikethrough; return *this; }

    ColoredString& clear_style() { style_ = Style::None; return *this; }

    friend std::ostream& operator<<(std::ostream& os, const ColoredString& cs) {
        vtMODE();  
        
        std::string seq = "\033[";
        bool first = true;
        
        if (cs.fg_ != Colors::Auto) {
            seq += first ? std::to_string(30 + static_cast<int>(cs.fg_)) 
                         : ";" + std::to_string(30 + static_cast<int>(cs.fg_));
            first = false;
        }

        if (cs.bg_ != Colors::Auto) {
            seq += first ? std::to_string(40 + static_cast<int>(cs.bg_)) 
                         : ";" + std::to_string(40 + static_cast<int>(cs.bg_));
            first = false;
        }

        if (cs.style_ & Style::Bold)      { seq += first ? "1" : ";1"; first = false; }
        if (cs.style_ & Style::Dim)       { seq += first ? "2" : ";2"; first = false; }
        if (cs.style_ & Style::Italic)    { seq += first ? "3" : ";3"; first = false; }
        if (cs.style_ & Style::Underline) { seq += first ? "4" : ";4"; first = false; }
        if (cs.style_ & Style::Blink)     { seq += first ? "5" : ";5"; first = false; }
        if (cs.style_ & Style::Reverse)   { seq += first ? "7" : ";7"; first = false; }
        if (cs.style_ & Style::Hidden)    { seq += first ? "8" : ";8"; first = false; }
        if (cs.style_ & Style::Strikethrough) { seq += first ? "9" : ";9"; first = false; }
        
        seq += "m";
        os << seq << cs.str_ << "\033[0m";
        return os;
    }

private:
    std::string str_;
    Colors fg_ = Colors::Auto;
    Colors bg_ = Colors::Auto;
    Style style_ = Style::None;
};

inline ColoredString colored(std::string s) {
    return ColoredString(std::move(s));
}

} // namespace mirror

#endif //mirror
