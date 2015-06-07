#ifndef _CW_REF_WRAP_H_
#define _CW_REF_WRAP_H_

#include "base/CCRef.h"
#include <type_traits>

namespace cw {

    template <class _REF> class RefWrap final {
        _REF *_ref = nullptr;

    public:
        ~RefWrap<_REF>() {
            CC_SAFE_RELEASE(_ref);
            CCLOGINFO("In the destructor of RefWrap!");
        }

        explicit RefWrap<_REF>(_REF *ref) {
            static_assert(std::is_convertible<_REF *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_REF>!");
            CCLOGINFO("In the default constructor of RefWrap!");
            _ref = ref;
            CC_SAFE_RETAIN(_ref);
        }

        RefWrap<_REF>(const RefWrap<_REF> &other) {
            static_assert(std::is_convertible<_REF *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_REF>!");
            CCLOGINFO("In the copy constructor of RefWrap!");
            _ref = other._ref;
            CC_SAFE_RETAIN(_ref);
        }

        RefWrap<_REF>(RefWrap<_REF> &&other) {
            static_assert(std::is_convertible<_REF *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_REF>!");
            CCLOGINFO("In the move constructor of RefWrap!");
            _ref = other._ref;
            other._ref = nullptr;
        }

        inline RefWrap<_REF> &operator=(const RefWrap<_REF> &other) {
            CC_SAFE_RELEASE(_ref);
            _ref = other._ref;
            CC_SAFE_RETAIN(_ref);
            return *this;
        }

        inline RefWrap<_REF> &operator=(RefWrap<_REF> &&other) {
            CC_SAFE_RELEASE(_ref);
            _ref = other._ref;
            other._ref = nullptr;
            return *this;
        }

        inline _REF *get() {
            return _ref;
        }

        inline const _REF *get() const {
            return _ref;
        }

        inline void reset() {
            CC_SAFE_RELEASE(_ref);
            _ref = nullptr;
        }

        inline void reset(_REF *ref) {
            CC_SAFE_RELEASE(_ref);
            _ref = ref;
            CC_SAFE_RETAIN(_ref);
        }

        inline _REF *operator->() {
            return _ref;
        }

        inline const _REF *operator->() const {
            return _ref;
        }
    };

    template <class _REF> static RefWrap<_REF> makeRefWrap(_REF *ref) {
        return RefWrap<_REF>(ref);
    }
}

#endif
