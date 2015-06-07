#ifndef _CW_CALLFUNC_DELEGATE_H_
#define _CW_CALLFUNC_DELEGATE_H_

#include "base/CCRef.h"
#include <functional>

namespace cw {
    class CallFuncDelegate final : public cocos2d::Ref {
    public:
        static CallFuncDelegate *create(const std::function<void ()> &selector) {
            CallFuncDelegate *ret = new (std::nothrow) CallFuncDelegate();
            if (ret != nullptr) {
                ret->_selector = selector;
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        static inline cocos2d::SEL_CallFunc getSelector() {
            return (cocos2d::SEL_CallFunc)&CallFuncDelegate::selector;
        }

    private:
        void selector() {
            _selector();
        }

        std::function<void ()> _selector;
    };

    class CallFuncNDelegate final : public cocos2d::Ref {
    public:
        static CallFuncNDelegate *create(const std::function<void (cocos2d::Node *)> &selector) {
            CallFuncNDelegate *ret = new (std::nothrow) CallFuncNDelegate();
            if (ret != nullptr) {
                ret->_selector = selector;
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        static inline cocos2d::SEL_CallFuncN getSelector() {
            return (cocos2d::SEL_CallFuncN)&CallFuncNDelegate::selector;
        }

    private:
        void selector(cocos2d::Node *node) {
            _selector(node);
        }

        std::function<void (cocos2d::Node *)> _selector;
    };

    class CallFuncNDDelegate final : public cocos2d::Ref {
    public:
        static CallFuncNDDelegate *create(const std::function<void (cocos2d::Node *, void *)> &selector) {
            CallFuncNDDelegate *ret = new (std::nothrow) CallFuncNDDelegate();
            if (ret != nullptr) {
                ret->_selector = selector;
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        static inline cocos2d::SEL_CallFuncND getSelector() {
            return (cocos2d::SEL_CallFuncND)&CallFuncNDDelegate::selector;
        }

    private:
        void selector(cocos2d::Node *node, void *data) {
            _selector(node, data);
        }

        std::function<void (cocos2d::Node *, void *)> _selector;
    };

    class CallFuncODelegate final : public cocos2d::Ref {
    public:
        static CallFuncODelegate *create(const std::function<void (cocos2d::Ref *)> &selector) {
            CallFuncODelegate *ret = new (std::nothrow) CallFuncODelegate();
            if (ret != nullptr) {
                ret->_selector = selector;
                ret->autorelease();
                return ret;
            }
            return nullptr;
        }

        static inline cocos2d::SEL_CallFuncO getSelector() {
            return (cocos2d::SEL_CallFuncO)&CallFuncODelegate::selector;
        }

    private:
        void selector(cocos2d::Ref *ref) {
            _selector(ref);
        }

        std::function<void (cocos2d::Ref *)> _selector;
    };
}

#endif
