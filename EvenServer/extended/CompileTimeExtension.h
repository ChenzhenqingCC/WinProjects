#ifndef __CompileTimeExtension_H
#define __CompileTimeExtension_H


template <bool> struct CompileTimeAssert
{
	CompileTimeAssert(...) {};
};

template <> struct CompileTimeAssert<false> { };

#define COMPILETIME_ASSERT(expr, msg)	\
{ \
	class _COMPLIE_ASSERT_FAIL__##msg {} _msg; \
	sizeof((CompileTimeAssert<expr>)(_msg)); \
}

template <typename name, int size> struct CompileSizeofOutput;

#define COMPILETIME_SIZEOF_OUTPUT(expr)	\
{ \
	CompileSizeofOutput<##expr, sizeof(expr)>(); \
}

template <int result> struct CompileCaclInt;

#define COMPILETIME_CACLINT(expr)	\
{ \
	CompileCaclInt<(expr)>(); \
}


#endif // __CompileTimeExtension_H

