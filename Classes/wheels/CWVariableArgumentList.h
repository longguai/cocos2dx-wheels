#ifndef _CW_VARIABLE_ARGUMENT_LIST_H_
#define _CW_VARIABLE_ARGUMENT_LIST_H_

#include "base/CCVector.h"
#include "2d/CCActionInterval.h"

namespace cw {

    template <class _T0, class ..._ARGS> struct VALPacker {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &vec, _T0 t0, _ARGS...args) {
            vec.pushBack(t0);
            VALPacker<_ARGS...>::pack(vec, args...);
        }
    };

    template <class ..._ARGS> struct VALPacker<std::nullptr_t, _ARGS...> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &, std::nullptr_t, _ARGS...) {
        }
    };

    template <class _T0> struct VALPacker<_T0> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &vec, _T0 t0) {
            vec.pushBack(t0);
        }
    };

    template <> struct VALPacker<std::nullptr_t> {
        template <class _T>
        static void pack(cocos2d::Vector<_T> &, std::nullptr_t) {
        }
    };

    // 用不定长参数创建一个Sequence动作，参数可以用nullptr结尾，也可以不用，随便你
    template <class _FINITE_TIME_ACTION_0, class ..._FINITE_TIME_ACTION_N>
    cocos2d::Sequence *createSequence(_FINITE_TIME_ACTION_0 action0, _FINITE_TIME_ACTION_N...actionN) {
        cocos2d::Vector<cocos2d::FiniteTimeAction *> vec;
        VALPacker<_FINITE_TIME_ACTION_0, _FINITE_TIME_ACTION_N...>::pack(vec, action0, actionN...);
        return cocos2d::Sequence::create(vec);
    }

    // 用不定长参数创建一个Spawn动作，参数可以用nullptr结尾，也可以不用，随便你
    template <class _FINITE_TIME_ACTION_0, class ..._FINITE_TIME_ACTION_N>
    cocos2d::Spawn *createSpawn(_FINITE_TIME_ACTION_0 action0, _FINITE_TIME_ACTION_N...actionN) {
        cocos2d::Vector<cocos2d::FiniteTimeAction *> vec;
        VALPacker<_FINITE_TIME_ACTION_0, _FINITE_TIME_ACTION_N...>::pack(vec, action0, actionN...);
        return cocos2d::Spawn::create(vec);
    }

}

#endif
