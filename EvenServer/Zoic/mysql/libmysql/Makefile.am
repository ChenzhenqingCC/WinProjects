SRC = \
	array.c \
	bchange.c \
	bcmp.c \
	bmove.c \
	bmove_upp.c \
	charset-def.c \
	charset.c \
	client.c \
	conf_to_src.c \
	ctype-big5.c \
	ctype-bin.c \
	ctype-cp932.c \
	ctype-czech.c \
	ctype-euc_kr.c \
	ctype-eucjpms.c \
	ctype-extra.c \
	ctype-gb2312.c \
	ctype-gbk.c \
	ctype-latin1.c \
	ctype-mb.c \
	ctype-simple.c \
	ctype-sjis.c \
	ctype-tis620.c \
	ctype-uca.c \
	ctype-ucs2.c \
	ctype-ujis.c \
	ctype-utf8.c \
	ctype-win1250ch.c \
	ctype.c \
	dbug.c \
	default.c \
	default_modify.c \
	errmsg.c \
	errors.c \
	get_password.c \
	hash.c \
	int2str.c \
	is_prefix.c \
	libmysql.c \
	list.c \
	llstr.c \
	longlong2str.c \
	manager.c \
	md5.c \
	mf_cache.c \
	mf_dirname.c \
	mf_fn_ext.c \
	mf_format.c \
	mf_iocache.c \
	mf_iocache2.c \
	mf_loadpath.c \
	mf_pack.c \
	mf_path.c \
	mf_tempfile.c \
	mf_unixpath.c \
	mf_wcomp.c \
	mulalloc.c \
	my_alloc.c \
	my_chsize.c \
	my_compress.c \
	my_create.c \
	my_delete.c \
	my_div.c \
	my_error.c \
	my_file.c \
	my_fopen.c \
	my_fstream.c \
	my_gethostbyname.c \
	my_getopt.c \
	my_getwd.c \
	my_init.c \
	my_lib.c \
	my_malloc.c \
	my_messnc.c \
	my_net.c \
	my_once.c \
	my_open.c \
	my_port.c \
	my_pread.c \
	my_pthread.c \
	my_read.c \
	my_realloc.c \
	my_rename.c \
	my_seek.c \
	my_sleep.c \
	my_static.c \
	my_strtoll10.c \
	my_symlink.c \
	my_thr_init.c \
	my_time.c \
	my_vsnprintf.c \
	my_write.c \
	net.c \
	pack.c \
	password.c \
	safemalloc.c \
	sha1.c \
	str2int.c \
	str_alloc.c \
	strcend.c \
	strcont.c \
	strend.c \
	strfill.c \
	string.c \
	strinstr.c \
	strmake.c \
	strmov.c \
	strnlen.c \
	strnmov.c \
	strtod.c \
	strtoll.c \
	strtoull.c \
	strxmov.c \
	strxnmov.c \
	thr_mutex.c \
	typelib.c \
	vio.c \
	viosocket.c \
	viossl.c \
	viosslfactories.c \
	xml.c \
	my_access.c

INCLUDES = -I$(SQLINCLUDE) -I../../zlib
AM_CFLAGS = -Wimplicit-function-declaration -DDBUG_OFF -DUNDEF_THREAD_HACK -DDEFAULT_CHARSET_HOME="\"/usr/local\""  -DSHAREDIR="\"/usr/local/share/mysql\""

LIB = $(MYSQLLIBDIR)/lib$(MYSQLLIBNAME).a


noinst_LIBRARIES = $(LIB)

__MYSQLLIBDIR__lib__MYSQLLIBNAME__a_SOURCES = $(SRC)

