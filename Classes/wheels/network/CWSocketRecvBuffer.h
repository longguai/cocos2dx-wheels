#ifndef _CW_SOCKET_RECV_BUFFER_H_
#define _CW_SOCKET_RECV_BUFFER_H_

#include "CWCircularIOBuffer.h"

namespace cw {

    template <int _Size> class SocketRecvBuffer final : protected CircularIOBuffer<_Size> {

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
        using CircularIOBuffer<_Size>::read;
        using CircularIOBuffer<_Size>::peek;
        using CircularIOBuffer<_Size>::skip;

        template <class _RECV> int doRecv(_RECV &&func) {
            if (isFull()) {
                return 0;
            }

            if (_tail >= _head) {
                if (_head == 0) {
                    int s = _Size - _tail - 1;
                    int ret = func(_buf + _tail, s);
                    if (ret > 0) {
                        _tail += ret;
                    }
                    return ret;
                } else {
                    int s = _Size - _tail;
                    int ret = func(_buf + _tail, s);
                    if (ret > 0) {
                        _tail += ret;
                        if (_tail >= _Size) {
                            _tail -= _Size;
                        }
                    }
                    return ret;
                }
            } else {
                int s = _head - _tail - 1;
                int ret = func(_buf + _tail, s);
                if (ret > 0) {
                    _tail += ret;
                }
                return ret;
            }
        }
    };

}

#endif
