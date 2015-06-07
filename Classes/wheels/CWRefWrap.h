#ifndef _CW_REF_WRAP_H_
#define _CW_REF_WRAP_H_

#include "base/CCRef.h"

namespace cw {

    template <class _REF> class RefWrap final {
        _REF *_ref = nullptr;

    public:
        ~RefWrap<_REF>() {
            if (_ref != nullptr) _ref->release();
        }

        explicit RefWrap<_REF>(_REF *ref) {
            _ref = ref;
            if (_ref != nullptr) _ref->retain();
        }

        inline RefWrap<_REF> &operator=(const RefWrap<_REF> &other) {
            _ref = other._ref;
            if (_ref != nullptr) _ref->retain();
            return *this;
        }

        inline RefWrap<_REF> &operator=(RefWrap<_REF> &&other) {
            _ref = other._ref;
            other._ref = nullptr;
            return *this;
        }

        RefWrap<_REF>(const RefWrap<_REF> &other) {
            this->operator=(other);
        }

        RefWrap<_REF>(RefWrap<_REF> &&other) {
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
