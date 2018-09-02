/*************************************************************************
	> File Name: worldtime.c
	> Author: 
	> Mail: 
	> Created Time: Thu 30 Aug 2018 02:07:15 PM CST
 ************************************************************************/

#include<stdio.h>
#include<mysql/mysql.h>
#include<time.h>
#include"cgi_base.h"
int main()
{
    //a.0 获取query_string
    char query_string[1024*4]={0};
    int ret=GetQueryString(query_string);
    if(ret<0)
    {
        fprintf(stderr,"getquerystring failed\n");
        return 1;
    }
    //query_stringtring: %e4%ba%91%e5%8d%97            
      char city[1024*4]={0};      
      sscanf(query_string,"city=%s",city); 
    //1.0 连接数据库
    MYSQL* connect_fd=mysql_init(NULL);
    MYSQL* connect_ret=mysql_real_connect(connect_fd,"127.0.0.1","root","","TestDB",3306,NULL,0);
    if(connect_ret==NULL)
    {
        fprintf(stderr,"mysql connect failed\n");
        mysql_close(connect_fd);
        return 1;
    }
    fprintf(stderr,"mysql connect ok!\n");
    //2.0 拼装sql语句
    char sql[1024*4]={0};
    sprintf(sql,"select timedis from WorldTime where city='%s'",city);
    //3.0 把sql语句发送到服务器--使用handler
    ret=mysql_query(connect_fd,sql);
    if(ret<0)
    {
        fprintf(stderr,"mysql_query failed! %s\n",sql);
        mysql_close(connect_fd);
        return 1;
    }
    //4.0 读取并遍历服务器返回的结果
    MYSQL_RES* result=mysql_store_result(connect_fd);
    if(result==NULL)
    {
        fprintf(stderr,"mysql_store_result failed!\n");
        mysql_close(connect_fd);
        return 1;
    }
    
    //c>获取具体值
    MYSQL_ROW row=mysql_fetch_row(result);
    if(NULL==row)
    {
        printf("<html>");
        printf("没找到，城市不存在！");
        printf("</html>");
        mysql_close(connect_fd);
        return 1;
    }
    double time_dis=atof(row[0]);
    
    //5.0 断开连接
    mysql_close(connect_fd);
   //获取当前时间
   time_t t=time(NULL);
   t+=time_dis*3600;
   struct tm* tp=localtime(&t);
   printf("<html>");
   printf("%02d:%02d",tp->tm_hour,tp->tm_min);
   printf("</html>");
   return 0;
}

