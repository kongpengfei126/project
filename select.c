/*************************************************************************
	> File Name: select.c
	> Author: 
	> Mail: 
	> Created Time: Wed 29 Aug 2018 04:54:34 PM CST
 ************************************************************************/

#include<stdio.h>
#include<mysql/mysql.h>
#include"cgi_base.h"
int main()
{
    //获取query_string
    //char buf[1024*10]={0};
    //if(GetQueryString(buf))
    //{
    // fprintf(stderr,"GetQueryString failed\n");
    // return 1;
    //}
    //1.0 连接数据库
    MYSQL* connect_fd=mysql_init(NULL);
    MYSQL* connect_ret=mysql_real_connect(connect_fd,"127.0.0.1","root","","TestDB",3306,NULL,0);
    if(connect_ret==NULL)
    {
        fprintf(stderr,"mysql connect failed\n");
        return 1;
    }
    fprintf(stderr,"mysql connect ok!\n");
    //2.0 拼装sql语句
    const char* sql="select * from WorldTime";
    //3.0 把sql语句发送到服务器--使用handler
    int ret=mysql_query(connect_fd,sql);
    if(ret<0)
    {
        fprintf(stderr,"mysql_query failed! %s\n",sql);
        return 1;
    }
    //4.0 读取并遍历服务器返回的结果
    MYSQL_RES* result=mysql_store_result(connect_fd);
    if(result==NULL)
    {
        fprintf(stderr,"mysql_store_result failed!\n");
        return 1;
    }
    //a>获取表 n行 n列
    int rows=mysql_num_rows(result);
    int fields=mysql_num_fields(result);
    //b>获取表结构
    MYSQL_FIELD* field=mysql_fetch_field(result);
    while(field!=NULL)
    {
        printf("%s\t",field->name);
        field=mysql_fetch_field(result);
    }
    printf("<br>");
    //c>获取具体值
    int i=0;
    for(;i<rows;++i)
    {
        MYSQL_ROW row=mysql_fetch_row(result);
        int j=0;
        for(;j<fields;++j)
        {
            printf("%s\t",row[j]);
        }
        printf("<br>");
    }
    printf("<br>");
    //5.0 断开连接
    mysql_close(connect_fd);
    return 0;
}
