#ifndef _NETWORK_TEST_H_
#define _NETWORK_TEST_H_

#include "cocos2d.h"

namespace cw {
    class SocketManager;
}

class NetworkTestLayer : public cocos2d::Layer {
public:
    NetworkTestLayer();
    virtual ~NetworkTestLayer();

    virtual bool init() override;
    CREATE_FUNC(NetworkTestLayer)

private:
    cw::SocketManager *_sm;
};

#endif
