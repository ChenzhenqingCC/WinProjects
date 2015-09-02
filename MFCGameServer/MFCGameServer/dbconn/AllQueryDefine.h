#ifndef __GAMEDBCONNECTION_ALLQUERYDEFINE_H
#define __GAMEDBCONNECTION_ALLQUERYDEFINE_H
#include <Zoic/include/Types.h>
#include "Zoic/include/QueryDefine.h"
#include "GameDBConnection.h"
#include "Zoic/include/Query.h"

QUERY_DEFINE(GameDBConnection, UserAccSelect, "select * from user;", "");
QUERY_DEFINE(GameDBConnection, PlayerBaseSelect, "select * from playerbase;", "");
QUERY_DEFINE(GameDBConnection, PlayerBaseInsert, "insert into playerbase(uid,level,worktype,nickname) values(puid,plevel,pworktype,pnickname);", "puid(INT UNSIGNED),plevel(INT UNSIGNED),pworktype(INT UNSIGNED),pnickname(VARCHAR 100)");
QUERY_DEFINE(GameDBConnection, UserAccInsert, "insert into user(name,password) values(accname,pass);", "accname(VARCHAR 100), pass(VARCHAR 100)");
#endif 
