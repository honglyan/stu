#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "cgic.h"


char * headname = "head.html";
char * footname = "footer.html";

int cgiMain()
{
  char cno[32] = "\0";
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

	status = cgiFormString("sno",sno, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get cname error!\n");
		return 1;
	}

	status = cgiFormString("cno",cno, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get cno error!\n");
		return 1;
	}

	int ret;
	char sql[500] = "\0";
  char sql1[500] = "\0";
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

		mysql_query(db, "set character set utf8");

	sprintf(sql, "insert into score(sno,cno) values('%s','%s')",sno,cno);

	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		fprintf(cgiOut, "%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}
  //输出选课后的结果
  sprintf(sql1, "select sno,name,cno,cname,cirdet,cgrade from stuinfo where sno ='%s'", sno);
	if ((ret = mysql_real_query(db, sql1, strlen(sql1) + 1)) != 0)
	{
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}
  fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">选课成功！</h1>");

  if ((ret = mysql_real_query(db, sql1, strlen(sql1) + 1)) != 0)
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

  fprintf(cgiOut, "<div class=\"container\"> <h2 class=\"text-center\">该学生的选课结果</h2>");

  fprintf(cgiOut,"<table class=\"table table-striped table-bordered\"><tr>");
  int i = 0;

  unsigned int fields;
  fields = mysql_num_fields(res);

  MYSQL_FIELD *mysql_filed;
  mysql_filed = mysql_fetch_fields(res);
  for (i = 0; i < fields ; i++)
  {
  	fprintf(cgiOut, "<th>%s</th>", mysql_filed[i].name);
  }
  fprintf(cgiOut,"</tr>");

  //访问每一条记录的值
  MYSQL_ROW  row;
  unsigned long  *len;

  while ((row = mysql_fetch_row(res)) != NULL)
  {
  	fprintf(cgiOut,"<tr>");
  	len = mysql_fetch_lengths(res);
  	for (i = 0; i < fields; i++)
  	{
  		fprintf(cgiOut,"<td>%.*s</td>", (int)len[i], row[i]);
  	}
  	fprintf(cgiOut,"</tr>");
  }
  fprintf(cgiOut,"</table></div>");
	fprintf(cgiOut, "select a course ok!\n");
	mysql_close(db);
	return 0;
}
