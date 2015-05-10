#include "CWRippleSprite.h"

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
        glGenBuffers(1, &_vertexBuffer);
        glGenBuffers(1, &_texCoordinateBuffer);
        glGenBuffers(1, &_colorBuffer);
        glGenBuffers(1, &_indexBuffer);
    }

    RippleSprite::~RippleSprite() {
        if (_vertexBuffer != 0) glDeleteBuffers(1, &_vertexBuffer);
        if (_texCoordinateBuffer != 0) glDeleteBuffers(1, &_texCoordinateBuffer);
        if (_colorBuffer != 0) glDeleteBuffers(1, &_colorBuffer);
        if (_indexBuffer != 0) glDeleteBuffers(1, &_indexBuffer);
    }

    void RippleSprite::_initRipple() {
        _gridSideLen = 8;// gridSideLen;

        _row = floorf(_contentSize.height / _gridSideLen) + 1;//add one more row to ensure buffer bigger than background pic
        _col = floorf(_contentSize.width / _gridSideLen) + 1;//add one more col to ensure buffer bigger than background pic

        //fill vertex attributes
        const int nVertex = (_row + 1) * (_col + 1);
        _vertices.reserve(nVertex * 2);//in order to faster push_back, do reserve
        _texCoordinates.reserve(nVertex * 2);
        _colorList.reserve(nVertex * 4);
        for (int i = 0; i < _row + 1; ++i) {
            for (int j = 0; j < _col + 1; ++j) {
                float x = _gridSideLen * j;
                float y = _contentSize.height - _gridSideLen * i;
                float s = x / _contentSize.width;
                float t = 1.0F - y / _contentSize.height;

                _vertices.push_back(x);
                _vertices.push_back(y);

                _texCoordinates.push_back(s);
                _texCoordinates.push_back(t);

                _colorList.push_back(1.0F);
                _colorList.push_back(1.0F);
                _colorList.push_back(1.0F);
                _colorList.push_back(1.0F);
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
                int vID_LU = i * nVertexPerRow + j;
                int vID_RU = vID_LU + 1;
                int vID_LD = vID_LU + nVertexPerRow;
                int vID_RD = vID_LD + 1;

                _indices.push_back(vID_LU);
                _indices.push_back(vID_LD);
                _indices.push_back(vID_RD);

                _indices.push_back(vID_LU);
                _indices.push_back(vID_RD);
                _indices.push_back(vID_RU);
            }
        }

        _texCoordinatesStore = _texCoordinates;

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordinateBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _texCoordinates.size(), &_texCoordinates[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _colorList.size(), &_colorList[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
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
        glEnable(GL_SCISSOR_TEST);

        //calculate world rect
        Rect lr = Rect(0, 0, _contentSize.width, _contentSize.height);
        Rect wr = RectApplyAffineTransform(lr, this->getNodeToWorldAffineTransform());

        //do not use glScissor, instead use cocos2dx api CCEGLView::setScissorInPoints, or the result will be wrong.
        //note: setScissorInPoints receive world space rect as parameter
        Director::getInstance()->getOpenGLView()->setScissorInPoints(wr.getMinX(), wr.getMinY(), wr.size.width, wr.size.height);

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

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordinateBuffer);
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
        glDisable(GL_SCISSOR_TEST);

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
        float x_OLU = np.x;//origin at left up corner
        float y_OLU = _contentSize.height - np.y;//origin at left up corner

        //position range in OLU space
        float xmin = x_OLU - radius;
        float xmax = x_OLU + radius;
        float ymin = y_OLU - radius;
        float ymax = y_OLU + radius;

        //calculate index range from position range
        //note: min is floor(x), max is ceil ceil(x)-1
        int imin, imax, jmin, jmax;
        int nRow = _row + 1;
        int nCol = _col + 1;
        const int imargin = 1;  //do not let the vertex on the edge to be pressed
        const int jmargin = 1;  //do not let the vertex on the edge to be pressed
        imin = std::min(imargin, (int)floorf(ymin / _gridSideLen));
        imax = std::min(nRow - 1 - imargin, (int)ceilf(ymax / _gridSideLen) - 1);
        jmin = std::min(jmargin, (int)floorf(xmin / _gridSideLen));
        jmax = std::min(nCol - 1 - jmargin, (int)ceilf(xmax / _gridSideLen) - 1);

        //iterate all vertex in range [imin,imax]x[jmin,jmax], and press them
        for (int i = imin; i <= imax; ++i) {
            for (int j = jmin; j <= jmax; ++j) {
                int idx = (i * nCol + j) * 2;
                const Vec2 v = Vec2(_vertices[idx], _vertices[idx + 1]);
                const Vec2 v_OLU = Vec2(v.x, _contentSize.height - v.y);
                //press m_srcBuffer at point v
                float dis = v_OLU.distance(Vec2(x_OLU, y_OLU));
                if (dis < radius) {
                    float curDepth = depth * (0.5F + 0.5F * cosf(dis * M_PI / radius));
                    _srcMat[i * nRow + j] -= curDepth;
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
        float k = 0.5F - 0.5F / _rippleStrength;
        float kTexCoord = 1.0F / 1048;
        const int nRow = _row + 1;
        const int nCol = _col + 1;
        for (int i = 1; i < _row; ++i){
            for (int j = 1; j < _col; ++j){
                //update m_dstBuffer
                GLfloat Hup_src = _srcMat[(i - 1) * nRow + j];
                GLfloat Hdn_src = _srcMat[(i + 1) * nRow + j];
                GLfloat Hleft_src = _srcMat[i * nRow + j - 1];
                GLfloat Hright_src = _srcMat[i * nRow + j + 1];
                GLfloat Hcenter_dst = _dstMat[i * nRow + j];
                GLfloat H = (Hup_src + Hdn_src + Hleft_src + Hright_src - 2 * Hcenter_dst) * k;
                _dstMat[i * nRow + j] = H;

                //update texCoord
                float s_offset = (Hup_src - Hdn_src) * kTexCoord;
                float t_offset = (Hleft_src - Hright_src) * kTexCoord;
                const int idx = (i * nCol + j) * 2;
                _texCoordinates[idx] = _texCoordinatesStore[idx] + s_offset;
                _texCoordinates[idx + 1] = _texCoordinatesStore[idx + 1] + t_offset;
            }
        }

        std::swap(_dstMat, _srcMat);

        //resubmit texCoord
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordinateBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _texCoordinates.size(), &_texCoordinates[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
