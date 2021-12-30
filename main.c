#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

SQLHENV henv; //环境语柄
SQLHDBC hdbc; //连接语柄
SQLHSTMT hstmt; //语句语柄
SQLRETURN retcode; //返回值

//主菜单
int startface()
{
	int a;
	/*printf("1.录入学生数据\n2.查询学生数据\n");
	printf("3.打印所有数据\n4.删除学生数据\n");
	printf("5.修改学生数据\n6.查看统计信息\n");
	printf("9.开发者模式(操作数据库)");
	printf("0.退出\n");*/
	printf("\n=================高校学生成绩管理系统=================\n");
	printf("------------------------------------------------------\n");
	printf("1.学生成绩录入\t\t\t2.学生成绩查询\n");
	printf("3.打印所有数据\t\t\t4.学生资料删除\n");
	printf("5.学生资料修改\t\t\t6.学生成绩统计\n");
	printf("\t\t0.系统退出\n");
	scanf_s("%d", &a);
	return a;
}

//学生信息录入模块
int addstu()
{
	int flag, t;
	char ch;
	
	printf("请输入要录入的学生数量\n");
	scanf_s("%d", &t);
	SQLCHAR s1[100] = "insert into stu values(?,?,?,?)";
	SQLCHAR str1[100];
	SQLCHAR str2[100];
	SQLCHAR str3[100];
	SQLCHAR	str4[100];
	SQLINTEGER* p = SQL_NTS;

	while (t)
	{
		//strcpy_s(s1, sizeof(s1), "insert into stu values(?,?,?,?)");
		/*printf("%s\n", s1);*/

		SQLPrepare(hstmt, s1, strlen(s1));	//预编译语句命令			第三个参数与数组大小相同
		printf("请输入学号： ");
		ch = getchar();
		gets_s(str1, 10);
		printf("请输入姓名：");
		//fflush(stdin);
		//ch = getchar();
		gets_s(str2,10);
		printf("请选择性别（1.男 2.女)");
		do {
			scanf_s("%d", &flag);
			if (flag == 1)
				strcpy_s(str3, sizeof(str2), "男");
			else if (flag == 2)
				strcpy_s(str3, sizeof(str2), "女");
			else
				printf("请重新选择\n");
		} while (!(flag == 1 || flag == 2));
		str3[2] = '\0';
		//printf("%d", strlen(str2));
		printf("请输入成绩\n");
		ch = getchar();
		gets_s(str4, 4);

		/*puts(str1);
		puts(str2);
		puts(str3);
		puts(str4);*/
		

		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str1), 0, str1, 0, NULL);
		//第一个参数是语句语柄，第二个参数指定送参数的编号，这里是1，那么就是第一个问号，第三个参数用来指定送参数的类型，这里用的SQL_PARAM_INPUT就是指定为输入类型
		//第四个参数指定了Ｃ类型，第五个参数指定了SQL类型，第六个参数制定了送参数大小，第七个参数为送参数标记的列或表达式的十进制数字
		//第八个参数指定了指向送参数地址的指针(也可以直接写数据)，第九个参数指定了缓冲区的长度，最后一个参数指定了指向参数长度缓冲区的指针
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str2), 0, str2, 0, NULL);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str3), 0, str3, 0, NULL);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str4), 0, str4, 0, NULL);
		//绑定参数值完成
		retcode = SQLExecute(hstmt);		//执行语句
		SQLCancel(hstmt);		//让语句句柄回到上一个状态
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			printf("操作成功\n");
			t--;
		}
		else
		{
			SQLCHAR errmsg[100],errmsg2[100];
			SQLError(henv, hdbc, hstmt, errmsg2, NULL, errmsg, sizeof(errmsg), NULL);
			printf("%s\n", errmsg);
			printf("是否重新输入？(0.Yes  Other.No)");
			int qtq = 1;
			scanf_s("%d", &qtq);
			if (qtq != 0)
				t--;
		}
		
		
	}
	printf("操作完成\n");
	return 0;
}

//打印所有信息模块
int qc()
{
	SQLCHAR s1[100] = "use test01";
	//SQLCHAR s2[300] = "select * from stu";
	SQLCHAR s2[100] = "select * ";
	SQLCHAR s3[100] = "from stu";
	
	strncat_s(s2, sizeof(s2), s3, strlen(s3));
	//strncat_s(s2, sizeof(s2), s3, _TRUNCATE);
	/*starcat和strncat的使用会导致栈溢出，vs2019返回的报错信息是s2空间只有100，而写入s2需要的空间可能会有200，故不可用，这里使用strncat_s
	_TRUNCATE是一个宏定义，一个很大的数字，在不知道需要复制多少数据的时候可以使用它，会尽量的复制更多的数据*/
	
	//strcpy(s3, "from stu");
	//strcat((char*)s2, (char*)s3);
	
	retcode = SQLExecDirect(hstmt, s1, 100);//第三个参数可以写为第二个参数所提取的长度,此处s2占用空间为100，即可以写为100
	retcode = SQLExecDirect(hstmt, s2, SQL_NTS);//常数SQL_NTS表示前面的参数是一个以null结尾的字符串
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		SQLCHAR str1[50], str2[50], str3[50], str4[50];
		SQLLEN *len_str1, *len_str2, *len_str3, *len_str4;
		//这里使用指针是为了在退出函数的时候不发生内存溢出
		while (SQLFetch(hstmt) != SQL_NO_DATA)
		{
			SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 50, &len_str1);
			SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 50, &len_str2);
			SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 50, &len_str3);
			SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 50, &len_str4);
			printf("\n%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
		}
	}
	
	//SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLCancel(hstmt);
	return 0;
}
	
//查询学生信息
int sel()
{
	SQLCHAR s1[100] = "select * from stu where ";
	SQLCHAR s2[] = "姓名 = '";
	SQLCHAR s3[] = "学号 = '";
	SQLCHAR s4[] = "'";
	SQLCHAR str1[40];
	SQLCHAR str2[40];
	char ch;
	int flag = 0;

	printf("请选择通过什么数据查询\n1.姓名  2.学号：");
	while (!(flag <=2 && flag >=1))
	{
		scanf_s("%d", &flag);
		if (flag == 1)
		{
			/*SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen("姓名"), 0, "姓名", 0, NULL);*/
			strcpy_s(str1, sizeof(str1), "姓名");
			strncat_s(s1, sizeof(s1), s2,strlen(s2));
		}
		else if (flag == 2)
		{
			/*SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen("学号"), 0, "学号", 0, NULL);*/
			strcpy_s(str1, sizeof(str1), "学号");
			strncat_s(s1, sizeof(s1), s3,strlen(s3));
		}
		else
			printf("请重新输入\n");
	}
	printf("请输入要查询的%s的值：  ", str1);
	ch = getchar();
	
	gets_s(str2,10);
	strncat_s(s1, sizeof(s1), str2, strlen(str2));
	strncat_s(s1, sizeof(s1), s4, strlen(s4));
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str2), 0, str2, 0, NULL);
	retcode = SQLExecDirect(hstmt,s1, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		SQLCHAR str1[50], str2[50], str3[50], str4[50];
		SQLLEN* len_str1, * len_str2, * len_str3, * len_str4;
		//这里使用指针是为了在退出函数的时候不发生内存溢出
		while (SQLFetch(hstmt) != SQL_NO_DATA)
		{
			SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 50, &len_str1);
			SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 50, &len_str2);
			SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 50, &len_str3);
			SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 50, &len_str4);
			printf("\n%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
		}
	}
	SQLCancel(hstmt);
	return 0;
}

//删除学生信息
int del()
{
	SQLCHAR s1[100] = "delete from stu where 学号 = '";
	SQLCHAR s2[100];
	SQLCHAR s3[100] = "'";
	SQLCHAR errmsg[100];
	char ch;
	printf("请输入要删除的记录的学号： ");
	ch = getchar();
	gets_s(s2, sizeof(s2));
	strncat_s(s1, sizeof(s1), s2, strlen(s2));
	strncat_s(s1, sizeof(s1), s3, strlen(s3));		//用字符串拼接完成SQL语句操作
	retcode = SQLExecDirect(hstmt, s1, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		printf("操作成功\n");
	}
	else
	{
		SQLError(henv, hdbc, hstmt, NULL, NULL, errmsg, sizeof(errmsg), NULL);
		printf("操作失败，请检查数据是否存在\n%s\n", errmsg);
	}
	SQLCancel(hstmt);
	return 0;
}

//修改学生信息
int upd()
{
	SQLCHAR up[100] = "update stu set ?=? where 学号=?";
	//SQLCHAR ss[100] = "update stu set 成绩=3 where 学号=2";
	SQLCHAR v1[100] = "update stu set ";
	SQLCHAR v2[100] = "=";
	SQLCHAR v3[100] = " where 学号=";
	SQLCHAR s1[100];
	SQLCHAR s2[100];
	SQLCHAR s3[100];
	char ch;
	/*printf("%s\n", up);*/
	//SQLPrepare(hstmt, up, 100);
	printf("请输入你想要修改的学生的学号\n");
	ch = getchar();
	gets_s(s3, sizeof(s3));
	printf("请选择要修改的属性\n");
	gets_s(s1, 5);
	printf("请输入修改后的值\n");
	gets_s(s2, sizeof(s2));
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_CHAR, SQL_C_CHAR, strlen(s1), 0, s1, 100, NULL);
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_CHAR, SQL_C_CHAR, strlen(s2), 0, s2, 100, NULL);
	//SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_CHAR, SQL_C_CHAR, strlen(s3), 0, s3, 100, NULL);
	//retcode = SQLExecute(hstmt);
	////retcode = SQLExecDirect(hstmt, ss, SQL_NTS);
	strncat_s(v1, sizeof(v1), s1, strlen(s1));
	strncat_s(v1, sizeof(v1), v2, strlen(v2));
	strncat_s(v1, sizeof(v1), s2, strlen(s2));
	strncat_s(v1, sizeof(v1), v3, strlen(v3));
	strncat_s(v1, sizeof(v1), s3, strlen(s3));
	/*printf("%s\n", v1);*/
	SQLExecDirect(hstmt, v1, SQL_NTS);
	SQLCancel(hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		printf("操作成功\n");
	}
	else {
		UCHAR errmsg[100],errmsg2[100];
		SQLError(henv, hdbc, hstmt,errmsg2, NULL, errmsg, strlen(errmsg), NULL);
		printf("%s\n", errmsg);
	}
}

//统计学生信息
int coun()
{
	SQLCHAR s1[100] = "select row_number() over(order by 成绩 desc),学号,姓名,成绩 from stu";
	SQLCHAR s2[100] = "select top 1 姓名,成绩 from stu order by 成绩 desc";
	SQLCHAR s3[100] = "select top 1 姓名,成绩 from stu order by 成绩";
	SQLCHAR s4[100] = "select avg(成绩) from stu";
	float va = 0;
	int a;
	printf("请选择要查看的信息\n1.查看所有学生排名  2.查看分数极值、平均值： ");
	scanf_s("%d", &a);
	if (a == 1)
	{
		retcode = SQLExecDirect(hstmt, s1, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR str1[50], str2[50], str3[50], str4[50];
			SQLLEN* len_s1, * len_s2, * len_s3, * len_s4;
			printf("排名\t学号\t姓名\t成绩\n");
			while (SQLFetch(hstmt) != SQL_NO_DATA)
			{
				SQLGetData(hstmt, 1, SQL_CHAR, str1, 50, &len_s1);
				SQLGetData(hstmt, 2, SQL_CHAR, str2, 50, &len_s2);
				SQLGetData(hstmt, 3, SQL_CHAR, str3, 50, &len_s3);
				SQLGetData(hstmt, 4, SQL_CHAR, str4, 50, &len_s4);
				printf("%s\t%s\t%s\t%s\n",str1,str2,str3,str4);
			}
		}
		SQLCancel(hstmt);
	}
	else if (a == 2)
	{
		retcode = SQLExecDirect(hstmt, s2, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR str1[50], str2[50];
			SQLLEN* len_s1, * len_s2;
			printf("最高分获得者:\n");
			while (SQLFetch(hstmt) != SQL_NO_DATA)
			{
				SQLGetData(hstmt, 1, SQL_CHAR, str1, 50, &len_s1);
				SQLGetData(hstmt, 2, SQL_CHAR, str2, 50, &len_s2);
				printf("%s\t%s\n", str1, str2);
			}
		}
		SQLCancel(hstmt);
		retcode = SQLExecDirect(hstmt, s3, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR str1[50], str2[50];
			SQLLEN* len_s1, * len_s2;
			printf("最低分者:\n");
			while (SQLFetch(hstmt) != SQL_NO_DATA)
			{
				SQLGetData(hstmt, 1, SQL_CHAR, str1, 50, &len_s1);
				SQLGetData(hstmt, 2, SQL_CHAR, str2, 50, &len_s2);
				printf("%s\t%s\n", str1, str2);
			}
		}
		SQLCancel(hstmt);
		retcode = SQLExecDirect(hstmt, s4, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR str1[50];
			SQLLEN* len_s;
			printf("平均分: ");
			SQLFetch(hstmt);
				SQLGetData(hstmt, 1, SQL_CHAR, str1, 50, &len_s);
				printf("%s\n", str1);
			
		}
		SQLCancel(hstmt);
	}
	return 0;
}

int main()
{
	int flag = 1;
	SQLCHAR szDSN[SQL_MAX_DSN_LENGTH + 1] = "kit";
	SQLCHAR szUID[MAXNAME] = "sa";
	SQLCHAR szAuthStr[MAXNAME] = "252625ILY";

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv); //申请环境语柄
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); //申请数据库连接语柄
	retcode = SQLConnect(hdbc, szDSN, strlen("bit"), szUID, strlen("kit"), szAuthStr, strlen("252625ILY"));
	//"kit为数据源名称

	if ((retcode == SQL_SUCCESS) || (retcode == SQL_SUCCESS_WITH_INFO))
	{
		printf("连接成功\n");
	}
	else
	{
		printf("数据库连接失败\n");
		return -1;
	}

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	while (flag)
	{
		
		switch (startface())
		{
		case 1: addstu(); break;
		case 2: sel(); break;
		case 3: qc(); break;
		case 4: del(); break;
		case 5: upd(); break;
		case 6: coun(); break;
		case 0: flag = 0; break;
		default: printf("请重新选择\n");
		}
	}
	printf("完成\n");
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);	//释放语句语柄
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);	//释放环境语柄
	system("pause");
}