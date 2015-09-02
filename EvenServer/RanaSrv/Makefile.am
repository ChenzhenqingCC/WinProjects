SRC = \
	RanaService.cpp \
	RanaClient.cpp \
	RanaServicesStrategy.cpp \
	RanaScriptManager.cpp \
	command/Cmd_Rana.cpp

BUILT_SOURCES = \
	../protocol/Rana/RanaProto_Rana.h

../protocol/Rana/RanaProto_Rana.h: ./protocol/RanaProto_Rana.h
	cp ./protocol/RanaProto_Rana.h ../protocol/Rana/RanaProto_Rana.h

INCLUDES = -I$(LUAINCLUDE) -I$(PROTOTYPESINCLUDE) -I$(PROTOINCLUDE)/Rana

LIB = $(RANASRVLIBDIR)/lib$(RANASRVLIBNAME).a

noinst_LIBRARIES = $(LIB)

__RANASRVLIBDIR__lib__RANASRVLIBNAME__a_SOURCES = $(SRC)
