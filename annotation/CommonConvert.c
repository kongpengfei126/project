#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)   
enum STATE
{
	NUL_STATE,
	C_STATE,
	CPP_STATE,
	END_STATE
};

void NulConvert(FILE *pfIn, FILE *pfOut, enum STATE *psta)
{
	int first = 0;
	int second = 0;
	first = fgetc(pfIn);   //读取第一个字符
	switch (first)
	{
	case '/':

		second = fgetc(pfIn);   //读取第二个字符
		switch (second)
		{
		case '*':              // 读取到/*  进入c格式
			fputc('/', pfOut);
			fputc('/', pfOut);
			*psta = C_STATE;
			break;
		case '/':             //  读取到//   进入c++ 模式
			fputc('/', pfOut);
			fputc('/', pfOut);
			*psta = CPP_STATE;
			break;
		default:             //     表示 没有  开始转换  
			ungetc(second, pfIn);      //   第二个字符 还回去
			*psta = NUL_STATE;
			break;
		}
		break;
	case EOF:                       //  结束
		*psta = END_STATE;
		break;
	default:                       //  普通 字符    写到 pfOut流中 
		fputc(first, pfOut);
		*psta = NUL_STATE;
		break;
	}
}
void CConvert(FILE *pfIn, FILE *pfOut, enum STATE *psta)
{
	int first = 0;
	int second = 0;
	int third = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '*':            // 判断是否结束
		second = fgetc(pfIn);           // 获取结束的第二个字符  看是否结束
		switch (second)
		{
		case '/':		       //  表示c 模式 结束
			third = fgetc(pfIn);   // 获取第三个字符，用来判断连续注释以及换行问题
			if (third != '\n' && third != EOF)
				ungetc(third, pfIn);
			fputc('\n', pfOut);
			*psta = NUL_STATE;
			break;
		default:
			ungetc(second, pfIn);
			fputc(first, pfOut);
			*psta = C_STATE;
			break;
		}
		break;
	case '\n':           //   用来 解决多行注释
		fputc('\n', pfOut);
		fputc('/', pfOut);
		fputc('/', pfOut);
		*psta = C_STATE;
		break;
	default:
		fputc(first, pfOut);
		*psta = C_STATE;
		break;
	}
}

void CppCovert(FILE *pfIn, FILE *pfOut, enum STATE *psta)
{
	int first = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '\n':
		fputc(first, pfOut);
		*psta = NUL_STATE;
		break;
	case EOF:
		*psta = END_STATE;
		break;
	default:
		fputc(first, pfOut);
		*psta = CPP_STATE;
		break;
	}
}
void DoConvert(FILE *pfIn, FILE *pfOut, enum STATE *psta)
{
	while (*psta != END_STATE)
	{
		switch (*psta)
		{
		case C_STATE:
			CConvert(pfIn, pfOut, psta);
			break;
		case CPP_STATE:
			CppCovert(pfIn, pfOut, psta);
			break;
		case NUL_STATE:
			NulConvert(pfIn, pfOut, psta);
			break;
		default:
			break;
		}
	}
	printf("转换成功！\n");
}


void CommentConvert()
{
	enum STATE psta = NUL_STATE;
	FILE *pfIn = fopen("input.c", "r"); //打开input.c流
	if (pfIn == NULL)
	{
		perror("input.c");
		exit(EXIT_FAILURE);
	}
	FILE *pfOut = fopen("output.c", "w"); //打开 output.c流
	if (pfOut == NULL)
	{
		perror("output");
		fclose(pfIn);
		exit(EXIT_FAILURE);
	}
	DoConvert(pfIn, pfOut, &psta);
	fclose(pfIn);
	fclose(pfOut);
}

void Test()
{
	CommentConvert();
}
