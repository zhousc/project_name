#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("===============================================\n");
    return 0;
}

#define CHECK_RC(rc,szInfo,szErrMsg,db) if(rc!=SQLITE_OK) \
           {printf("%s error!\n",szInfo);\
           printf("%s\n",szErrMsg);    \
           sqlite3_free(szErrMsg);         \
           sqlite3_close(db);              \
           return 0;}

int main(int argc, char* argv[])
{

    sqlite3 *db;
    char *dbPath="test.db";
    char *szErrMsg = 0;
	char *szSql = NULL;
	int rc;

    rc= sqlite3_open(dbPath, &db);
    CHECK_RC(rc,"open database",szErrMsg,db);

	szSql="create table UserInfo(ID int primary key , UserName char, PassWord float);";
    rc=sqlite3_exec(db,szSql,0,0,&szErrMsg);
	CHECK_RC(rc,"create table",szErrMsg,db);
	rc=sqlite3_exec(db,"insert into UserInfo(ID,UserName,PassWord) values(1,'kfqcome','1.23456')",0,0,&szErrMsg);
    CHECK_RC(rc,"insert info",szErrMsg,db);

	rc=sqlite3_exec(db,"insert into UserInfo(ID,UserName,PassWord) values(2,'miss wang','6.54321')",0,0,&szErrMsg);
    CHECK_RC(rc,"insert info",szErrMsg,db);

	szSql="select * from UserInfo";
    rc = sqlite3_exec(db,szSql, callback, 0, &szErrMsg);
    CHECK_RC(rc,"query values",szErrMsg,db);

	sqlite3_close(db);
    return 0;
}
