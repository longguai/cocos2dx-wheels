#ifndef _CIRCULAR_IO_BUFFER_H_
#define _CIRCULAR_IO_BUFFER_H_

#include <string.h>
#include <assert.h>

namespace cw {

    template <int _Size> class CircularIOBuffer {

    protected:
        char _buf[_Size];
        int _head;
        int _tail;

    public:
        static const int capacity = _Size - 1;

        CircularIOBuffer<_Size>() : _head(0), _tail(0) {
            static_assert(_Size > 1, "_Size must greater than 1");
        }

        inline bool isFull() const {
            return (_tail + 1 == _head || _tail + 1 - _Size == _head);
        }

        inline bool isEmpty() const {
            return (_head == _tail);
        }

        inline int size() {
            if (_head <= _tail) {
                return _tail - _head;
            } else {
                return _Size - _head + _tail;
            }
        }

        inline int freeSize() {
            if (_head <= _tail) {
                return _Size - 1 - _tail + _head;
            } else {
                return _head - _tail - 1;
            }
        }

        int read(char *data, int len) {
            if (isEmpty()) {
                return 0;
            }

            if (_tail > _head) {
                int s = _tail - _head;
                if (s >= len) {
                    memcpy(data, _buf + _head, len);
                    _head += len;
                    return len;
                } else {
                    memcpy(data, _buf + _head, s);
                    _head += s;
                    return s;
                }
            } else {
                int s1 = _Size - _head;
                if (s1 >= len) {
                    memcpy(data, _buf + _head, len);
                    _head += len;
                    if (_head == _Size) {
                        _head = 0;
                    }
                    return len;
                } else {
                    if (s1 > 0) {
                        memcpy(data, _buf + _head, s1);
                        _head += s1;
                        return s1 + read(data + s1, len - s1);
                    } else {
                        _head = 0;
                        return read(data, len);
                    }
                }
            }
        }

        int _peek(int head, char *data, int len) const {
            if (head == _tail) {
                return 0;
            }

            if (_tail > head) {
                int s = _tail - head;
                if (s >= len) {
                    memcpy(data, _buf + head, len);
                    return len;
                } else {
                    memcpy(data, _buf + head, s);
                    return s;
                }
            } else {
                int s1 = _Size - head;
                if (s1 >= len) {
                    memcpy(data, _buf + head, len);
                    return len;
                } else {
                    if (s1 > 0) {
                        memcpy(data, _buf + head, s1);
                        return s1 + _peek(head + s1, data + s1, len - s1);
                    } else {
                        return _peek(0, data, len);
                    }
                }
            }
        }

        inline int peek(char *data, int len) const {
            return _peek(_head, data, len);
        }

        int skip(int len) {
            if (isEmpty()) {
                return 0;
            }

            if (_tail > _head) {
                int s = _tail - _head;
                if (s >= len) {
                    _head += len;
                    return len;
                } else {
                    _head += s;
                    return s;
                }
            } else {
                int s1 = _Size - _head;
                if (s1 >= len) {
                    _head += len;
                    if (_head == _Size) {
                        _head = 0;
                    }
                    return len;
                } else {
                    if (s1 > 0) {
                        _head += s1;
                        return s1 + skip(len - s1);
                    } else {
                        _head = 0;
                        return skip(len);
                    }
                }
            }
        }

        int write(const char *data, int len) {
            if (isFull()) {
                return 0;
            }

            if (_tail >= _head) {
                int s = _Size - _tail - 1;
                if (s >= len) {
                    memcpy(_buf + _tail, data, len);
                    _tail += len;
                    return len;
                } else {
                    if (s > 0) {
                        memcpy(_buf + _tail, data, s);
                        _tail += s;
                        return s + write(data + s, len - s);
                    } else {
                        _buf[_tail] = *data;
                        _tail = 0;
                        return 1 + write(data + 1, len - 1);
                    }
                }
            } else {
                int s = _head - _tail - 1;
                if (s >= len) {
                    s = len;
                }
                memcpy(_buf + _tail, data, s);
                _tail += s;
                return s;
            }
        }

        template <size_t _Size1> int write(const char (&data)[_Size1]) {
            return write(data, _Size1);
        }

        template <size_t _Size1> int read(char (&data)[_Size1]) {
            return read(data, _Size1);
        }
    };
}

#endif
