#ifndef _CW_SOCKET_MANAGER_H_
#define _CW_SOCKET_MANAGER_H_

#include "cocos2d.h"

namespace cw {

    class ClientConnection;

    class SocketManager final {
    public:
        SocketManager();
        ~SocketManager();

        void connectAsync(const char *ip, unsigned short port, const std::function<void (bool result)> &callback);
        int writeBuf(const char *data, int len);
        int readBuf(char *data, int maxLen);

    private:
        ClientConnection *_cc = nullptr;
    };
}

#endif
