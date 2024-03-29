﻿#ifndef _CW_NOTIFICATION_CENTER_H_
#define _CW_NOTIFICATION_CENTER_H_

#include "deprecated/CCNotificationCenter.h"
#include "../utilities/CWRefWrap.h"
#include "CWCallFuncDelegate.h"
#include <unordered_map>
#include <string>

namespace cw {

    class NotificationCenter final {
    public:
        static inline NotificationCenter *getInstance() {
            if (_this == nullptr) {
                _this = new (std::nothrow) NotificationCenter();
            }
            return _this;
        }

        static inline NotificationCenter *sharedNotificationCenter() {
            return NotificationCenter::getInstance();
        }

        static inline void destroyInstance() {
            if (_this != nullptr) {
                delete _this;
                _this = nullptr;
            }
        }

        static inline void purgeNotificationCenter() {
            NotificationCenter::destroyInstance();
        }

        void postNotification(const std::string &name) {
            _nc->postNotification(name);
        }

        void postNotification(const std::string &name, cocos2d::Ref *sender) {
            _nc->postNotification(name, sender);
        }

        void addObserver(void *target, const std::function<void (cocos2d::Ref *)> &selector, const std::string &name, cocos2d::Ref *sender);
        void removeObserver(void *target, const std::string &name);
        int removeAllObservers(void *target);

    private:
        NotificationCenter() {
            _nc = cocos2d::__NotificationCenter::getInstance();
        }

        cocos2d::__NotificationCenter *_nc = nullptr;
        static NotificationCenter *_this;

        typedef RefWrap<CallFuncODelegate> CallFuncOMapValue;
        typedef std::unordered_map<std::string, CallFuncOMapValue> CallFuncOMap;
        std::unordered_map<void *, CallFuncOMap> _targetMap;
    };

}

#endif
