#include "CWSocketManager.h"
#include "CWClientConnection.h"

#define KEY_SOCKET_CONNECTION_TEST "KEY_SOCKET_CONNECTION_TEST"

namespace cw {
    SocketManager::SocketManager() {
        _cc = new (std::nothrow) ClientConnection();
    }

    SocketManager::~SocketManager() {
        cocos2d::Scheduler *scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->unschedule(KEY_SOCKET_CONNECTION_TEST, this);

        CC_SAFE_DELETE(_cc);
    }

    void SocketManager::connectAsync(const char *ip, unsigned short port, const std::function<void (bool result)> &callback) {
        _cc->connentToServer(ip, port);

        cocos2d::Scheduler *scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->schedule([this, scheduler, callback](float) {
            if (_cc->isWaiting()) {
                return;
            }

            scheduler->unschedule(KEY_SOCKET_CONNECTION_TEST, this);
            if (_cc->isConnectSuccess()) {
                callback(true);
            } else {
                callback(false);
            }
        }, this, 0.0F, CC_REPEAT_FOREVER, 0.0F, false, KEY_SOCKET_CONNECTION_TEST);
    }

    int SocketManager::writeBuf(const char *data, int len) {
        return _cc->writeBuf(data, len);
    }

    int SocketManager::readBuf(char *data, int maxLen) {
        return _cc->readBuf(data, maxLen);
    }
}
