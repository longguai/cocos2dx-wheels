#ifndef _CW_MACROS_H_
#define _CW_MACROS_H_

#include "platform/CCPlatformMacros.h"

// 这一组宏声明/定义的函数都是非虚函数

#define CW_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: varType get##funName(void) const;

#define CW_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: const varType& get##funName(void) const;

#define CW_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: varType get##funName(void);\
public: void set##funName(varType var);

#define CW_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: const varType& get##funName(void) const;\
public: void set##funName(const varType& var);

#define CW_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void) const { return varName; }

#define CW_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline const varType& get##funName(void) const { return varName; }

#define CW_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType get##funName(void) const { return varName; }\
public: inline void set##funName(varType var){ varName = var; }

#define CW_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline const varType& get##funName(void) const { return varName; }\
public: inline void set##funName(const varType& var){ varName = var; }

#define CW_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: inline varType get##funName(void) const { return varName; } \
public: inline void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        CC_SAFE_RETAIN(var); \
        CC_SAFE_RELEASE(varName); \
        varName = var; \
    } \
}

#endif
