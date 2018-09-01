/*************************************************************************
	> File Name: cgi_base.h
	> Author: 
	> Mail: 
	> Created Time: Wed 29 Aug 2018 12:31:15 PM CST
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
static int GetQueryString(char buf[])        //只在当前编译单元有效
{
    //1.0 从环境变量里获取方法
    char* method=getenv("REQUEST_METHOD");
    if(NULL==method)
    {
        //避免程序内部错误暴露给客户用stderr来输入日志
        fprintf(stderr,"[CGI]method==NULL\n");
        return -1;
    }
    //2.0 判断方法是GET？POST     GET-从环境变量里读取QUERY_STRING POST--CONTENT_LENGTH
    //把参数a=10&b=20拿出来
    if(strcasecmp(method,"GET")==0)
    {
        char* query_string=getenv("QUERY_STRING");
        if(NULL==query_string)
        {
            fprintf(stderr,"[CGI]query_string is NULL\n");
            return -1;
        }
        //拷贝之后buf'里内容：a=10&b=20
        strcpy(buf,query_string);
        fprintf(stderr,"[CGI]get----request\n");
    }
    else
    {
        char* content_length_str=getenv("CONTENT_LENGTH");
        if(NULL==content_length_str)
        {
            fprintf(stderr,"[CGI]content_length is NULL\n");
            return -1;
        }
        int content_length=atoi(content_length_str);
        int i=0;
        for(;i<content_length;++i)
        {
         //由于父进程把body写入管道  子进程把0号文件描述符重定向至管道
         read(0,&buf[i],1);
        }
        //buf 内容为----a=10&b=10
        buf[i]='\0';
        fprintf(stderr,"[CGI]post----request\n");
    }
    return 0;
}
