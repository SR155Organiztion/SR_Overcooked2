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

        if (is_integral<T>::value) {
            uniform_int_distribution<T> dist(_rangeStart, _rangeEnd);
            return dist(gen);
        }
        else if (is_floating_point<T>::value) {
            uniform_real_distribution<T> dist(_rangeStart, _rangeEnd);
            return dist(gen);
        }
        else {
            static_assert(is_arithmetic<T>::value, "Make_Random only supports arithmetic types");
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
};

