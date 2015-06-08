#include "CWNotificationCenter.h"
#include <algorithm>

USING_NS_CC;

namespace cw {

    NotificationCenter *NotificationCenter::_this = nullptr;

    void NotificationCenter::addObserver(void *target, const std::function<void (cocos2d::Ref *)> &selector, const std::string &name, cocos2d::Ref *sender)
    {
        std::unordered_map<void *, CallFuncOMap>::iterator it = _targetMap.find(target);
        if (it != _targetMap.end())
        {
            CallFuncOMap &callfuncOMap = it->second;
            CallFuncOMap::iterator it0 = callfuncOMap.find(name);
            if (it0 != callfuncOMap.end())
            {
                _nc->removeObserver(it0->second.get(), name);

                CallFuncODelegate *cd = it0->second.get();
                cd->reset(selector);
                _nc->addObserver(cd, CallFuncODelegate::getSelector(), name, sender);
            }
            else
            {
                CallFuncODelegate *cd = CallFuncODelegate::create(selector);
                callfuncOMap.insert(std::make_pair(name, makeRefWrap(cd)));
                _nc->addObserver(cd, CallFuncODelegate::getSelector(), name, sender);
            }
        }
        else
        {
            CallFuncOMap callfuncOMap;
            CallFuncODelegate *cd = CallFuncODelegate::create(selector);
            callfuncOMap.insert(std::make_pair(name, RefWrap<CallFuncODelegate>(cd)));
            _targetMap.insert(std::make_pair(target, std::move(callfuncOMap)));
            _nc->addObserver(cd, CallFuncODelegate::getSelector(), name, sender);
        }
    }

    void NotificationCenter::removeObserver(void *target, const std::string &name)
    {
        std::unordered_map<void *, CallFuncOMap>::iterator it = _targetMap.find(target);
        if (it != _targetMap.end())
        {
            CallFuncOMap &callfuncOMap = it->second;
            CallFuncOMap::iterator it0 = callfuncOMap.find(name);
            if (it0 != callfuncOMap.end())
            {
                _nc->removeObserver(it0->second.get(), name);
                callfuncOMap.erase(it0);

                if (callfuncOMap.empty())
                {
                    _targetMap.erase(it);
                }
            }
        }
    }

    int NotificationCenter::removeAllObservers(void *target)
    {
        int cnt = 0;
        std::unordered_map<void *, CallFuncOMap>::iterator it = _targetMap.find(target);
        if (it != _targetMap.end())
        {
            CallFuncOMap &callfuncOMap = it->second;
            std::for_each(callfuncOMap.begin(), callfuncOMap.end(), [this, &cnt](CallFuncOMap::value_type &pair) {
                _nc->removeObserver(pair.second.get(), pair.first);
                ++cnt;
            });
            _targetMap.erase(it);
        }
        return cnt;
    }

}
