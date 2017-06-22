#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "cgic.h"


char * headname = "head.html";
char * footname = "footer.html";

int cgiMain()
{
  char sno[32] = "\0";
	char name[32] = "\0";
	char sex[4] = "\0";
	char birthday[12]= "\0";
	char sid[10] ="\0";
	int status = 0;
	char ch;

	FILE * fd;
		fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");
		if(!(fd = fopen(headname, "r"))){
			fprintf(cgiOut, "Cannot open file, %s\n", headname);
			return -1;
		}
		ch = fgetc(fd);

		while(ch != EOF){
			fprintf(cgiOut, "%c", ch);
			ch = fgetc(fd);
		}
		fclose(fd);

	status = cgiFormString("name",name, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get name error!\n");
		return 1;
	}

	status = cgiFormString("sex",sex, 4);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sex error!\n");
		return 1;
	}

	status = cgiFormString("sno",sno, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sno error!\n");
		return 1;
	}

	status = cgiFormString("birthday",birthday, 12);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get birthday error!\n");
		return 1;
	}

	status = cgiFormString("sid",sid, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sid error!\n");
		return 1;
	}

	//fprintf(cgiOut, "name = %s, age = %s, stuId = %s\n", name, age, stuId);

	//int ret;
	char sql[500] = "\0";
	MYSQL *db;

	//初始化
	db = mysql_init(NULL);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_init fail:%s\n", mysql_error(db));
		return -1;
	}

	//连接数据库
	db = mysql_real_connect(db, "127.0.0.1", "root", "123456", "stu",  3306, NULL, 0);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_real_connect fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}



	/*strcpy(sql, "create table information(sno int(4) primary key check(sno>0),
  name varchar(8) not null,sex char(5)not null,birthday date default '1990-1-1',sel int default '0',sid int(4),
  foreign key(sid) references school(sid))default CHARSET=utf8");
	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		if (ret != 1)
		{
			fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
			mysql_close(db);
			return -1;
		}
	}*/

    //fprintf(cgiOut,"insert into information(sno,name,sex,birthday,sid)  values(%d,'%s','%s','%s',%d)",atoi(sno),name,sex,birthday,atoi(sid));
		mysql_query(db, "set character set utf8");
	sprintf(sql, "insert into information(sno,name,sex,birthday,sid)  values('%s','%s','%s','%s',%d)",sno,name,sex,birthday,atoi(sid));
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		fprintf(cgiOut, "%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">添加学生成功！</h1>");
	mysql_close(db);
	return 0;
}
