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

	status = cgiFormString("sno",  sno, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sno error!\n");
		return 1;
	}


	int ret;
	char sql[128] = "\0";
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
	//查询是否有这个人的存在
	sprintf(sql, "select * from information where sno=%s ", sno);
	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	MYSQL_RES *res;
	res = mysql_store_result(db);
	if (res == NULL)
	{
		fprintf(cgiOut,"mysql_store_result fail:%s\n", mysql_error(db));
		return -1;
	}

	 int num = (int)res->row_count;
	 if(num){

	sprintf(sql, "update  information set sel=1 where sno = '%s'", sno);
	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}


	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">删除学生成功！</h1>");
}else{
	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">该学生不存在！</h1>");
}
	mysql_close(db);

	return 0;
}
