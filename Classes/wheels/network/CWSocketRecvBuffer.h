#ifndef _CW_SOCKET_RECV_BUFFER_H_
#define _CW_SOCKET_RECV_BUFFER_H_

#include "CWCircularIOBuffer.h"

namespace cw {

    template <int _N> class SocketRecvBuffer final : protected CircularIOBuffer<_N> {

    protected:
        // xcode needs these
        using CircularIOBuffer<_N>::_buf;
        using CircularIOBuffer<_N>::_head;
        using CircularIOBuffer<_N>::_tail;

    public:
        using CircularIOBuffer<_N>::capacity;
        using CircularIOBuffer<_N>::isFull;
        using CircularIOBuffer<_N>::isEmpty;
        using CircularIOBuffer<_N>::size;
        using CircularIOBuffer<_N>::freeSize;
        using CircularIOBuffer<_N>::read;
        using CircularIOBuffer<_N>::peek;
        using CircularIOBuffer<_N>::skip;

        template <class _RECV> int doRecv(_RECV &&func) {
            if (isFull()) {
                return 0;
            }

            if (_tail >= _head) {
                if (_head == 0) {
                    int s = _N - _tail - 1;
                    int ret = func(_buf + _tail, s);
                    if (ret > 0) {
                        _tail += ret;
                    }
                    return ret;
                } else {
                    int s = _N - _tail;
                    int ret = func(_buf + _tail, s);
                    if (ret > 0) {
                        _tail += ret;
                        if (_tail >= _N) {
                            _tail -= _N;
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
