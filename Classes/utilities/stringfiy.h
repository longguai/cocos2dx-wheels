#ifndef _STRINGFIY_H_
#define _STRINGFIY_H_

#include <string>
#include <ostream>
#include <sstream>

namespace __stringify_impl__ {
    template <class _OSTREAM, class _T0, class ..._ARGS> struct Writer {
        static inline _OSTREAM &invoke(_OSTREAM &os, const _T0 &t0, const _ARGS &...args) {
            os << t0;
            return Writer<_OSTREAM, _ARGS...>::invoke(os, args...);
        }
    };

    template <class _OSTREAM, class _T0> struct Writer<_OSTREAM, _T0> {
        static inline _OSTREAM &invoke(_OSTREAM &os, const _T0 &t0) {
            os << t0;
            return os;
        }
    };
}

template <class ..._ARGS> std::string stringify(const _ARGS &...args) {
    std::ostringstream ss;
    return __stringify_impl__::Writer<std::ostringstream, _ARGS...>::invoke(ss, args...).str();
}

static std::string stringify() {
    return "";
}

#endif
