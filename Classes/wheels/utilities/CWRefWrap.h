#ifndef _CW_REF_WRAP_H_
#define _CW_REF_WRAP_H_

#include "base/CCRef.h"
#include <type_traits>
#include <functional>
#include <ostream>

namespace cw {

    template <class _Ref> class RefWrap final {
        _Ref *_ref = nullptr;

    public:
        ~RefWrap<_Ref>() {
            CC_SAFE_RELEASE(_ref);
            CCLOGINFO("In the destructor of RefWrap!");
        }

        explicit RefWrap<_Ref>(_Ref *ref) {
            static_assert(std::is_convertible<_Ref *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_Ref>!");
            CCLOGINFO("In the default constructor of RefWrap!");
            _ref = ref;
            CC_SAFE_RETAIN(_ref);
        }

        RefWrap<_Ref>(std::nullptr_t) {
            _ref = nullptr;
        }

        RefWrap<_Ref>(const RefWrap<_Ref> &other) noexcept {
            static_assert(std::is_convertible<_Ref *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_Ref>!");
            CCLOGINFO("In the copy constructor of RefWrap!");
            _ref = other._ref;
            CC_SAFE_RETAIN(_ref);
        }

        RefWrap<_Ref>(RefWrap<_Ref> &&other) noexcept {
            static_assert(std::is_convertible<_Ref *, cocos2d::Ref *>::value, "Invalid Type for cw::RefWrap<_Ref>!");
            CCLOGINFO("In the move constructor of RefWrap!");
            _ref = other._ref;
            other._ref = nullptr;
        }

        inline RefWrap<_Ref> &operator=(const RefWrap<_Ref> &other) noexcept {
            CC_SAFE_RELEASE(_ref);
            _ref = other._ref;
            CC_SAFE_RETAIN(_ref);
            return *this;
        }

        inline RefWrap<_Ref> &operator=(RefWrap<_Ref> &&other) noexcept {
            CC_SAFE_RELEASE(_ref);
            _ref = other._ref;
            other._ref = nullptr;
            return *this;
        }

        inline RefWrap<_Ref> &operator=(std::nullptr_t) noexcept {
            CC_SAFE_RELEASE(_ref);
            _ref = nullptr;
            return *this;
        }

        inline _Ref *get() noexcept {
            return _ref;
        }

        inline const _Ref *get() const noexcept {
            return _ref;
        }

        inline void reset() noexcept {
            CC_SAFE_RELEASE(_ref);
            _ref = nullptr;
        }

        inline void reset(_Ref *ref) noexcept {
            CC_SAFE_RELEASE(_ref);
            _ref = ref;
            CC_SAFE_RETAIN(_ref);
        }

        void swap(RefWrap<_Ref> &other) noexcept {
            std::swap(_ref, other._ref);
        }

        inline _Ref *operator->() {
            return _ref;
        }

        inline const _Ref *operator->() const {
            return _ref;
        }

        inline explicit operator bool() const noexcept {
            return (_ref != nullptr);
        }
    };

    template <class _Ref> static inline RefWrap<_Ref> makeRefWrap(_Ref *ref) {
        return RefWrap<_Ref>(ref);
    }

    template <class _Ref1, class _Ref2>
    inline bool operator==(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (left.get() == right.get());
    }

    template <class _Ref1, class _Ref2>
    inline bool operator!=(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (left.get() != right.get());
    }

    template <class _Ref1, class _Ref2>
    inline bool operator<(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (std::less<void *>()(left.get(), right.get()));
    }

    template <class _Ref1, class _Ref2>
    inline bool operator>=(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (!(left < right));
    }

    template <class _Ref1, class _Ref2>
    inline bool operator>(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (right < left);
    }

    template <class _Ref1, class _Ref2>
    inline bool operator<=(const RefWrap<_Ref1> &left, const RefWrap<_Ref2> &right) noexcept {
        return (!(right < left));
    }

    template <class _Ref>
    inline bool operator==(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (left.get() == nullptr);
    }

    template <class _Ref>
    inline bool operator==(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (nullptr == right.get());
    }

    template <class _Ref>
    inline bool operator!=(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (!(left == nullptr));
    }

    template <class _Ref>
    inline bool operator!=(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (!(nullptr== right));
    }

    template <class _Ref>
    inline bool operator<(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (std::less<_Ref *>()(left.get(), nullptr));
    }

    template <class _Ref>
    inline bool operator<(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (std::less<_Ref *>()(nullptr, right.get()));
    }

    template <class _Ref>
    inline bool operator>=(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (!(left < nullptr));
    }

    template <class _Ref>
    inline bool operator>=(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (!(nullptr < right));
    }

    template <class _Ref>
    bool operator>(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (nullptr < left);
    }

    template <class _Ref>
    inline bool operator>(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (right < nullptr);
    }

    template <class _Ref>
    inline bool operator<=(const RefWrap<_Ref> &left, std::nullptr_t) noexcept {
        return (!(nullptr < left));
    }

    template <class _Ref>
    inline bool operator<=(std::nullptr_t, const RefWrap<_Ref> &right) noexcept {
        return (!(right < nullptr));
    }

    template <class _Elem, class _Traits, class _Ref>
    std::basic_ostream<_Elem, _Traits> &operator<<(std::basic_ostream<_Elem, _Traits> &out, const RefWrap<_Ref> &p) {
        return (out << p.get());
    }

    template <class _Ref>
    inline void swap(RefWrap<_Ref> &left, RefWrap<_Ref> &right) noexcept {
        left.swap(right);
    }
}

#endif
