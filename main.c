#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

SQLHENV henv; //�������
SQLHDBC hdbc; //�������
SQLHSTMT hstmt; //������
SQLRETURN retcode; //����ֵ

//���˵�
int startface()
{
	int a;
	printf("1.¼��ѧ������\n2.��ѯѧ������\n");
	printf("3.��ӡ��������\n4.ɾ��ѧ������\n");
	printf("5.�޸�ѧ������\n6.�鿴ͳ����Ϣ\n");
	printf("9.������ģʽ(�������ݿ�)");
	printf("0.�˳�\n");
	scanf_s("%d", &a);
	return a;
}

//ѧ����Ϣ¼��ģ��
int addstu()
{
	int flag, t;
	char ch;
	
	printf("������Ҫ¼���ѧ������\n");
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
		printf("%s\n", s1);

		SQLPrepare(hstmt, s1, strlen(s1));	//Ԥ�����������			�����������������С��ͬ
		printf("������ѧ�ţ� ");
		ch = getchar();
		gets_s(str4, 10);
		printf("������������");
		//fflush(stdin);
		//ch = getchar();
		gets_s(str1,10);
		printf("��ѡ���Ա�1.�� 2.Ů)");
		do {
			scanf_s("%d", &flag);
			if (flag == 1)
				strcpy_s(str2, sizeof(str2), "��");
			else if (flag == 2)
				strcpy_s(str2, sizeof(str2), "Ů");
			else
				printf("������ѡ��\n");
		} while (!(flag == 1 || flag == 2));
		str2[2] = '\0';
		//printf("%d", strlen(str2));
		printf("������ɼ�\n");
		ch = getchar();
		gets_s(str3, 4);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str1), 0, str1, 0, NULL);
		//��һ�����������������ڶ�������ָ���Ͳ����ı�ţ�������1����ô���ǵ�һ���ʺţ���������������ָ���Ͳ��������ͣ������õ�SQL_PARAM_INPUT����ָ��Ϊ��������
		//���ĸ�����ָ���ˣ����ͣ����������ָ����SQL���ͣ������������ƶ����Ͳ�����С�����߸�����Ϊ�Ͳ�����ǵ��л���ʽ��ʮ��������
		//�ڰ˸�����ָ����ָ���Ͳ�����ַ��ָ��(Ҳ����ֱ��д����)���ھŸ�����ָ���˻������ĳ��ȣ����һ������ָ����ָ��������Ȼ�������ָ��
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str2), 0, str2, 0, NULL);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str3), 0, str3, 0, NULL);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(str4), 0, str4, 0, NULL);
		//�󶨲���ֵ���
		retcode = SQLExecute(hstmt);		//ִ�����
		SQLCancel(hstmt);		//��������ص���һ��״̬
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			printf("�����ɹ�\n");
			t--;
		}
		else
		{
			SQLCHAR errmsg[100],errmsg2[100];
			SQLError(henv, hdbc, hstmt, errmsg2, NULL, errmsg, sizeof(errmsg), NULL);
			printf("%s\n", errmsg);
			printf("�Ƿ��������룿(0.Yes  Other.No)");
			int qtq = 1;
			scanf_s("%d", &qtq);
			if (qtq != 0)
				t--;
		}
		
		
	}
	printf("�������\n");
	return 0;
}

//��ӡ������Ϣģ��
int qc()
{
	SQLCHAR s1[100] = "use test01";
	//SQLCHAR s2[300] = "select * from stu";
	SQLCHAR s2[100] = "select * ";
	SQLCHAR s3[100] = "from stu";
	
	strncat_s(s2, sizeof(s2), s3, strlen(s3));
	//strncat_s(s2, sizeof(s2), s3, _TRUNCATE);
	/*starcat��strncat��ʹ�ûᵼ��ջ�����vs2019���صı�����Ϣ��s2�ռ�ֻ��100����д��s2��Ҫ�Ŀռ���ܻ���200���ʲ����ã�����ʹ��strncat_s
	_TRUNCATE��һ���궨�壬һ���ܴ�����֣��ڲ�֪����Ҫ���ƶ������ݵ�ʱ�����ʹ�������ᾡ���ĸ��Ƹ��������*/
	
	//strcpy(s3, "from stu");
	//strcat((char*)s2, (char*)s3);
	
	retcode = SQLExecDirect(hstmt, s1, 100);//��������������дΪ�ڶ�����������ȡ�ĳ���,�˴�s2ռ�ÿռ�Ϊ100��������дΪ100
	retcode = SQLExecDirect(hstmt, s2, SQL_NTS);//����SQL_NTS��ʾǰ��Ĳ�����һ����null��β���ַ���
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		SQLCHAR str1[50], str2[50], str3[50], str4[50];
		SQLLEN *len_str1, *len_str2, *len_str3, *len_str4;
		//����ʹ��ָ����Ϊ�����˳�������ʱ�򲻷����ڴ����
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
	
//��ѯѧ����Ϣ
int sel()
{
	SQLCHAR s1[100] = "select * from stu where ";
	SQLCHAR s2[] = "���� = '";
	SQLCHAR s3[] = "ѧ�� = '";
	SQLCHAR s4[] = "'";
	SQLCHAR str1[40];
	SQLCHAR str2[40];
	char ch;
	int flag = 0;

	printf("��ѡ��ͨ��ʲô���ݲ�ѯ\n1.����  2.ѧ�ţ�");
	while (!(flag <=2 && flag >=1))
	{
		scanf_s("%d", &flag);
		if (flag == 1)
		{
			/*SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen("����"), 0, "����", 0, NULL);*/
			strcpy_s(str1, sizeof(str1), "����");
			strncat_s(s1, sizeof(s1), s2,strlen(s2));
		}
		else if (flag == 2)
		{
			/*SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen("ѧ��"), 0, "ѧ��", 0, NULL);*/
			strcpy_s(str1, sizeof(str1), "ѧ��");
			strncat_s(s1, sizeof(s1), s3,strlen(s3));
		}
		else
			printf("����������\n");
	}
	printf("������Ҫ��ѯ��%s��ֵ��  ", str1);
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
		//����ʹ��ָ����Ϊ�����˳�������ʱ�򲻷����ڴ����
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

//ɾ��ѧ����Ϣ
int del()
{
	SQLCHAR s1[100] = "delete from stu where ѧ�� = '";
	SQLCHAR s2[100];
	SQLCHAR s3[100] = "'";
	SQLCHAR errmsg[100];
	char ch;
	printf("������Ҫɾ���ļ�¼��ѧ�ţ� ");
	ch = getchar();
	gets_s(s2, sizeof(s2));
	strncat_s(s1, sizeof(s1), s2, strlen(s2));
	strncat_s(s1, sizeof(s1), s3, strlen(s3));		//���ַ���ƴ�����SQL������
	retcode = SQLExecDirect(hstmt, s1, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		printf("�����ɹ�\n");
	}
	else
	{
		SQLError(henv, hdbc, hstmt, NULL, NULL, errmsg, sizeof(errmsg), NULL);
		printf("����ʧ�ܣ����������Ƿ����\n%s\n", errmsg);
	}
	SQLCancel(hstmt);
	return 0;
}

//�޸�ѧ����Ϣ
int upd()
{
	SQLCHAR up[100] = "update stu set ?=? where ѧ��=?";
	//SQLCHAR ss[100] = "update stu set �ɼ�=3 where ѧ��=2";
	SQLCHAR v1[100] = "update stu set ";
	SQLCHAR v2[100] = "=";
	SQLCHAR v3[100] = " where ѧ��=";
	SQLCHAR s1[100];
	SQLCHAR s2[100];
	SQLCHAR s3[100];
	char ch;
	printf("%s\n", up);
	//SQLPrepare(hstmt, up, 100);
	printf("����������Ҫ�޸ĵ�ѧ����ѧ��\n");
	ch = getchar();
	gets_s(s3, sizeof(s3));
	printf("��ѡ��Ҫ�޸ĵ�����\n");
	gets_s(s1, 5);
	printf("�������޸ĺ��ֵ\n");
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
	printf("%s\n", v1);
	SQLExecDirect(hstmt, v1, SQL_NTS);
	SQLCancel(hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		printf("�����ɹ�\n");
	}
	else {
		UCHAR errmsg[100],errmsg2[100];
		SQLError(henv, hdbc, hstmt,errmsg2, NULL, errmsg, strlen(errmsg), NULL);
		printf("%s\n", errmsg);
	}
}

//ͳ��ѧ����Ϣ
int coun()
{
	SQLCHAR s1[100] = "select row_number() over(order by �ɼ� desc),ѧ��,����,�ɼ� from stu";
	SQLCHAR s2[100] = "select top 1 ����,�ɼ� from stu order by �ɼ� desc";
	SQLCHAR s3[100] = "select top 1 ����,�ɼ� from stu order by �ɼ�";
	SQLCHAR s4[100] = "select avg(�ɼ�) from stu";
	float va = 0;
	int a;
	printf("��ѡ��Ҫ�鿴����Ϣ\n1.�鿴����ѧ������  2.�鿴������ֵ��ƽ��ֵ�� ");
	scanf_s("%d", &a);
	if (a == 1)
	{
		retcode = SQLExecDirect(hstmt, s1, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			SQLCHAR str1[50], str2[50], str3[50], str4[50];
			SQLLEN* len_s1, * len_s2, * len_s3, * len_s4;
			printf("����\tѧ��\t����\t�ɼ�\n");
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
			printf("��߷ֻ����:\n");
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
			printf("��ͷ���:\n");
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
			printf("ƽ����: ");
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

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv); //���뻷�����
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); //�������ݿ��������
	retcode = SQLConnect(hdbc, szDSN, strlen("bit"), szUID, strlen("kit"), szAuthStr, strlen("252625ILY"));
	//"kitΪ����Դ����

	if ((retcode == SQL_SUCCESS) || (retcode == SQL_SUCCESS_WITH_INFO))
	{
		printf("���ӳɹ�\n");
	}
	else
	{
		printf("���ݿ�����ʧ��\n");
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
		default: printf("������ѡ��\n");
		}
	}
	printf("���\n");
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);	//�ͷ�������
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);	//�ͷŻ������
	system("pause");
}