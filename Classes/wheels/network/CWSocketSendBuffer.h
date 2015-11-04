#ifndef _CW_SOCKET_SEND_BUFFER_H_
#define _CW_SOCKET_SEND_BUFFER_H_

#include "CWCircularIOBuffer.h"

namespace cw {

    template <int _Size> class SocketSendBuffer final : protected CircularIOBuffer<_Size> {

    protected:
        // xcode needs these
        using CircularIOBuffer<_Size>::_buf;
        using CircularIOBuffer<_Size>::_head;
        using CircularIOBuffer<_Size>::_tail;

    public:
        using CircularIOBuffer<_Size>::capacity;
        using CircularIOBuffer<_Size>::isFull;
        using CircularIOBuffer<_Size>::isEmpty;
        using CircularIOBuffer<_Size>::size;
        using CircularIOBuffer<_Size>::freeSize;
        using CircularIOBuffer<_Size>::write;

        template <class _SEND> int doSend(_SEND &&func) {
            if (_head == _tail) {
                return 0;
            }

            if (_head <= _tail) {
                int s = _tail - _head;
                int ret = func(_buf + _head, s);
                if (ret > 0) {
                    _head += ret;
                }
                return ret;
            } else {
                int s = _Size - _head;
                int ret = func(_buf + _head, s);
                if (ret > 0) {
                    _head += ret;
                    if (_head >= _Size) {
                        _head -= _Size;
                    }
                }
                return ret;
            }
        }
    };
}

#endif
