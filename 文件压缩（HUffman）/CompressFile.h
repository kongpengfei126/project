#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include"huffumantree.h"
#pragma warning(disable:4996)      //将某个警报4996置为失效
using namespace std;
typedef long long Type;
 struct CharInfo
{
   char _ch;         //字符
   Type _count;    // 字符出现次数
   string _strcode;      //字符编码
   CharInfo(Type count = 0)
	   :_count(count)
   {}
   //a[i]!=invalid  结构体不等号编译器不认自己重载  注意顺序右侧为const CharInfo& 
   bool operator!=(const CharInfo& info)
   {
	   return _count!=info._count;               
   }
   //left->_weight + right->_weight  结构体加法编译器不认自己重载
   Type operator+(const CharInfo& info)
   {
	   return _count + info._count;                  //---可以直接返回_count的和 ---会调用构造函数
   }
   //l->_weight > r->_weight  结构体大于号编译器不认自己重载
   bool operator>(const CharInfo& info) const
   {
	   return _count > info._count;
   }
};
class Comparess_and_Uncomparess
{
public:
	struct ConfigInfo
	{
		char _ch;
		Type _count;
	};
	typedef HuffmanTreeNode<CharInfo> Node;

	Comparess_and_Uncomparess()
	{
		//初始化
		for (int i = 0; i < 256; ++i)
		{
			_charInfo[i]._ch = i;
		}
	}

	void CompressFile(const string& filepath)
	{
		FILE* fIN = fopen(filepath.c_str(), "r");
		assert(fIN);
		char* readbuffer = new char[1024];
		//1.0 统计字符出现次数
		while (true)
		{
			size_t readsize = fread(readbuffer, 1, 1024, fIN);  // buffer 元素字节数  buffer字节数  输入流对象
			if (readsize == 0)
				break;
			for (int i = 0; i < readsize; ++i)
			{
				_charInfo[readbuffer[i]]._count++;
			}
		}

		//2.0 生成哈夫曼树--以字符的_count作为权值
		HuffmanTree<CharInfo>  hft(_charInfo, 256, CharInfo(0));
		//3.0 得到每个字符的哈夫曼编码
		GetHuffmanCode(hft.GetRoot());
		//4.0 按照字符编码来改写文件

		FILE* fOut = fopen("2","w");
		assert(fOut);

		//写压缩编码信息         // 文件后缀 有效字符行数  字符-字符出现次数  解压信息        压缩后数据
		string strFileHead;
		strFileHead+=GetFilePosFix(filepath) ;
		strFileHead += '\n';
		string strcode;
		char size[32];
		size_t linecount = 0;
		for (size_t i = 0; i < 256; ++i)
		{
			if (_charInfo[i]._count)
			{
				strcode += _charInfo[i]._ch;
				strcode += ',';
				itoa(_charInfo[i]._count,size,10);      //10进制
				strcode += size;
				strcode += '\n';
				++linecount;
			}
				
		}
		memset((void *)size, 0, 32);         //前面的itoa  之后的size 可能会对现在的linecount造成影响
		itoa(linecount,size,10);
		strFileHead += size;
		strFileHead += '\n';
		//写压缩数据
		strFileHead += strcode;
		fwrite(strFileHead.c_str(), 1, strFileHead.size(), fOut);


		fseek(fIN, 0, SEEK_SET); //文件指针  偏移量  源origin
		char* writebuffer = new char[1024];
		char ch = 0;              //比特位
		char pos = 0;             //计数字符ch是否满8位
		size_t writesize = 0;        //写可能会溢出
		while (true)
		{
			//fIN在上次read时已经在EOF处 
			size_t readsize = fread(readbuffer, 1, 1024, fIN);
			if (readsize == 0)
				break;
			for (int i = 0; i < readsize; ++i)
			{
				//提取字符编码
				string& strcode = _charInfo[readbuffer[i]]._strcode;
				for (char j = 0; j < strcode.size();++j)
				{
					ch <<= 1;
					if (strcode[j] == '1')
						ch |= 1;
					++pos;
					if (8 == pos)   //当前字符已经写满
					{
						writebuffer[writesize++] = ch;
						if (writesize == 1024)
						{
							fwrite(writebuffer, 1, 1024, fOut);
							writesize = 0;
						}
						//ch = 0;
						pos = 0;
					}
				}
			}
		}
		if (pos < 8)          //最后一个不满8位的字符左移补齐
		{
			ch<<=(8 - pos);
			writebuffer[writesize++] = ch;
		}
			

		//readsize<1024  还是得写到fOut指向的文件里
		fwrite(writebuffer, 1, writesize, fOut);

		//释放空间关闭文件
		delete[] readbuffer;
		delete[] writebuffer;

		fclose(fIN);
		fclose(fOut);
	}

   //解压缩---压缩文件只有二进制信息
	void UnCompressFile(const string& filepath)
	{
		//1.0 读压缩文件
		FILE* fIn = fopen(filepath.c_str(), "r");
 		assert(fIn);
	    //2.0 读取文件后缀名
		string strFilePosFix;
		ReadLine(fIn,strFilePosFix);
		//3.0 读取行数
		string strInfo;
		ReadLine(fIn, strInfo);
		size_t linecount = atoi(strInfo.c_str());

		for (int i = 0; i < linecount;++i)
		{
			strInfo = "";
			ReadLine(fIn, strInfo);         //A ，1
			_charInfo[strInfo[0]]._count=atoi(strInfo.c_str() + 2);
		}

		HuffmanTree<CharInfo>  hft(_charInfo, 256, CharInfo(0));

		//解压缩的文件路径
	/*	string strUncompressFile = GetFileName(filepath);
		strUncompressFile += strFilePosFix;*/

		FILE* fout = fopen("3.txt","w");
		assert(fout);


		char* readbuffer = new char[1024];
		char* writebuffer = new char[1024];

		int pos = 7;
		HuffmanTreeNode<CharInfo>* pCur = hft.GetRoot();
		size_t writesize = 0;
		size_t Filesize = pCur->_weight._count;
		size_t UnFilesize = 0;
		while(true)
		{
		    size_t readsize = fread(readbuffer, 1, 1024, fIn);
			if (readsize == 0)
				return;
			//拿到一个解析一个
			for (size_t i = 0; i < readsize; ++i)
			{
				//8 出
				pos = 7;
				while (pos>=0)
				{
					
					if (readbuffer[i] & (1 << pos))     //   往右走
						pCur = pCur->_right;
					else                                //   往左走
						pCur = pCur->_left;
					if (pCur->_left == NULL&&pCur->_right == NULL)  // 叶子节点
					{
						writebuffer[writesize++] = pCur->_weight._ch;
						++UnFilesize;

						if (writesize == 1024)
						{
							fwrite(writebuffer, 1, 1024, fout);
							writesize = 0;
						}
						if (UnFilesize == Filesize)    // 解压完成
						{
							fwrite(writebuffer, 1, writesize, fout);
							fflush(fout);
							break;
						}
						//是叶子节点才转到root重新向下遍历
						pCur = hft.GetRoot();
					}
					--pos;
				}
			}
		}
		delete[] readbuffer;
		delete[] writebuffer;

		fclose(fIn);
		fclose(fout);
 	}
protected:
	//获取字符的哈夫曼编码
	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* root)
	{
		if (NULL == root)
			return;
		// 1.0 找叶子节点
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);
		if (NULL == root->_left&&NULL == root->_right)   //叶子节点
		{
			HuffmanTreeNode<CharInfo>* pCur = root;
			HuffmanTreeNode<CharInfo>* parent = root->_parent;
			string& strcode=_charInfo[pCur->_weight._ch]._strcode;
			//2.0 得到编码
			while (parent)
			{
				if (parent->_left == pCur)
					strcode += '0';
				else
					strcode += '1';
				pCur = parent;
				parent = pCur->_parent;
			}
			//3.0 逆序
			std::reverse(strcode.begin(), strcode.end());    
		}
	}
	//得到文件后缀
	string GetFilePosFix(const string& filepath)
	{
		return filepath.substr(filepath.find_last_of('.'));
	}
	//一次读取一行
	void ReadLine(FILE* fIn, string& strInfo)
	{
		char ch = fgetc(fIn);
		if(ch == EOF)
			return;
		while (ch != '\n')
		{
			strInfo += ch;
			ch = fgetc(fIn);
			if (EOF == ch)
				return;
		}
	}
	//得到文件名
	string GetFileName(const string& filepath)           
	{
		size_t pos = filepath.find_last_of('.');
		return filepath.substr(0,filepath.find_last_of('.')); 
	}
private:
	CharInfo _charInfo[256];
};
void Comparess_and_Uncomparess_Test()
{
	Comparess_and_Uncomparess cau;
	cau.CompressFile("1.txt");
	cau.UnCompressFile("2");
}