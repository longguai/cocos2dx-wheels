#ifndef _CW_REF_CONVERTER_H_
#define _CW_REF_CONVERTER_H_

#include "base/CCRef.h"

namespace cw {

    template <class _T> class RefConverter : public cocos2d::Ref {
        _T _val;
    public:
        static RefConverter<_T> *create(const _T &val) {
            RefConverter<_T> *ret = new (std::nothrow) RefConverter<_T>();
            if (ret != nullptr) {
                ret->_val = val;
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        static RefConverter<_T> *create(_T &&val) {
            RefConverter<_T> *ret = new (std::nothrow) RefConverter<_T>();
            if (ret != nullptr) {
                ret->_val = std::move(val);
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        inline _T &get() {
            return _val;
        }

        inline const _T &get() const {
            return _val;
        }

        inline void reset(const _T &val) {
            _val = val;
        }

        inline void reset(_T &&val) {
            _val = std::move(val);
        }
    };

    template <class _T> RefConverter<_T> *createRefConverter(const _T &val) {
        return RefConverter<_T>::create(val);
    }

    template <class _T> RefConverter<_T> *createRefConverter(_T &&val) {
        return RefConverter<_T>::create(std::move(val));
    }
}

#endif
