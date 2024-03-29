﻿#include "CWRippleSprite.h"
#include "base/CCDirector.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGLProgram.h"

namespace cw {

    RippleSprite *RippleSprite::create(const std::string &filename) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithFile(filename)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    RippleSprite *RippleSprite::create(const std::string &filename, const cocos2d::Rect &rect) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithFile(filename, rect)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    RippleSprite *RippleSprite::createWithTexture(cocos2d::Texture2D *texture) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithTexture(texture)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    RippleSprite *RippleSprite::createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated /* = false*/) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithTexture(texture, rect, rotated)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    RippleSprite *RippleSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithSpriteFrame(spriteFrame)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    RippleSprite *RippleSprite::createWithSpriteFrameName(const std::string &spriteFrameName) {
        RippleSprite *ret = new (std::nothrow) RippleSprite();
        if (ret != nullptr && ret->initWithSpriteFrameName(spriteFrameName)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool RippleSprite::init() {
        if (!cocos2d::Sprite::init()) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithTexture(cocos2d::Texture2D *texture) {
        if (!cocos2d::Sprite::initWithTexture(texture)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect) {
        if (!cocos2d::Sprite::initWithTexture(texture, rect)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect, bool rotated) {
        if (!cocos2d::Sprite::initWithTexture(texture, rect, rotated)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame) {
        if (!cocos2d::Sprite::initWithSpriteFrame(spriteFrame)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithSpriteFrameName(const std::string &spriteFrameName) {
        if (!cocos2d::Sprite::initWithSpriteFrameName(spriteFrameName)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithFile(const std::string &filename) {
        if (!cocos2d::Sprite::initWithFile(filename)) {
            return false;
        }
        _initRipple();
        return true;
    }

    bool RippleSprite::initWithFile(const std::string &filename, const cocos2d::Rect &rect) {
        if (!cocos2d::Sprite::initWithFile(filename, rect)) {
            return false;
        }
        _initRipple();
        return true;
    }

    RippleSprite::RippleSprite() {
    }

    RippleSprite::~RippleSprite() {
        if (_vertexBuffer != 0) {
            glDeleteBuffers(1, &_vertexBuffer);
        }
        if (_texCoordBuffer != 0) {
            glDeleteBuffers(1, &_texCoordBuffer);
        }
        if (_colorBuffer != 0) {
            glDeleteBuffers(1, &_colorBuffer);
        }
        if (_indexBuffer != 0) {
            glDeleteBuffers(1, &_indexBuffer);
        }
    }

    void RippleSprite::_initRipple() {
        if (_vertexBuffer != 0) {
            glDeleteBuffers(1, &_vertexBuffer);
        }
        if (_texCoordBuffer != 0) {
            glDeleteBuffers(1, &_texCoordBuffer);
        }
        if (_colorBuffer != 0) {
            glDeleteBuffers(1, &_colorBuffer);
        }
        if (_indexBuffer != 0) {
            glDeleteBuffers(1, &_indexBuffer);
        }

        glGenBuffers(1, &_vertexBuffer);
        glGenBuffers(1, &_texCoordBuffer);
        glGenBuffers(1, &_colorBuffer);
        glGenBuffers(1, &_indexBuffer);

        _gridSideLen = 8;// gridSideLen;

        _row = floorf(_contentSize.height / _gridSideLen) + 1;//add one more row to ensure buffer bigger than background pic
        _col = floorf(_contentSize.width / _gridSideLen) + 1;//add one more col to ensure buffer bigger than background pic

        //fill vertex attributes
        const int nVertex = (_row + 1) * (_col + 1);
        _vertices.reserve(nVertex * 2);//in order to faster push_back, do reserve
        _texCoords.reserve(nVertex * 2);
        _colors.reserve(nVertex * 4);
        for (int i = 0; i < _row + 1; ++i) {
            for (int j = 0; j < _col + 1; ++j) {
                const GLfloat x = _gridSideLen * j;
                const GLfloat y = _contentSize.height - _gridSideLen * i;
                const GLfloat u = x / _contentSize.width;
                const GLfloat v = 1.0F - y / _contentSize.height;

                _vertices.push_back(x);
                _vertices.push_back(y);

                _texCoords.push_back(u);
                _texCoords.push_back(v);

                _colors.push_back(1.0F);  // r
                _colors.push_back(1.0F);  // g
                _colors.push_back(1.0F);  // b
                _colors.push_back(1.0F);  // a
            }
        }

        //fill indexs
        _indices.reserve(_row * _col * 6);
        const int nVertexPerRow = _col + 1;
        for (int i = 0; i < _row; ++i) {
            for (int j = 0; j < _col; ++j) {
                //current grid is grid(i,j)
                //grid(i,j)'s leftup vertex is vertex(i,j)
                //vertex(i,j) is vertex(i*nVertexPerRow+j)
                const GLushort tl = i * nVertexPerRow + j;
                const GLushort tr = tl + 1;
                const GLushort bl = tl + nVertexPerRow;
                const GLushort br = bl + 1;

                _indices.push_back(tl);
                _indices.push_back(bl);
                _indices.push_back(br);

                _indices.push_back(tl);
                _indices.push_back(br);
                _indices.push_back(tr);
            }
        }

        _texCoordsStore = _texCoords;

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(GLfloat), &_texCoords[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(GLfloat), &_colors[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), &_indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        _srcMat.resize(nVertex);
        _dstMat.resize(nVertex);

        setGLProgramState(cocos2d::GLProgramState::getOrCreateWithGLProgramName(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    }

    void RippleSprite::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
        _customCommand.init(_globalZOrder);
        _customCommand.func = std::bind(&RippleSprite::onDraw, this, transform);
        renderer->addCommand(&_customCommand);
    }

    void RippleSprite::onDraw(const cocos2d::Mat4 &transform) {
        using namespace cocos2d;

        //use scissor test is more fast than clippingNode for large rect.
        //enable scissor test
        //glEnable(GL_SCISSOR_TEST);

        //calculate world rect
        Rect lr = Rect(0, 0, _contentSize.width, _contentSize.height);
        Rect wr = RectApplyAffineTransform(lr, this->getNodeToWorldAffineTransform());

        //do not use glScissor, instead use cocos2dx api CCEGLView::setScissorInPoints, or the result will be wrong.
        //note: setScissorInPoints receive world space rect as parameter
        //Director::getInstance()->getOpenGLView()->setScissorInPoints(wr.getMinX(), wr.getMinY(), wr.size.width, wr.size.height);

        //----change shader
        GL::blendFunc(_blendFunc.src, _blendFunc.dst);

        //pass values for cocos2d-x build-in uniforms
        GLProgram *program = this->getGLProgram();
        program->use();

        //when program is have MV, we should use program->setUniformsForBuiltins(transform) instead of program->setUniformsForBuiltins()
        program->setUniformsForBuiltins(transform);

        //enable attributes
        GLint isAttribPositionOn;
        glGetVertexAttribiv(GLProgram::VERTEX_ATTRIB_POSITION, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isAttribPositionOn);

        GLint isAttribColorOn;
        glGetVertexAttribiv(GLProgram::VERTEX_ATTRIB_COLOR, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isAttribColorOn);

        GLint isAttribTexCoordOn;
        glGetVertexAttribiv(GLProgram::VERTEX_ATTRIB_TEX_COORD, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isAttribTexCoordOn);

        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);

        //bindTexture
        GL::bindTexture2D(_texture->getName());

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        //the third parameter of glDrawElements must be GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE
        //or there will be OpenGL error 0x0500
        //http://blog.csdn.net/zhucde/article/details/3547442
        //http://www.khronos.org/opengles/sdk/docs/man/xhtml/glDrawElements.xml
        //http://www.opengl.org/wiki/OpenGL_Error
        //here we use GL_UNSIGNED_SHORT
        glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GL::bindTexture2D(0);

        //disable attributes
        if (isAttribPositionOn) glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        else glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        if (isAttribColorOn) glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        else glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        if (isAttribTexCoordOn) glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        else glDisableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);

        //disable scissor test
        //glDisable(GL_SCISSOR_TEST);

        CHECK_GL_ERROR_DEBUG();
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _vertices.size());
    }

    void RippleSprite::touch(const cocos2d::Vec2 &pt, float depth, float radius) {
        using namespace cocos2d;

        //touchPoint is in parent space, convert it to local space
        //in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
        this->getNodeToParentTransform();

        AffineTransform parentToNodeTransform = this->getParentToNodeAffineTransform();
        Vec2 np = PointApplyAffineTransform(pt, parentToNodeTransform);

        //convert np to OLU (origin at left up corner) space
        const float x_OLU = np.x;//origin at left up corner
        const float y_OLU = _contentSize.height - np.y;//origin at left up corner

        //position range in OLU space
        const float xmin = x_OLU - radius;
        const float xmax = x_OLU + radius;
        const float ymin = y_OLU - radius;
        const float ymax = y_OLU + radius;

        //calculate index range from position range
        //note: min is floor(x), max is ceil ceil(x)-1
        const int nRow = _row + 1;
        const int nCol = _col + 1;
        const int imargin = 1;  //do not let the vertex on the edge to be pressed
        const int jmargin = 1;  //do not let the vertex on the edge to be pressed
        const int imin = std::max(imargin, (int)floorf(ymin / _gridSideLen));
        const int imax = std::min(nRow - 1 - imargin, (int)ceilf(ymax / _gridSideLen) - 1);
        const int jmin = std::max(jmargin, (int)floorf(xmin / _gridSideLen));
        const int jmax = std::min(nCol - 1 - jmargin, (int)ceilf(xmax / _gridSideLen) - 1);

        //iterate all vertex in range [imin,imax]x[jmin,jmax], and press them
        for (int i = imin; i <= imax; ++i) {
            for (int j = jmin; j <= jmax; ++j) {
                const int idx = (i * nCol + j) * 2;
                const Vec2 v = Vec2(_vertices[idx], _vertices[idx + 1]);
                const Vec2 v_OLU = Vec2(v.x, _contentSize.height - v.y);
                //press m_srcBuffer at point v
                const float dis = v_OLU.distance(Vec2(x_OLU, y_OLU));
                if (dis < radius) {
                    const float curDepth = depth * (0.5F + 0.5F * cosf(dis * M_PI / radius));
                    _srcMat[i * nCol + j] -= curDepth;
                }
            }
        }
    }

    void RippleSprite::update(float dt) {
        _time += dt;
        if (_time < 1.0F / 45) {
            return;
        }
        _time = 0;

        using namespace cocos2d;

        //update buffer and mesh
        const float k = 0.5F - 0.5F / _rippleStrength;
        const float kTexCoord = 1.0F / 1024;
        const int nCol = _col + 1;
        for (int i = 1; i < _row; ++i) {
            for (int j = 1; j < _col; ++j) {
                //update m_dstBuffer
                const GLfloat HTSrc = _srcMat[(i - 1) * nCol + j];
                const GLfloat HBSrc = _srcMat[(i + 1) * nCol + j];
                const GLfloat HLSrc = _srcMat[i * nCol + j - 1];
                const GLfloat HRSrc = _srcMat[i * nCol + j + 1];
                const GLfloat HCenterDst = _dstMat[i * nCol + j];
                const GLfloat H = (HTSrc + HBSrc + HLSrc + HRSrc - 2 * HCenterDst) * k;
                _dstMat[i * nCol + j] = H;

                //update texCoord
                const float offsetV = (HTSrc - HBSrc) * kTexCoord;
                const float offsetH = (HLSrc - HRSrc) * kTexCoord;
                const int idx = (i * nCol + j) * 2;
                _texCoords[idx] = _texCoordsStore[idx] + offsetV;
                _texCoords[idx + 1] = _texCoordsStore[idx + 1] + offsetH;
            }
        }

        std::swap(_dstMat, _srcMat);

        //resubmit texCoord
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(GLfloat), &_texCoords[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
