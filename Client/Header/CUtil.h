#pragma once
class CUtil
{
public:
    template<typename T>
    static T Make_Random(T _rangeStart, T _rangeEnd) {
        static random_device rd;
        static mt19937 gen(rd());

        if constexpr (is_integral<T>::value) {
            uniform_int_distribution<T> dist(_rangeStart, _rangeEnd);
            return dist(gen);
        }
        else if constexpr (is_floating_point<T>::value) {
            uniform_real_distribution<T> dist(_rangeStart, _rangeEnd);
            return dist(gen);
        }
        else {
            static_assert(is_arithmetic<T>::value, "Make_Random only supports arithmetic types");
            return T();
        }
    }
};

