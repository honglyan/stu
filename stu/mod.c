#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "cgic.h"

char * headname = "head.html";
char * footname = "footer.html";

int cgiMain()
{

	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");

	char sno[32] = "\0";
	char name[32] = "\0";
	char birthday[12]= "\0";
	char sid[10] ="\0";
	int status = 0;
	char ch;

  //输出头网页head.html
	FILE * fd;
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
//从网页中取出各个需要的值
	status = cgiFormString("name",  name, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get name error!\n");
		return 1;
	}

	status = cgiFormString("sno",  sno, 32);
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

	status = cgiFormString("sid",sid, 10);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sid error!\n");
		return 1;
	}


	int ret;
	char sql[128] = "\0";
	MYSQL *db;
	//初始化
	db = mysql_init(NULL);
	mysql_options(db, MYSQL_SET_CHARSET_NAME, "utf8");
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
	  sprintf(sql, "update information set name='%s',birthday='%s',sid=%d where sno='%s' ", name,birthday,atoi(sid),sno);

	   if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	   {
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	  }

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">修改成功！</h1>");

  //输出修改后的结果
	char sql1[128] = "\0";
	sprintf(sql1, "select distinct sno,name,sex,birthday,sname,sdept,smajor,sclass from stuall where sno = '%s'", sno);

  if ((ret = mysql_real_query(db, sql1, strlen(sql1) + 1)) != 0)
  {
	fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
	mysql_close(db);
	return -1;
  }

  res = mysql_store_result(db);
  if (res == NULL)
  {
	fprintf(cgiOut,"mysql_store_result fail:%s\n", mysql_error(db));
	return -1;
}

  fprintf(cgiOut, "<div class=\"container\"> <h2 class=\"text-center\">修改后的结果</h2>");

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
}else{
fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">该学生不存在！</h1>");
}

	mysql_close(db);
	return 0;
}
