// LuaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <stdarg.h>
#include "dirent.h"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <process.h>
#include <stdlib.h>
}

//打印错误
void error(lua_State *L, const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	char buffer[1024] = { 0 };
	vsprintf(buffer, fmt, argp);
	buffer[strlen(buffer)] = '\n';
	OutputDebugStringA(buffer);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void call_va(lua_State *L, const char *func, const char *sig, ...) {
	va_list vl;
	int narg, nres; /* number of arguments and results */
	va_start(vl, sig);
	lua_getglobal(L, func); /* get function */
	/* push arguments */
	narg = 0;
	while (*sig) { /* push arguments */
		switch (*sig++) {
			case 'd': /* double argument */
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'i': /* int argument */
				lua_pushnumber(L, va_arg(vl, int));
				break;
			case 's': /* string argument */
				lua_pushstring(L, va_arg(vl, char *));
				break;
			case '>':
				goto endwhile;
				break;
			default:
				error(L, "invalid option (%c)", *(sig - 1));
		}
		narg++;
		luaL_checkstack(L, 1, "too many arguments");
	} 
endwhile:
	/* do the call */
	nres = strlen(sig); /* number of expected results */
	if (lua_pcall(L, narg, nres, 0) != 0) /* do the call */
		error(L, "error running function '%s': %s",
		func, lua_tostring(L, -1));
	/* retrieve results */
	nres = -nres; /* stack index of first result */
	while (*sig) { /* get results */
		switch (*sig++) {
			case 'd': /* double result */
			{
				if (!lua_isnumber(L, nres))
					error(L, "wrong result type");
				*va_arg(vl, double *) = lua_tonumber(L, nres);
			}
			break;
			case 'i': /* int result */
			{
				if (!lua_isnumber(L, nres))
					error(L, "wrong result type");
				*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
			}
			break;
			case 's': /* string result */
			{
				if (!lua_isstring(L, nres))
					error(L, "wrong result type");
				*va_arg(vl, const char **) = lua_tostring(L, nres);
			}
			break;
			default:
				error(L, "invalid option (%c)", *(sig - 1));
		}
		nres++;
	}
	va_end(vl);
}



//打印栈
static void stackDump(lua_State *L) 
{
	int top = lua_gettop(L);
	for (size_t i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING: /* strings */
				printf("'%s'", lua_tostring(L, i));
			break;
			case LUA_TBOOLEAN: /* booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
			break;
			case LUA_TNUMBER: /* numbers */
				printf("%g", lua_tonumber(L, i));
			break;
			default: /* other values */
				printf("%s", lua_typename(L, t));
			break;
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

//加载lua
void load(char *filename, int *width, int *height) 
{
	lua_State *L = luaL_newstate();
	luaopen_base(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
	}
		
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2))	
	{
		error(L, "'width' should be a number\n");
	}
	if (!lua_isnumber(L, -1))
	{
		error(L, "'height' should be a number\n");
	}
	*width = (int)lua_tonumber(L, -2);
	*height = (int)lua_tonumber(L, -1);
	lua_close(L);
}

#define MAX_COLOR 255
/* assume that table is on the stack top */
int getfield(lua_State *L, const char *key)
{
	int result;
	lua_pushstring(L, key);
	lua_gettable(L, -2); /* get background[key] */
	if (!lua_isnumber(L, -1))
		error(L, "invalid component in background color");
	result = (int)lua_tonumber(L, -1) * MAX_COLOR;
	lua_pop(L, 1); /* remove number */
	return result;
}

/* call a function `f' defined in Lua */
//调用Lua写的函数
double f(lua_State *L, double x, double y)
{
	double z;
	/* push functions and arguments */
	lua_getglobal(L, "f"); /* function to be called */
	lua_pushnumber(L, x); /* push 1st argument */
	lua_pushnumber(L, y); /* push 2nd argument */
	/* do the call (2 arguments, 1 result) */
	if (lua_pcall(L, 2, 1, 0) != 0)
	{
		error(L, "error running function 'f': %s",
			lua_tostring(L, -1));
	}
		
	/* retrieve result */
	if (!lua_isnumber(L, -1))
		error(L, "function 'f' must return a number");
	z = lua_tonumber(L, -1);
	lua_pop(L, 1); /* pop returned value */
	return z;
}

double newf(lua_State *L, double x, double y)
{
	double z = 0;
	call_va(L, "f", "dd>d", x, y, &z);
	return z;
}

void test1()
{
	int width = 0;
	int height = 0;
	load("test.lua", &width, &height);
}

void test2()
{
	lua_State *L = luaL_newstate();
	lua_pushboolean(L, 0);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");

	lua_pushvalue(L, -5);
	//lua_pop(L, 3);
	//API用索引来访问栈中的元素。在栈中的第一个元素（也就是第一个被压入栈的）有索引1，下一个有索引2，以此类推。我们也可以用栈顶作为参照来存取元素，利用负索引。在这种情况下，-1指出栈顶元素（也就是最后被压入的），-2指出它的前一个元素，以此类推。

	stackDump(L);
}

void test3()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0))
	{
		error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
	}
	printf("%f", f(L, 2.0, 2.0));
	lua_close(L);
}

//lua调用C写的函数

typedef struct NumArray {
	int size;
	double values[1]; /* variable part */
} NumArray;

static int newarray(lua_State *L) {
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(NumArray) + (n - 1)*sizeof(double);
	NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);
	luaL_getmetatable(L, "LuaBook.array");
	lua_setmetatable(L, -2);
	a->size = n;
	return 1; /* new userdatum is already on the stack */
}

static NumArray *checkarray(lua_State *L) {
	void *ud = luaL_checkudata(L, 1, "LuaBook.array");
	luaL_argcheck(L, ud != NULL, 1, "'array' expected");
	return (NumArray *)ud;
}

static double *getelem(lua_State *L) {
	NumArray *a = checkarray(L);
	int index = luaL_checkint(L, 2);
	luaL_argcheck(L, 1 <= index && index <= a->size, 2,
		"index out of range");
	/* return element address */
	return &a->values[index - 1];
}

static int setarray(lua_State *L) {
	double newvalue = luaL_checknumber(L, 3);
	*getelem(L) = newvalue;
	return 0;
}

static int getarray(lua_State *L) {
	lua_pushnumber(L, *getelem(L));
	return 1;
}

static int getsize(lua_State *L) {
	NumArray *a = checkarray(L);
	lua_pushnumber(L, a->size);
	return 1;
}

//把函数注册为表下的
static const struct luaL_Reg arraylib[] = {
	{ "new", newarray },
	{ "set", setarray },
	{ "get", getarray },
	{ "size", getsize },
	{ NULL, NULL }
};


int array2string(lua_State *L) {
	NumArray *a = checkarray(L);
	lua_pushfstring(L, "array(%d)", a->size);
	return 1;
}

//以面向对象的形式注册
static const struct luaL_Reg arraylib_f[] = {
	{ "new", newarray },
	{ NULL, NULL }
};
static const struct luaL_Reg arraylib_m[] = {
	{ "__tostring", array2string },
	{ "set", setarray },
	{ "get", getarray },
	{ "size", getsize },
	{ NULL, NULL }
};

//使用原始方式注册函数
int luaopen_array(lua_State *L) 
{
	luaL_newmetatable(L, "LuaBook.array");
	luaL_openlib(L, "array", arraylib, 0);
	return 1;
}

//以面向对象方式注册函数
int luaopen_array_new(lua_State *L)
{
	luaL_newmetatable(L, "LuaBook.array");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2); /* pushes the metatable */
	lua_settable(L, -3); /* metatable.__index = metatable */
	luaL_openlib(L, NULL, arraylib_m, 0);
	luaL_openlib(L, "array", arraylib_f, 0);
	return 1;
}

//测试以不同方式注册函数
void test4()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_array(L);
	//luaopen_array_new(L);
	if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0))
	{
		error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
	}
	lua_close(L);
}

/* forward declaration for the iterator function */
static int dir_iter(lua_State *L);
static int l_dir(lua_State *L) {
	const char *path = luaL_checkstring(L, 1);
	/* create a userdatum to store a DIR address */
	DIR **d = (DIR **)lua_newuserdata(L, sizeof(DIR *));
	/* set its metatable */
	luaL_getmetatable(L, "LuaBook.dir");
	lua_setmetatable(L, -2);
	/* try to open the given directory */
	*d = opendir(path);
	if (*d == NULL) /* error opening the directory? */
		luaL_error(L, "cannot open %s: %s", path,
		strerror(errno));
	/* creates and returns the iterator function
	(its sole upvalue, the directory userdatum,
	is already on the stack top */
	lua_pushcclosure(L, dir_iter, 1);
	return 1;
}

static int dir_iter(lua_State *L) {
	DIR *d = *(DIR **)lua_touserdata(L, lua_upvalueindex(1));
	struct dirent *entry;
	if ((entry = readdir(d)) != NULL) {
		lua_pushstring(L, entry->d_name);
		return 1;
	}
	else return 0; /* no more values to return */
}

static int dir_gc(lua_State *L) {
	DIR *d = *(DIR **)lua_touserdata(L, 1);
	if (d) closedir(d);
	return 0;
}

int luaopen_dir(lua_State *L) {
	luaL_newmetatable(L, "LuaBook.dir");
	/* set its __gc field */
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, dir_gc);
	lua_settable(L, -3);
	/* register the `dir' function */
	lua_pushcfunction(L, l_dir);
	lua_setglobal(L, "dir");
	return 0;
}

//测试使用__gc回收lua无法管理的内存
void test5()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_dir(L);
	//luaopen_array_new(L);
	if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0))
	{
		error(L, "cannot run configuration file: %s", lua_tostring(L, -1));
	}
	lua_close(L);
}


int _tmain(int argc, _TCHAR* argv[])
{
	//test3();
	//test4();
	test5();
	getchar();
	return 0;
}

