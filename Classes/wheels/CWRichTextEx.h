#ifndef __CWRICHTEXTEX_H__
#define __CWRICHTEXTEX_H__

#include "ui/UIRichText.h"

namespace cocos2d {
    class Label;
}

namespace cw {

class RichElementEx : public cocos2d::Ref
{
public:
    enum class Type
    {
        TEXT,
        IMAGE,
        CUSTOM,
        NEWLINE
    };
    typedef std::function<void (cocos2d::Ref*, cocos2d::Ref*)> ccRichElementExClickCallback;
    RichElementEx(){};
    virtual ~RichElementEx(){};
    bool init(int tag, const cocos2d::Color3B& color, GLubyte opacity);
    Type getType() const { return _type; }
    int getTag() const { return _tag; }
    const cocos2d::Color3B &getColor() const { return _color; }
    GLubyte getOpacity() const { return _opacity; }
    void setClickCallback(const ccRichElementExClickCallback &clickCallback) { _clickCallback = clickCallback; }
    const ccRichElementExClickCallback &getClickCallback() const { return _clickCallback; }

protected:
    Type _type;
    int _tag;
    cocos2d::Color3B _color;
    GLubyte _opacity;
    ccRichElementExClickCallback _clickCallback;
    friend class RichTextEx;
};

class RichElementTextEx : public RichElementEx
{
public:
    RichElementTextEx(){_type = Type::TEXT; _underlineEnable = false; _outlineEnable = false;};
    virtual ~RichElementTextEx(){};
    bool init(int tag, const cocos2d::Color3B& color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize);
    static RichElementTextEx* create(int tag, const cocos2d::Color3B& color, GLubyte opacity, const std::string& text, const std::string& fontName, float fontSize);
    const std::string &getText() const { return _text; }
    const std::string &getFontName() const { return _fontName; }
    float getFontSize() const { return _fontSize; }
    void enableUnderline() { _underlineEnable = true; }
    void disableUnderline() { _underlineEnable = false; }
    void enableOutline(const cocos2d::Color3B &outlineColor, float outlineSize){ _outlineEnable = true; _outlineColor = outlineColor; _outlineSize = outlineSize; }
    void disableOutline() { _outlineEnable = false; }

protected:
    std::string _text;
    std::string _fontName;
    float _fontSize;
    bool _underlineEnable;
    bool _outlineEnable;
    cocos2d::Color3B _outlineColor;
    float _outlineSize;
    friend class RichTextEx;

};

class RichElementImageEx : public RichElementEx
{
public:
    enum class TextureType
    {
        LOCAL,
        PLIST
    };
    RichElementImageEx(){_type = Type::IMAGE; _textureType = TextureType::LOCAL; _scaleX = _scaleY = 1.0f;};
    virtual ~RichElementImageEx(){};
    bool init(int tag, const cocos2d::Color3B& color, GLubyte opacity, const std::string& filePath, TextureType textureType);
    static RichElementImageEx* create(int tag, const cocos2d::Color3B& color, GLubyte opacity, const std::string& filePath, TextureType textureType);
    const std::string &getFilePath() const { return _filePath; }
    TextureType getTextureType() const { return _textureType; }
    void setScale(float scale){_scaleX = _scaleY = scale;}
    void setScale(float scaleX, float scaleY){_scaleX = scaleX; _scaleY = scaleY;}
    void setScaleX(float scaleX){_scaleX = scaleX;}
    void setScaleY(float scaleY){_scaleY = scaleY;}

protected:
    std::string _filePath;
    cocos2d::Rect _textureRect;
    TextureType _textureType;
    float _scaleX;
    float _scaleY;
    friend class RichTextEx;
};

class RichElementCustomNodeEx : public RichElementEx
{
public:
    RichElementCustomNodeEx(){_type = Type::CUSTOM;};
    virtual ~RichElementCustomNodeEx(){CC_SAFE_RELEASE(_customNode);};
    bool init(int tag, const cocos2d::Color3B& color, GLubyte opacity, cocos2d::Node* customNode);
    static RichElementCustomNodeEx* create(int tag, const cocos2d::Color3B& color, GLubyte opacity, cocos2d::Node* customNode);
protected:
    cocos2d::Node* _customNode;
    friend class RichTextEx;
};

class RichElementNewLineEx : public RichElementEx
{
public:
    RichElementNewLineEx(){_type = Type::NEWLINE;};
    virtual ~RichElementNewLineEx(){};
    bool init(int tag);
    static RichElementNewLineEx* create(int tag);
protected:
    friend class RichTextEx;
};

class RichTextEx : public cocos2d::ui::Widget
{
public:
    RichTextEx();
    virtual ~RichTextEx();
    static RichTextEx* create();
    void insertElement(RichElementEx* element, int index);
    void pushBackElement(RichElementEx* element);
    void removeElement(int index);
    void removeElement(RichElementEx* element);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    void setVerticalSpace(float space);
    float getVerticalSpace() const;
    void setEmptyLineHeight(float height);
    float getEmptyLineHeight() const;
    void setHorizontalAlignment(cocos2d::TextHAlignment alignment);
    cocos2d::TextHAlignment getHorizontalAlignment() const;
    virtual void setAnchorPoint(const cocos2d::Vec2 &pt);
    virtual cocos2d::Size getVirtualRendererSize() const override;
    void formatText();
    virtual void ignoreContentAdaptWithSize(bool ignore);
    virtual std::string getDescription() const override;

CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;

protected:
    virtual void initRenderer();
    void pushToContainer(cocos2d::Node* renderer);
    void handleTextRenderer(RichElementTextEx* elmtText, const std::string& text, const std::string& fontName, float fontSize, const cocos2d::Color3B& color, GLubyte opacity);
    void handleImageRenderer(RichElementImageEx* elmtImage, const std::string& fileParh, const cocos2d::Color3B& color, GLubyte opacity);
    void handleCustomRenderer(RichElementEx* element, cocos2d::Node* renderer);
    void formatRenderers();
    void addNewLine();
    virtual void releaseUpEvent() override;
    virtual void cancelUpEvent() override;

protected:
    bool _formatTextDirty;
    cocos2d::Vector<RichElementEx*> _richElements;
    std::vector<cocos2d::Vector<cocos2d::Node*>*> _elementRenders;
    float _leftSpaceWidth;
    float _verticalSpace;
    float _emptyLineHeight;
    cocos2d::TextHAlignment _hAlignment;
    cocos2d::Node* _elementRenderersContainer;
};

}

#endif
