#pragma once
#include "pch.h"
#include "Engine_Define.h"

class CUtil
{
public:
    template<typename T>
    static T Make_Random(T _rangeStart, T _rangeEnd) {
        static random_device rd;
        static mt19937 gen(rd());

        if constexpr (std::is_integral<T>::value) {
            uniform_int_distribution<T> dist(_rangeStart, _rangeEnd);
            return dist(gen);
        }
        else if constexpr (std::is_floating_point<T>::value || std::is_same<T, _float>::value) {
            uniform_real_distribution<float> dist(_rangeStart, _rangeEnd); // 내부적으로 float 사용
            return static_cast<T>(dist(gen));
        }
        else {
            static_assert(std::is_arithmetic<T>::value, "Make_Random only supports arithmetic types");
            return T();
        }
    }


    static wstring StringToWString(const std::string& str)
    {
        return std::wstring(str.begin(), str.end());
    }

    static _bool isSameStr(const _tchar* _pDest, const _tchar* _pTarget) {
        return _tcscmp(_pDest, _pTarget) == 0;
    }

    static const _tchar* ConvertToWChar(const string& str)
    {
        static wstring wstr;

        int iLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
        if (iLen == 0) return nullptr;

        wstr.resize(iLen);
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], iLen);

        return wstr.c_str();
    }
};

