#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<sys/sendfile.h>
#include<sys/wait.h>
#include"http_server.h"
typedef struct sockaddr sackaddr;
typedef struct sockaddr_in sockaddr_in;
//一次从socket中读取一行数据把数据放至缓冲区读取失败返回-1   \n \r \r\n
int ReadLine(int sock,char buf[],ssize_t size)
{
    //1.0 从socket一次读取一个字符遇到换行返回
    char c='\0';
    ssize_t i=0;  //读取字符个数
    while(i<size-1&&c!='\n')  //结束条件：1.0 读的长度达到了缓冲区上限 2.0 读到了换行（多个）
    {
        ssize_t read_size=recv(sock,&c,1,0);
        if(read_size<=0)
        {
            return -1;
        }
        if(c=='\r')
        {
           //确定下一个字符--MSG_PEEK从缓冲区读取数据后--缓冲区的数据还在未删除
           recv(sock,&c,1,MSG_PEEK);
           if(c=='\n')    // \r\n
           {
            recv(sock,&c,1,0); 
           }
           else //当前分隔符为\r
           {
            c='\n';    
           }
        }    
        buf[i++]=c;
    }
    buf[i]='\0';
    return i;          //实际放至缓冲区数据长度
}
int Split(char input[],const char* split_char,char* output[],int output_size)
{
    //使用strtok函数
    char * pch;
    int i=0;
    char* temp;
    //pch = strtok (input,"split_char");       静态变量--不可重入--多线程不安全--temp用来保存上次分割结束的位置
    pch = strtok_r(input,split_char,&temp);        
    while (pch != NULL)
    {
        if(i>=output_size)
        {
          return i;
        }
        output[i++]=pch;
        pch = strtok_r(NULL,split_char,&temp);
    }
    return i;
}
int ParseFirstLine(char first_line[],char** p_url,char** p_method)   //修改指针指向--输出型参数
{
    //把首行按照空格进行字符串分割    
    char* tok[10];
    //切分得到的每个部分放在tok数组之中 返回值为tok数组中包含几个元素
    int tok_size=Split(first_line," ",tok,10);
    if(tok_size!=3)
    {
        printf("Split failed! tok_size=%d %s\n",tok_size,tok[0]);
        return -1;
    }
    *p_method=tok[0];
    *p_url=tok[1];
    return 0;
}
int ParseQueryString(char* url,char**p_url_path,char** p_query_string)
{
   *p_url_path=url;
    char* p=url;
    for(;*p!='\0';++p)
    {
        if(*p=='?')    //url_path分割完毕
        {
            *p='\0';
            *p_query_string=p+1;
            return 0;
        }
    }
    *p_query_string=NULL;
    return  0;
}
int ParseHeader(int sock,int* content_length)
{
    //循环从socket读取一行
    //判断是不是content_length  是读value  否则丢弃  读到空行结束  
    char buf[SIZE]={0};
    while(1)
    {
        ssize_t read_size=ReadLine(sock,buf,sizeof(buf));    //函数体内已经考虑了‘\0’
        //printf("read_size；   %d\n",read_size);
        //处理失败
        if(read_size<=0)
        {
            return -1;
        }
        //读到空行结束循环
        if(strcmp(buf,"\n")==0)
        {
            return 0;
        }
        const char* content_length_str="Content-Length: ";
        //看是不是content_length 
        if(NULL!=content_length_str
           &&strncmp(buf,content_length_str,strlen(content_length_str))==0)
        {
            *content_length=atoi(buf+strlen(content_length_str));
        }
        //memset(buf,0,SIZE);
    }
}
void Handler404(int sock)
{
    //构造完整的http响应--版本号状态吗404 body---返回404相关界面
    const char* first_line="HTTP/1,1 404 Not Fount\n";
    const char* type_line="Content-Type: text/html;charset=utf-8\n";
    const char* blank_line="\n";
    const char* html="<head><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"></head>"
    "<h1>您的页面未找到！！！</h1>";                 //浏览器以utf8格式  统一字符编码
    send(sock,first_line,strlen(first_line),0);
    send(sock,type_line,strlen(type_line),0);
    send(sock,html,strlen(html),0);
}
int IsDir(const char* file_path)
{
  struct stat st;
  //查看文件源信息
  int ret=stat(file_path,&st);
  if(ret<0) //文件不存在
  {
    return 0;
  }
  if(S_ISDIR(st.st_mode))  //按位&再判断是否相等
  {
    return 1;
  }
  return 0;
}
void HandlerFilePath(const char*url_path,char* file_path)
{
 // printf("1\n");
 //1.0 给url加上前缀（http服务器的根目录）
 //url_path--->/index.html
 //file_path--->./wwwroot/index.html
 sprintf(file_path,"./wwwroot%s",url_path);
 //2.0 例如 url_path---/ 此时它是目录-给目录里追加index.html 
    if(file_path[strlen(file_path)-1]=='/')
    {
        strcat(file_path,"index.html");          // url_path/  /image/
    }
    // url_path    /image
    if(IsDir(file_path))
    {
        // printf("1.1");
        strcat(file_path,"/index.html");    
    }
// printf("url_path:%s file_path:%s\n",url_path,file_path);
}
ssize_t GetFileSize(const char* file_path)
{
    struct stat st;
    int ret=stat(file_path,&st);
    if(ret<0)
    {
      return 0;     //打开文件失败--文件不存在
    }
    return st.st_size;
}
int WriteStartFile(int sock,char* file_path)
{
    //printf("2\n");
    //1.0 打开文件
    int fd=open(file_path,O_RDONLY);
    if(fd<0)
    {
     perror("open");
     return 404;
    }
    //2.0把构造出的HTTP响应写入socket
    //a. 写入首行
    const char* first_line="HTTP/1.1 200 OK\n";
    send(sock,first_line,strlen(first_line),0);
    //b.写入Header
    //const char* type_line="Content-Type: text/html;charset=utf-8\n";
    //const char* type_line="Content-Type: image/jpg;charset=utf-8\n";
    //send(sock,type_line,strlen(type_line),0);   //服务器自己识别类型
    //c.写入空行
    const char* blank_line="\n";
    send(sock,blank_line,strlen(blank_line),0);
    //d.写入body
    /*ssize_t file_size=GetFileSize(file_path);
    for(ssize_t i=0;i<file_size;++i)
    {
        char c;
        read(fd,&c,1);
        send(sock,&c,1,0);
    }
    */
    sendfile(sock,fd,NULL,GetFileSize(file_path));
    //3.0关闭文件
    close(fd);
    return 200;
}
int HandlerStaticFile(int sock,Request* req)
{
    // printf("Enter HandlerStaticFile\n");
    //1.0 根据url _path获取文件在服务器上的路径
    char file_path[SIZE]={0};
    HandlerFilePath(req->url_path,file_path);
    //printf("file_path: %s",file_path);
    //2.0 读取文件，把文件内容写到socket之中
    int err_code=WriteStartFile(sock,file_path);
    //printf("err_code: %d",err_code);
    return err_code;
}
int HandlerCGIFather(int sock,int father_read,int father_write,int child_pid,Request* req)
{
    //POST请求的body部分写入管道
    if(strcasecmp(req->method,"POST")==0)
    {
        int i=0;
        char c='\0';
        for(;i<req->content_length;++i)
        {
            read(sock,&c,1);
            write(father_write,&c,1);
        }
    }
    //构造HTTP响应
    const char* first_line="HTTP/1.1 200 OK\n";
    send(sock,first_line,strlen(first_line),0);
    const char* type_line="Content-Type: text/html;charset=utf-8\n";
    send(sock,type_line,strlen(type_line),0);
    const char* blank_line="\n";
    send(sock,blank_line,strlen(blank_line),0);
    char c='\0';
    //循环从管道中读取数据并写入到socket
    while(read(father_read,&c,1)>0)           //写端关闭--read返回0
    {
       send(sock,&c,1,0);
    }
    //回收子进程资源
    waitpid(child_pid,NULL,0);
    return 200;
}
int HandlerCGIChild(int child_read,int child_write,Request* req)
{
    //设置必要环境变量
    char method_env[SIZE]={0};
    sprintf(method_env,"REQUEST_METHOD=%s",req->method);
    putenv(method_env);                    //环境变量的设置
    //设置query_string/content_length
    if(strcasecmp(req->method,"GET")==0)
    {
        char query_string_env[SIZE]={0};
        sprintf(query_string_env,"QUERY_STRING=%s",req->url_query_string);
        putenv(query_string_env);
    }
    else
    {
        char content_length_env[SIZE]={0};
        sprintf(content_length_env,"CONTENT_LENGTH=%d",req->content_length);
        putenv(content_length_env);
    }
    //把标准输入输出重定向到管道
    dup2(child_read,0);       //oldfd newfd 把newfd重定向到oldfd
    dup2(child_write,1);
    //对子进程程序替换
    // url_path: /cgi-bin/test
    // file_path:  ./wwwroot/cgi-bin/test
    char file_path[SIZE]={0};
    HandlerFilePath(req->url_path,file_path);
    // l    l--环境变量  p --在Path路径找
    // lp
    // le
    // v
    // vp
    // ve
    execl(file_path,file_path,NULL);
    exit(1);
    return 200;
}
int HandlerCGI(int new_sock,Request* req)
{
    int err_code=200;
    //1.0 创建一对匿名管道
    int fda[2],fdb[2];
    int ret=pipe(fda);
    if(ret<0)
    {
        return 404;
    }
    ret=pipe(fdb);
    if(ret<0)
    {
        //释放之前的文件描述符
        close(fda[0]);
        close(fda[1]);
        return 404;
    }
    //父读a写b  孩子读b写a
    int father_read=fda[0];
    int child_write=fda[1];
    int father_write=fdb[1];
    int child_read=fdb[0];
    //2.0 创建子进程
    ret=fork();
    //3.0 父子进程各自执行不同的逻辑
    if(ret>0)
    {
        //father  ----父进程先关闭俩管道fd--后面父进程在读取fda的内容时能读到EOF---写端全闭--读到EOF
        close(child_read);
        close(child_write);
        err_code=HandlerCGIFather(new_sock,father_read,father_write,ret,req);
    }
    else if(ret==0)
    {
        //child
        err_code=HandlerCGIChild(child_read,child_write,req);
    }
    else
    {
        err_code=404;
        goto END;
    }
    //4.0 收尾错误处理
    END:
    close(fda[0]);
    close(fda[1]);
    close(fdb[0]);
    close(fdb[1]);
    return err_code;
}
void PrintfRequest(Request* req)
{       
    printf("method: %s\n",req->method);
    printf("url_path: %s\n",req->url_path);
    printf("url_query_string: %s\n",req->url_query_string);
    printf("content_length: %d\n",req->content_length);
}
void HandlerRequest(int new_sock)
{
    int err_code=200;

    //1.0 反序列化
    Request req;
    memset(&req,0,sizeof(req));
    //a.从socket中读取首行
    if(ReadLine(new_sock,req.first_line,sizeof(req.first_line))<0)
    {
        err_code=404;
        goto End;
    }
    //b.解析首行 url/method
    if(ParseFirstLine(req.first_line,&req.url,&req.method))
    {      
        err_code=404;
        goto End;
    }
    //c.解析url中的url_path和query_string
    if(ParseQueryString(req.url,&req.url_path,&req.url_query_string))
    {
        err_code=404;
        goto End;
    }
    //d. 处理Header--content_length
    if(ParseHeader(new_sock,&req.content_length))
    {    
       err_code=404;
       goto End;
    }
    PrintfRequest(&req);
    //2.静/动态生成页面结果写回client
    //a. 静态页面
    if(strcasecmp(req.method,"GET")==0&&req.url_query_string==NULL)    //不区分大小写
    {
       err_code= HandlerStaticFile(new_sock,&req);
    }
    //b.动态页面
    else if(strcasecmp(req.method,"GET")==0&&req.url_query_string!=NULL)
    {
       err_code= HandlerCGI(new_sock,&req);
    }   
    else if(strcasecmp(req.method,"POST")==0)
    {
       err_code= HandlerCGI(new_sock,&req);
    }
    else
    {
        err_code=404;
        goto End;
    }
    End: 
    if(err_code!=200)
    {
      Handler404(new_sock);
    }
    //printf("err_code: %d",err_code);
     close (new_sock);
     return;
}
void* ThreadEntry(void* arg)
{
    int64_t new_sock=(int64_t)arg;
    //数据处理（请求过程)
    HandlerRequest(new_sock);
    return NULL;
}
//服务器开启
void HttpServerStart(short port)
{
    int listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0)
    {
      perror("socket");
      return;
    }
    int opt=1;
    //设置选项-重用端口号--解决多客户端等待时间过长问题--重用time_wait连接
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(port);
    //绑定端口号
    int ret=bind(listen_sock,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0)
    {
        perror("bind");
        return;
    }
    ret=listen(listen_sock,5);
    if(ret<0)
    {
        perror("listen");
        return;
    }   
    printf("SeverInit ok\n");
    //循环读写
    while(1)
    {
        //获取对端的ip+端口号
        sockaddr_in peer;
        socklen_t len=sizeof(peer);
        int64_t new_sock=accept(listen_sock,(struct sockaddr*)&peer,&len);
        if(new_sock<0)
        {
            perror("accept");
            continue;
        }
        //处理读写情况多用户的情况下用多线程---TCP
        pthread_t tid;
        pthread_create(&tid,NULL,ThreadEntry,(void*)new_sock);
        pthread_detach(tid);
    }
}
//http_server  [ip] [port]
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage .http_server [port]\n");
        return 1;
    }
    //屏蔽管道信号
   // sigset_t sigset;
   // sigemptyset(&sigset);
   // sigaddset(&sigset,SIGPIPE);
   // sigprocmask(SIG_BLOCK,&sigset,NULL);
    HttpServerStart(atoi(argv[1]));
    return 0;
}
