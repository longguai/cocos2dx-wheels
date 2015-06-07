#ifndef _CW_AUTO_REF_H_
#define _CW_AUTO_REF_H_

#include "base/CCRef.h"

namespace cw {

    template <class _REF> class AutoRef final {
        _REF *_ref = nullptr;

    public:
        ~AutoRef<_REF>() {
            if (_ref != nullptr) _ref->release();
        }

        explicit AutoRef<_REF>(_REF *ref) {
            _ref = ref;
            if (_ref != nullptr) _ref->retain();
        }

        inline AutoRef<_REF> &operator=(const AutoRef<_REF> &other) {
            _ref = other._ref;
            if (_ref != nullptr) _ref->retain();
            return *this;
        }

        inline AutoRef<_REF> &operator=(AutoRef<_REF> &&other) {
            _ref = other._ref;
            other._ref = nullptr;
            return *this;
        }

        AutoRef<_REF>(const AutoRef<_REF> &other) {
            this->operator=(other);
        }

        AutoRef<_REF>(AutoRef<_REF> &&other) {
            this->operator=(other);
        }

        inline _REF *get() {
            return _ref;
        }

        inline void reset() {
            if (_ref != nullptr) _ref->release();
            _ref = nullptr;
        }

        inline void reset(_REF *ref) {
            if (_ref != nullptr) _ref->release();
            _ref = ref;
            if (_ref != nullptr) _ref->retain();
        }

        inline _REF *operator->() {
            return _ref;
        }
    };
}

#endif
