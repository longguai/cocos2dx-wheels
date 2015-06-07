#ifndef _FUNCTION_TRAITS_H_
#define _FUNCTION_TRAITS_H_

#include <tuple>
#include <functional>
#include <type_traits>
#include <stddef.h>

template <class _FUNC>
struct function_traits : function_traits<decltype(&_FUNC::operator())> {
};

template <class _RET, class ..._ARGS>
struct function_traits<_RET (_ARGS...)> {
    typedef _RET return_type;
    static const size_t argc = sizeof...(_ARGS);
    template <size_t i> struct argv {
        typedef typename std::tuple_element<i, std::tuple<_ARGS...> >::type type;
    };
};

template <class _RET, class ..._ARGS>
struct function_traits<_RET (*)(_ARGS...)> : function_traits<_RET (_ARGS...)> {
};

template <class _RET, class _OBJ, class ..._ARGS>
struct function_traits<_RET (_OBJ::*)(_ARGS...)> : function_traits<_RET (_ARGS...)> {
};

template <class _RET, class _OBJ, class ..._ARGS>
struct function_traits<_RET (_OBJ::*)(_ARGS...) const> : function_traits<_RET (_ARGS...)> {
};

template <class _RET, class _OBJ, class ..._ARGS>
struct function_traits<_RET (_OBJ::*)(_ARGS...) volatile> : function_traits<_RET (_ARGS...)> {
};

template <class _RET, class _OBJ, class ..._ARGS>
struct function_traits<_RET (_OBJ::*)(_ARGS...) const volatile> : function_traits<_RET (_ARGS...)> {
};

template <class _SIGNATURE>
struct function_traits<std::function<_SIGNATURE> > : function_traits<_SIGNATURE> {
};

template <class _FUNC>
struct function_traits<_FUNC &> : function_traits<_FUNC>{
};

template <class _FUNC>
struct function_traits<const _FUNC &> : function_traits<_FUNC> {
};

template <class _FUNC>
struct function_traits<volatile _FUNC &> : function_traits<_FUNC> {
};

template <class _FUNC>
struct function_traits<const volatile _FUNC &> : function_traits<_FUNC> {
};

template <class _FUNC>
struct function_traits<_FUNC &&> : function_traits<_FUNC>{
};

template <class _FUNC>
struct function_traits<const _FUNC &&> : function_traits<_FUNC> {
};

template <class _FUNC>
struct function_traits<volatile _FUNC &&> : function_traits<_FUNC> {
};

template <class _FUNC>
struct function_traits<const volatile _FUNC &&> : function_traits<_FUNC> {
};

#endif
