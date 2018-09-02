/*************************************************************************
	> File Name: http_server.h
	> Author: 
	> Mail: 
	> Created Time: Sun 26 Aug 2018 09:33:38 PM CST
 ************************************************************************/

#pragma once
#define SIZE 10240
typedef struct Request
{
    char first_line[SIZE];
    char* method;
    //char version;
    char* url;
    char* url_path;   //路径
    char* url_query_string;   //参数
    int content_length;   //其它header只保留content_length
}Request; 
