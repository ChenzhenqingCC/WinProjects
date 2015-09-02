#ifndef __SERVER_MACROS_H__
#define __SERVER_MACROS_H__

/** CC_PROPERTY_READONLY is used to declare a protected variable.
We can use getter to read the variable.
@param varType     the type of variable.
@param varName     variable name.
@param funName     "get + funName" will be the name of the getter.
@warning   The getter is a public virtual function, you should rewrite it first.
The variables and methods declared after CC_PROPERTY_READONLY are all public.
If you need protected or private, please declare.
*/
#define CC_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const;

#define CC_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void) const;

/** CC_PROPERTY is used to declare a protected variable.
We can use getter to read the variable, and use the setter to change the variable.
@param varType     the type of variable.
@param varName     variable name.
@param funName     "get + funName" will be the name of the getter.
"set + funName" will be the name of the setter.
@warning   The getter and setter are public virtual functions, you should rewrite them first.
The variables and methods declared after CC_PROPERTY are all public.
If you need protected or private, please declare.
*/
#define CC_PROPERTY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void); \
public: virtual void set##funName(varType var);

#define CC_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void) const; \
public: virtual void set##funName(const varType& var);

/** CC_SYNTHESIZE_READONLY is used to declare a protected variable.
We can use getter to read the variable.
@param varType     the type of variable.
@param varName     variable name.
@param funName     "get + funName" will be the name of the getter.
@warning   The getter is a public inline function.
The variables and methods declared after CC_SYNTHESIZE_READONLY are all public.
If you need protected or private, please declare.
*/
#define CC_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; }

/** CC_SYNTHESIZE is used to declare a protected variable.
We can use getter to read the variable, and use the setter to change the variable.
定义一个保护型的变量，并声明一个getfunName函数和setfunName函数，你可以用getfunName函数得到变量的值，用setfunName函数设置变量得值。
@param varType     the type of variable.
@param varName     variable name.
@param funName     "get + funName" will be the name of the getter.
"set + funName" will be the name of the setter.
@warning   The getter and setter are public inline functions.
The variables and methods declared after CC_SYNTHESIZE are all public.
If you need protected or private, please declare.
*/
#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
if (varName != var) \
	{ \
	CC_SAFE_RETAIN(var); \
	CC_SAFE_RELEASE(varName); \
	varName = var; \
	} \
}

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define CC_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define CC_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define CC_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define CC_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define CC_BREAK_IF(cond)           if(cond) break

#endif