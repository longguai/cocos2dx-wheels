#ifndef __cocos2dx_wheels__CWRippleSprite__
#define __cocos2dx_wheels__CWRippleSprite__

#include "2d/CCSprite.h"

namespace cw {

    class RippleSprite : public cocos2d::Sprite {
    public:
        RippleSprite();
        virtual ~RippleSprite();

        CREATE_FUNC(RippleSprite);

        static RippleSprite *create(const std::string &filename);
        static RippleSprite *create(const std::string &filename, const cocos2d::Rect &rect);
        static RippleSprite *createWithTexture(cocos2d::Texture2D *texture);
        static RippleSprite *createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated = false);
        static RippleSprite *createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame);
        static RippleSprite *createWithSpriteFrameName(const std::string &spriteFrameName);

        virtual bool init() override;
        virtual bool initWithTexture(cocos2d::Texture2D *texture) override;
        virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect) override;
        virtual bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated) override;
        virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame) override;
        virtual bool initWithSpriteFrameName(const std::string &spriteFrameName) override;
        virtual bool initWithFile(const std::string &filename) override;
        virtual bool initWithFile(const std::string &filename, const cocos2d::Rect &rect) override;

        virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
        void onDraw(const cocos2d::Mat4 &transform);
        void touch(const cocos2d::Vec2 &pt, float depth, float radius);

        virtual void update(float dt);

    protected:
        void _initRipple();

        cocos2d::CustomCommand _customCommand;

        float _gridSideLen = 0.0F;
        int _row = 0;  // grid row count
        int _col = 0;  // grid col count
        std::vector<GLfloat> _texCoordinatesStore;
        float _rippleStrength = 8;
        float _time = 0.0F;

        std::vector<GLfloat> _vertices;
        std::vector<GLfloat> _texCoordinates;
        std::vector<GLfloat> _colorList;
        std::vector<GLushort> _indices;

        GLuint _vertexBuffer = 0;
        GLuint _texCoordinateBuffer = 0;
        GLuint _indexBuffer = 0;
        GLuint _colorBuffer = 0;

        std::vector<GLfloat> _srcMat;
        std::vector<GLfloat> _dstMat;
    };
}

#endif /* defined(__cocos2dx_wheels__CWRippleSprite__) */
