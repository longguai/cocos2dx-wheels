#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

#include "ClientConnection.h"

#include "cocos2d.h"

#define KEY_SOCKET_CONNECTION_TEST "KEY_SOCKET_CONNECTION_TEST"
#define KEY_SOCKET_PEEK "KEY_SOCKET_PEEK"

//#define SOCKET_MANAGER_DEBUG

namespace cw {

class SocketManager {
public:
    SocketManager() {
    }

    ~SocketManager() {
        cocos2d::Scheduler *scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->unschedule(KEY_SOCKET_CONNECTION_TEST, this);
        scheduler->unschedule(KEY_SOCKET_PEEK, this);
    }

    void connectToServer(const char *ip, unsigned short port, const std::function<void (bool result)> &callback) {
        _cc.connentToServer(ip, port);

        cocos2d::Scheduler *scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->schedule([this, scheduler, callback](float) {
            if (_cc.isWaiting()) {
                return;
            }

            scheduler->unschedule(KEY_SOCKET_CONNECTION_TEST, this);

            if (_cc.isConnectSuccess()) {
                callback(true);
                startPeek();
            } else {
                callback(false);
            }
        }, this, 0.0F, CC_REPEAT_FOREVER, 0.0F, false, KEY_SOCKET_CONNECTION_TEST);
    }

    void send(int protocal, const char *data, int size) {
        std::vector<char> buf(4 + size);
        writeProtocal(&buf[0], protocal);
        _cc.sendBuf(&buf[0], (int)buf.size());
    }

    template <class _FUNC>
    void sendAndRegisterRecvCallback(int protocal, const char *data, size_t size, _FUNC &&callback) {

        std::vector<char> buf(4 + size);
        writeProtocal(&buf[0], protocal);
        memcpy(&buf[4], data, size);
        _cc.sendBuf(&buf[0], (int)buf.size());

        _Helper::create([this, protocal, callback](_Helper *helper) {
            std::unordered_map<int, std::vector<char> >::iterator it = _packets.find(protocal);
            if (it != _packets.end()) {
                std::vector<char> buf(std::move(it->second));
                _packets.erase(it);

                helper->shutdown();
                helper->autorelease();
#ifdef SOCKET_MANAGER_DEBUG
                --_helperCnt;
                CCLOG("%d _helperCnt = %u", __LINE__, _helperCnt);
#endif
                callback(&buf[4], buf.size() - 4);
            }
        })->startup();
#ifdef SOCKET_MANAGER_DEBUG
        ++_helperCnt;
        CCLOG("%d _helperCnt = %u", __LINE__, _helperCnt);
#endif
    }

private:
    ClientConnection _cc;
    std::unordered_map<int, std::vector<char> > _packets;

    static void writeProtocal(char *buf, int protocal) {
        buf[0] = (protocal >> 24) & 0xFF;
        buf[1] = (protocal >> 16) & 0xFF;
        buf[2] = (protocal >>  8) & 0xFF;
        buf[3] = (protocal      ) & 0xFF;
    }

    static int readProtocal(const char *buf) {
        int protocal = (unsigned char)buf[0];
        protocal <<= 8;
        protocal |= (unsigned char)buf[1];
        protocal <<= 8;
        protocal |= (unsigned char)buf[2];
        protocal <<= 8;
        protocal |= (unsigned char)buf[3];
        return protocal;
    }

    void startPeek() {
        cocos2d::Scheduler *scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->schedule([this](float) {
            std::vector<char> buf;
            if (!_cc.peekBuf(&buf) || buf.size() < 4) {
                return;
            }
            
            int protocal = SocketManager::readProtocal(&buf[0]);
            _packets.insert(std::make_pair(protocal, std::move(buf)));
        }, this, 0.0F, CC_REPEAT_FOREVER, 0.0F, false, KEY_SOCKET_PEEK);
    }

#ifdef SOCKET_MANAGER_DEBUG
    unsigned _helperCnt = 0;
#endif

    class _Helper : public cocos2d::Ref {
    public:
        static _Helper *create(const std::function<void (_Helper *thiz)> &callback) {
            _Helper *ret = new (std::nothrow) _Helper();
            ret->_callback = callback;
            return ret;
        }

        void startup() {
            cocos2d::Director::getInstance()->getScheduler()->schedule(
                (cocos2d::SEL_SCHEDULE)&_Helper::update, this, 0.0F, CC_REPEAT_FOREVER, 0.0F, false);
        }

        void shutdown() {
            cocos2d::Director::getInstance()->getScheduler()->unschedule((cocos2d::SEL_SCHEDULE)&_Helper::update, this);
        }

        void update(float) {
            _callback(this);
        }

    private:
        std::function<void (_Helper *)> _callback;
    };
};

}

#endif
