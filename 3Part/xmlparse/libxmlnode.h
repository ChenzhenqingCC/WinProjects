#ifndef _LIBXMLNODE_H_
#define _LIBXMLNODE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "expat.h"
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "libexpat.lib")
#endif

#ifdef __STDC__

#include <stdarg.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap, f)
# define VA_END		va_end(ap)

#else /* __STDC__ */

# include <varargs.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap)
# define VA_END		va_end(ap)

#endif /* __STDC__ */

#ifndef HAVE_SNPRINTF
extern int ap_snprintf(char *, size_t, const char *, ...);
#define snprintf ap_snprintf
#endif

#ifndef HAVE_VSNPRINTF
extern int ap_vsnprintf(char *, size_t, const char *, va_list ap);
#define vsnprintf ap_vsnprintf
#endif

#define ZONE zonestr(__FILE__,__LINE__)
char *zonestr(char *file, int line);

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------- */
/*                                                           */
/* Pool-based memory management routines                     */
/*                                                           */
/* --------------------------------------------------------- */

#undef POOL_DEBUG
/*
flip these, this should be a prime number for top # of pools debugging
#define POOL_DEBUG 40009 
*/

/* pheap - singular allocation of memory */
struct pheap
{
	void *block;
	int size, used;
};

/* pool_cleaner - callback type which is associated
with a pool entry; invoked when the pool entry is 
free'd */
typedef void (*pool_cleaner)(void *arg);

/* pfree - a linked list node which stores an
allocation chunk, plus a callback */
struct pfree
{
	pool_cleaner f;
	void *arg;
	struct pheap *heap;
	struct pfree *next;
};

/* pool - base node for a pool. Maintains a linked list
of pool entries (pfree) */
typedef struct pool_struct
{
	int size;
	struct pfree *cleanup;
	struct pheap *heap;
#ifdef POOL_DEBUG
	char name[8], zone[32];
	int lsize;
} _pool, *pool;
#define pool_new() _pool_new(ZONE) 
#define pool_heap(i) _pool_new_heap(i,ZONE) 
#else
} _pool, *pool;
#define pool_heap(i) _pool_new_heap(i,NULL) 
#define pool_new() _pool_new(NULL)
#endif

pool _pool_new(char *zone); /* new pool :) */
pool _pool_new_heap(int size, char *zone); /* creates a new memory pool with an initial heap size */
void *pmalloc(pool p, int size); /* wrapper around malloc, takes from the pool, cleaned up automatically */
void *pmalloc_x(pool p, int size, char c); /* Wrapper around pmalloc which prefils buffer with c */
void *pmalloco(pool p, int size); /* YAPW for zeroing the block */
char *pstrdup(pool p, const char *src); /* wrapper around strdup, gains mem from pool */
void pool_stat(int full); /* print to stderr the changed pools and reset */
char *pstrdupx(pool p, const char *src); /* temp stub */
void pool_cleanup(pool p, pool_cleaner f, void *arg); /* calls f(arg) before the pool is freed during cleanup */
void pool_free(pool p); /* calls the cleanup functions, frees all the data on the pool, and deletes the pool itself */
int pool_size(pool p); /* returns total bytes allocated in this pool */


/* --------------------------------------------------------- */
/*                                                           */
/* Hashtable functions                                       */
/*                                                           */
/* --------------------------------------------------------- */
typedef struct xhn_struct
{
	struct xhn_struct *next;
	const char *key;
	void *val;
} *xhn, _xhn;

typedef struct xht_struct
{
	pool p;
	int prime;
	struct xhn_struct *zen;
} *xht, _xht;

xht xhash_new(int prime);
void xhash_put(xht h, const char *key, void *val);
void *xhash_get(xht h, const char *key);
void xhash_zap(xht h, const char *key);
void xhash_free(xht h);
typedef void (*xhash_walker)(xht h, const char *key, void *val, void *arg);
void xhash_walk(xht h, xhash_walker w, void *arg);


/* --------------------------------------------------------- */
/*                                                           */
/* String pools (spool) functions                            */
/*                                                           */
/* --------------------------------------------------------- */
struct spool_node
{
	char *c;
	struct spool_node *next;
};

typedef struct spool_struct
{
	pool p;
	int len;
	struct spool_node *last;
	struct spool_node *first;
} *spool;

spool spool_new(pool p); /* create a string pool */
void spooler(spool s, ...); /* append all the char * args to the pool, terminate args with s again */
char *spool_print(spool s); /* return a big string */
void spool_add(spool s, char *str); /* add a single char to the pool */
char *spools(pool p, ...); /* wrap all the spooler stuff in one function, the happy fun ball! */

/* --------------------------------------------------------- */
/*                                                           */
/* xmlnodes - Document Object Model                          */
/*                                                           */
/* --------------------------------------------------------- */
#define NTYPE_TAG    0
#define NTYPE_ATTRIB 1
#define NTYPE_CDATA  2

#define NTYPE_LAST   2
#define NTYPE_UNDEF  -1

/* -------------------------------------------------------------------------- 
Node structure. Do not use directly! Always use accessor macros 
and methods!
-------------------------------------------------------------------------- */
typedef struct xmlnode_t
{
	char*               name;
	unsigned short      type;
	char*               data;
	int                 data_sz;
	int                 complete;
	pool               p;
	struct xmlnode_t*  parent;
	struct xmlnode_t*  firstchild; 
	struct xmlnode_t*  lastchild;
	struct xmlnode_t*  prev; 
	struct xmlnode_t*  next;
	struct xmlnode_t*  firstattrib;
	struct xmlnode_t*  lastattrib;
} _xmlnode, *xmlnode;

/* Node creation routines */
xmlnode  xmlnode_wrap(xmlnode x,const char* wrapper);
xmlnode  xmlnode_new_tag(const char* name);
xmlnode  xmlnode_new_tag_ns(const char *name, const char *prefix, const char *ns_iri); // 添加前缀和后缀的
xmlnode  xmlnode_new_tag_pool(pool p, const char* name);
xmlnode  xmlnode_insert_tag(xmlnode parent, const char* name); 
xmlnode  xmlnode_insert_tag_ns(xmlnode parent, const char *name, const char *prefix, const char *ns_iri); // 添加前缀和后缀的
xmlnode  xmlnode_insert_cdata(xmlnode parent, const char* CDATA, unsigned int size);
xmlnode  xmlnode_insert_tag_node(xmlnode parent, xmlnode node);
void     xmlnode_insert_node(xmlnode parent, xmlnode node);
xmlnode  xmlnode_str(char *str, int len);
xmlnode  xmlnode_file(char *file);
char*    xmlnode_file_borked(char *file); /* same as _file but returns the parsing error */
xmlnode  xmlnode_dup(xmlnode x); /* duplicate x */
xmlnode  xmlnode_dup_pool(pool p, xmlnode x);

/* Node Memory Pool */
pool xmlnode_pool(xmlnode node);
xmlnode _xmlnode_new(pool p, const char *name, unsigned int type);

/* Node editing */
void xmlnode_hide(xmlnode child);
void xmlnode_hide_attrib(xmlnode parent, const char *name);

/* Node deletion routine, also frees the node pool! */
void xmlnode_free(xmlnode node);

/* Locates a child tag by name and returns it */
xmlnode  xmlnode_get_tag(xmlnode parent, const char* name);
char* xmlnode_get_tag_data(xmlnode parent, const char* name);

/* Attribute accessors */
void     xmlnode_put_attrib(xmlnode owner, const char* name, const char* value);
char*    xmlnode_get_attrib(xmlnode owner, const char* name);
void     xmlnode_put_expat_attribs(xmlnode owner, const char** atts);

/* Bastard am I, but these are fun for internal use ;-) */
void     xmlnode_put_vattrib(xmlnode owner, const char* name, void *value);
void*    xmlnode_get_vattrib(xmlnode owner, const char* name);

/* Node traversal routines */
xmlnode  xmlnode_get_firstattrib(xmlnode parent);
xmlnode  xmlnode_get_firstchild(xmlnode parent);
xmlnode  xmlnode_get_lastchild(xmlnode parent);
xmlnode  xmlnode_get_nextsibling(xmlnode sibling);
xmlnode  xmlnode_get_prevsibling(xmlnode sibling);
xmlnode  xmlnode_get_parent(xmlnode node);

/* Node information routines */
char*    xmlnode_get_name(xmlnode node);
char*    xmlnode_get_data(xmlnode node);
int      xmlnode_get_datasz(xmlnode node);
int      xmlnode_get_type(xmlnode node);

int      xmlnode_has_children(xmlnode node);
int      xmlnode_has_attribs(xmlnode node);

/* Node-to-string translation */
char*    xmlnode2str(xmlnode node);

/* Node-to-terminated-string translation 
-- useful for interfacing w/ scripting langs */
char*    xmlnode2tstr(xmlnode node);

int      xmlnode_cmp(xmlnode a, xmlnode b); /* compares a and b for equality */

int      xmlnode2file(char *file, xmlnode node); /* writes node to file */

/* Expat callbacks */
void expat_startElement(void* userdata, const char* name, const char** atts);
void expat_endElement(void* userdata, const char* name);
void expat_charData(void* userdata, const char* s, int len);


/* --------------------------------------------------------- */
/*                                                           */
/* String management routines                                */
/*                                                           */
/* --------------------------------------------------------- */
char *j_strdup(const char *str); /* provides NULL safe strdup wrapper */
char *j_strcat(char *dest, char *txt); /* strcpy() clone */
int j_strcmp(const char *a, const char *b); /* provides NULL safe strcmp wrapper */
int j_strcasecmp(const char *a, const char *b); /* provides NULL safe strcasecmp wrapper */
int j_strncmp(const char *a, const char *b, int i); /* provides NULL safe strncmp wrapper */
int j_strncasecmp(const char *a, const char *b, int i); /* provides NULL safe strncasecmp wrapper */
int j_strlen(const char *a); /* provides NULL safe strlen wrapper */
int j_atoi(const char *a, int def); /* checks for NULL and uses default instead, convienence */
void str_b64decode(char *str); /* what it says */


/* --------------------------------------------------------- */
/*                                                           */
/* XML escaping utils                                        */
/*                                                           */
/* --------------------------------------------------------- */
char *strescape(pool p, char *buf); /* Escape <>&'" chars */
char *strunescape(pool p, char *buf);

#ifdef __cplusplus
}
#endif

#endif