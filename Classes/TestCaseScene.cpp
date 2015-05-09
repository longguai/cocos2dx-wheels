#include "TestCaseScene.h"
#include <functional>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "tests/ArrangeTest.h"
#include "tests/InConvexPolygonTest.h"
#include "tests/RichTextExTest.h"
#include "tests/TableViewTest.h"
#include "tests/FoldableTest.h"
#include "tests/CoordinateSystemTest.h"
#include "tests/NetworkTest.h"

namespace cw {
#pragma pack(push, 2)
    struct IDTriangle {
        short id[3];
    };
#pragma pack(pop)

    class RippleSprite : public cocos2d::Sprite {
    public:
        RippleSprite();
        virtual ~RippleSprite();
        CREATE_FUNC(RippleSprite);

        virtual bool init() override;

        virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
        void onDraw(const cocos2d::Mat4 &transform);
        void touch(const cocos2d::Vec2 &pt, float depth, float radius);

        virtual void update(float dt);

    protected:
        cocos2d::CustomCommand _customCommand;

        float _gridSideLen = 0.0F;
        int _row = 0;  // grid row count
        int _col = 0;  // grid col count
        std::vector<cocos2d::Vec2> _texCoordListStore;
        float _rippleStrength = 8;
        float _time = 0.0F;

        std::vector<cocos2d::Vec2> _vlist;
        std::vector<cocos2d::Vec2> _texCoordList;
        std::vector<cocos2d::Color4F> _colorList;
        std::vector<IDTriangle> _IDTriangleList;

        GLuint _posBuffer = 0;
        GLuint _texCoordBuffer = 0;
        GLuint _indexBuffer = 0;
        GLuint _colorBuffer = 0;

        std::vector<std::vector<float> > _srcMat;
        std::vector<std::vector<float> > _dstMat;
    };

    RippleSprite::RippleSprite() {
        glGenBuffers(1, &_posBuffer);
        glGenBuffers(1, &_texCoordBuffer);
        glGenBuffers(1, &_indexBuffer);
        glGenBuffers(1, &_colorBuffer);
    }

    RippleSprite::~RippleSprite() {
        if (_posBuffer != 0) glDeleteBuffers(1, &_posBuffer);
        if (_texCoordBuffer != 0) glDeleteBuffers(1, &_texCoordBuffer);
        if (_indexBuffer != 0) glDeleteBuffers(1, &_indexBuffer);
        if (_colorBuffer != 0) glDeleteBuffers(1, &_colorBuffer);
    }

    bool RippleSprite::init() {
        using namespace cocos2d;

        if (!Sprite::initWithFile("HelloWorld.png")) {
            return false;
        }

        _gridSideLen = 8;// gridSideLen;
        //----init this sprite
        //this->CCSprite::initWithFile(texFileName.c_str());

        _row = floorf(_contentSize.height / _gridSideLen) + 1;//add one more row to ensure buffer bigger than background pic
        _col = floorf(_contentSize.width / _gridSideLen) + 1;//add one more col to ensure buffer bigger than background pic

        //fill vertex attributes
        //nVertex should be (row+1)*(col+1)
        int nVertex = (_row + 1) * (_col + 1);
        _vlist.reserve(nVertex);//in order to faster push_back, do reserve
        _texCoordList.reserve(nVertex);
        _colorList.reserve(nVertex);
        for (int i = 0; i < _row + 1; ++i) {
            for (int j = 0; j < _col + 1; ++j) {
                float x = _gridSideLen * j;
                float y = _contentSize.height - _gridSideLen * i;
                float s = x / _contentSize.width;
                float t = 1.0F - y / _contentSize.height;

                _vlist.push_back(Vec2(x, y));
                _texCoordList.push_back(Vec2(s, t));
                _colorList.push_back(Color4F(1.0F, 1.0F, 1.0F, 1.0F));
            }
        }

        //fill indexs
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
                IDTriangle IDtri1 = { vID_LU, vID_LD, vID_RD };
                IDTriangle IDtri2 = { vID_LU, vID_RD, vID_RU };

                //current grid = IDtri1+IDtri2
                _IDTriangleList.push_back(IDtri1);
                _IDTriangleList.push_back(IDtri2);
            }
        }

        //----_texCoordListStore
        _texCoordListStore = _texCoordList;

        //----submit mesh
        glBindBuffer(GL_ARRAY_BUFFER, _posBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * _vlist.size(), &_vlist.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * _texCoordList.size(), &_texCoordList.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Color4F) * _colorList.size(), &_colorList.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short [3]) * _IDTriangleList.size(), &_IDTriangleList.front(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //----generate srcBuffer and dstBuffer

        //each element of srcBuffer or dstBuffer correspond to a vertex, present the vertex's height
        //so srcBuffer and dstBuffer are all (row+1)*(col+1) size
        //----srcBuffer
        _srcMat.resize(_row + 1);
        for (size_t i = 0; i < _srcMat.size(); ++i) {
            _srcMat[i].resize(_col + 1);
        }

        //----dstBuffer
        _dstMat.resize(_row + 1);
        for (size_t i = 0; i < _dstMat.size(); ++i) {
            _dstMat[i].resize(_col + 1);
        }

        //change shader to have_mv
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

        return true;
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
        ::glEnable(GL_SCISSOR_TEST);
        //scissor test
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
        ////program->setUniformsForBuiltins();
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

        glBindBuffer(GL_ARRAY_BUFFER, _posBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        //the third parameter of glDrawElements must be GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE
        //or there will be OpenGL error 0x0500
        //http://blog.csdn.net/zhucde/article/details/3547442
        //http://www.khronos.org/opengles/sdk/docs/man/xhtml/glDrawElements.xml
        //http://www.opengl.org/wiki/OpenGL_Error
        //here we use GL_UNSIGNED_SHORT
        glDrawElements(GL_TRIANGLES, _IDTriangleList.size() * 3, GL_UNSIGNED_SHORT, 0);
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
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _IDTriangleList.size() * 3);
    }

    void RippleSprite::touch(const cocos2d::Vec2 &pt, float depth, float radius) {
        using namespace cocos2d;

        //touchPoint is in parent space, convert it to local space
        this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
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
        for (int i = imin; i <= imax; ++i){
            for (int j = jmin; j <= jmax; ++j) {
                const Vec2 &v = _vlist[i*nCol + j];
                const Vec2 v_OLU = Vec2(v.x, _contentSize.height - v.y);
                //press m_srcBuffer at point v
                float dis = v_OLU.distance(Vec2(x_OLU, y_OLU));
                if (dis < radius) {
                    float curDepth = depth * (0.5F + 0.5F * cosf(dis * M_PI / radius));
                    _srcMat[i][j] -= curDepth;
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
        for (int i = 1; i < nRow - 1; ++i){
            for (int j = 1; j < nCol - 1; ++j){
                //update m_dstBuffer
                float Hup_src = _srcMat[i - 1][j];
                float Hdn_src = _srcMat[i + 1][j];
                float Hleft_src = _srcMat[i][j - 1];
                float Hright_src = _srcMat[i][j + 1];
                float Hcenter_dst = _dstMat[i][j];
                float H = (Hup_src + Hdn_src + Hleft_src + Hright_src - 2 * Hcenter_dst)*k;
                _dstMat[i][j] = H;

                //update texCoord
                float s_offset = (Hup_src - Hdn_src) * kTexCoord;
                float t_offset = (Hleft_src - Hright_src) * kTexCoord;
                Vec2 &texCoord = _texCoordList[i * nCol + j];
                Vec2 &texCoordStore = _texCoordListStore[i * nCol + j];
                texCoord.x = texCoordStore.x + s_offset;
                texCoord.y = texCoordStore.y + t_offset;
            }
        }

        std::swap(_dstMat, _srcMat);

        //resubmit texCoord
        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * _texCoordList.size(), &_texCoordList.front(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

USING_NS_CC;

static struct {
    std::string title;
    std::function<Layer *()> createFunc;
} sTestCases[] = {
    { "RichTextEx Test", &RichTextExTestLayer::create },
    { "TableView Test", &TableViewTestLayer::create },
    { "Arrange Test", &ArrangeTestLayer::create },
    { "InConvexPolygon Test", &InConvexPolygonTestLayer::create },
    { "Foldable Test", &FoldableTestLayer::create },
    { "CoordinateSystem Test", &CoordinateSystemLayer::create },
    { "Network Test", &NetworkTestLayer::create }
};

bool TestCaseScene::init() {
    if (!Scene::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    Vec2 midPos = Vec2(winSize.width * 0.5, winSize.height * 0.5);

    auto rs = cw::RippleSprite::create();
    this->addChild(rs);
    rs->setPosition(midPos);
    rs->runAction(RepeatForever::create(Sequence::create(CallFunc::create([rs, midPos]() {
        rs->touch(midPos, 512, 12);
    }), DelayTime::create(2), nullptr)));
    rs->scheduleUpdate();

    return true;

    Node *innerNode = Node::create();
    float height = 0.0F;
    size_t len = sizeof(sTestCases) / sizeof(*sTestCases);
    for (size_t j = len; j > 0; --j) {
        ui::Button *button = ui::Button::create();
        button->setTitleText(sTestCases[j - 1].title);
        button->setTitleFontSize(32);
        button->setScale9Enabled(true);

        innerNode->addChild(button);
        button->setPosition(Vec2(winSize.width * 0.5, 50 * (len - j) + 30));
        height += 50;

        button->addTouchEventListener([this, innerNode, j](Ref *, ui::Widget::TouchEventType type) {
            if (type != ui::Widget::TouchEventType::ENDED) {
                return;
            }
            Size winSize = Director::getInstance()->getWinSize();

            _innerNode->removeFromParentAndCleanup(true);
            Layer *layer = sTestCases[j - 1].createFunc();
            this->addChild(layer);

            Label *titleLabel = Label::create();
            titleLabel->setString(sTestCases[j - 1].title);
            titleLabel->setSystemFontSize(30);
            titleLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height - 20));
            layer->addChild(titleLabel);

            ui::Button *button = ui::Button::create();
            button->setTitleText("reset");
            button->setTitleFontSize(32);
            button->setScale9Enabled(true);
            button->addTouchEventListener([](Ref *, ui::Widget::TouchEventType type) {
                if (type == ui::Widget::TouchEventType::ENDED) {
                    CCLOG("ENDED");
                    Director::getInstance()->replaceScene(TestCaseScene::create());
                }
            });


            this->addChild(button, INT_MAX);
            button->setAnchorPoint(Vec2(1.0F, 1.0F));
            button->ignoreAnchorPointForPosition(false);
            button->setPosition(Vec2(winSize.width, winSize.height));
        });
    }
    innerNode->setContentSize(Size(winSize.width, height));
    if (height < winSize.height) {
        innerNode->setAnchorPoint(Vec2(0.5, 0.5));
        innerNode->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
        this->addChild(innerNode);
        _innerNode = innerNode;
    } else {
        Node *rootNode = Node::create();
        rootNode->setContentSize(winSize);
        this->addChild(rootNode);
        _innerNode = rootNode;

        ui::ScrollView *scrollView = ui::ScrollView::create();
        scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
        scrollView->setBounceEnabled(true);
        scrollView->setContentSize(winSize);
        scrollView->setInnerContainerSize(Size(winSize.width, height));

        scrollView->addChild(innerNode);
        rootNode->addChild(scrollView);
        scrollView->setAnchorPoint(Vec2(0.5, 0.5));
        scrollView->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));

        cw::ScrollBar *scrollBar = cw::ScrollBar::create();
        scrollBar->setDirection(cw::ScrollBar::Direction::VERTICAL);
        scrollBar->setHideThumbWhenScrollingStopped(false);
        scrollBar->setViewAndContentLength(scrollView->getContentSize().height, scrollView->getInnerContainerSize().height);
        scrollBar->setAnchorPoint(Vec2(0.5, 0.5));
        scrollBar->setPosition(Vec2(winSize.width - 10, winSize.height * 0.5));
        rootNode->addChild(scrollBar);
        scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionY());
        scrollBar->setColor(Color3B(255, 255, 255));
        
        scrollView->addEventListener([scrollBar, scrollView](Ref *sender, ui::ScrollView::EventType type) {
            switch (type) {
                case ui::ScrollView::EventType::SCROLLING:
                case ui::ScrollView::EventType::SCROLL_TO_TOP:
                case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
                case ui::ScrollView::EventType::BOUNCE_TOP:
                case ui::ScrollView::EventType::BOUNCE_BOTTOM:
                    scrollBar->refreshOffset(scrollView->getInnerContainer()->getPositionY());
                    break;
                default:
                    break;
            }
        });
    }
    return true;
}
