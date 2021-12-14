#工程简介
一个期末作业
使用了C语言通过ODBC连接到SQLServer并且对数据库进行基本的增删查改操作

#参考资料
[ODBC文档](https://docs.microsoft.com/zh-cn/sql/odbc/reference/sample-odbc-program?view=sql-server-ver15)

#工程简介
一个作业


#项目依赖
数据库：sqlserver

构建软件：vs2019


#参考文档
[ODBC](https://docs.microsoft.com/zh-cn/sql/odbc/reference/syntax/odbc-api-reference?view=sql-server-ver15)


#有关数据源配置
安装好SQLserver2012 R2后，确保环境可用，win+s搜索ODBC数据源
配置系统DSN
在项目源码中main函数配置DSN(数据源名称), UID(数据库用户名), password(数据库用户密码)