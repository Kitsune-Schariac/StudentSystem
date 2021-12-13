#include <stdio.h>
#include <stdlib.h>


//需要的头文件
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>


void Insert_City();
void Select_all();
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt;
SQLRETURN retcode;
int main()
{



	SQLCHAR szDSN[SQL_MAX_DSN_LENGTH + 1] = "kit";
	SQLCHAR szUID[MAXNAME] = "sa";
	SQLCHAR szAuthStr[MAXNAME] = "252625ILY";
	SQLRETURN retcode;


	//1.环境句柄
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);


	//2.连接句柄
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	retcode = SQLConnect(hdbc, szDSN, strlen("kit"), szUID, strlen("sa"), szAuthStr, strlen("252625ILY"));


	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		printf("连接失败!\n");
	}
	else
	{
		printf("连接成功!\n");
	}

	//释放数据源
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	
	return 0;
}
