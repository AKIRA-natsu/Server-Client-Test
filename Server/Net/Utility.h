//#pragma once
#include <Winsock2.h>
#include <sstream>

// inline: resolve the mulity defined of method "UTF8ToGBEx" and "GBToUTF8Ex"
// come from: https://cplusplus.com/forum/beginner/233483/

namespace AKIRA_Net {
    // UTF-8 to GB2312
    inline std::string UTF8ToGBEx(const char* utf8)
    {
        if (!utf8 || strlen(utf8) < 1)
            return "";

        std::stringstream ss;
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
        len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
        ss << str;
        delete[]wstr;
        delete[]str;
        return ss.str();
    }

    // GB2312 to UTF-8
    inline std::string GBToUTF8Ex(const char* gb2312)
    {
        if (!gb2312 || strlen(gb2312) < 1)
            return "";

        std::stringstream ss;
        int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);
        MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
        len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        memset(str, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
        ss << str;
        delete[]wstr;
        delete[]str;
        return ss.str();
    }
}
