#include <stdio.h>
#include <stdlib.h>


//��Ҫ��ͷ�ļ�
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


	//1.�������
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);


	//2.���Ӿ��
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	retcode = SQLConnect(hdbc, szDSN, strlen("kit"), szUID, strlen("sa"), szAuthStr, strlen("252625ILY"));


	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		printf("����ʧ��!\n");
	}
	else
	{
		printf("���ӳɹ�!\n");
	}

	//�ͷ�����Դ
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	
	return 0;
}
