#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include"huffumantree.h"
#pragma warning(disable:4996)      //��ĳ������4996��ΪʧЧ
using namespace std;
typedef long long Type;
 struct CharInfo
{
   char _ch;         //�ַ�
   Type _count;    // �ַ����ִ���
   string _strcode;      //�ַ�����
   CharInfo(Type count = 0)
	   :_count(count)
   {}
   //a[i]!=invalid  �ṹ�岻�Ⱥű����������Լ�����  ע��˳���Ҳ�Ϊconst CharInfo& 
   bool operator!=(const CharInfo& info)
   {
	   return _count!=info._count;               
   }
   //left->_weight + right->_weight  �ṹ��ӷ������������Լ�����
   Type operator+(const CharInfo& info)
   {
	   return _count + info._count;                  //---����ֱ�ӷ���_count�ĺ� ---����ù��캯��
   }
   //l->_weight > r->_weight  �ṹ����ںű����������Լ�����
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
		//��ʼ��
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
		//1.0 ͳ���ַ����ִ���
		while (true)
		{
			size_t readsize = fread(readbuffer, 1, 1024, fIN);  // buffer Ԫ���ֽ���  buffer�ֽ���  ����������
			if (readsize == 0)
				break;
			for (int i = 0; i < readsize; ++i)
			{
				_charInfo[readbuffer[i]]._count++;
			}
		}

		//2.0 ���ɹ�������--���ַ���_count��ΪȨֵ
		HuffmanTree<CharInfo>  hft(_charInfo, 256, CharInfo(0));
		//3.0 �õ�ÿ���ַ��Ĺ���������
		GetHuffmanCode(hft.GetRoot());
		//4.0 �����ַ���������д�ļ�

		FILE* fOut = fopen("2","w");
		assert(fOut);

		//дѹ��������Ϣ         // �ļ���׺ ��Ч�ַ�����  �ַ�-�ַ����ִ���  ��ѹ��Ϣ        ѹ��������
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
				itoa(_charInfo[i]._count,size,10);      //10����
				strcode += size;
				strcode += '\n';
				++linecount;
			}
				
		}
		memset((void *)size, 0, 32);         //ǰ���itoa  ֮���size ���ܻ�����ڵ�linecount���Ӱ��
		itoa(linecount,size,10);
		strFileHead += size;
		strFileHead += '\n';
		//дѹ������
		strFileHead += strcode;
		fwrite(strFileHead.c_str(), 1, strFileHead.size(), fOut);


		fseek(fIN, 0, SEEK_SET); //�ļ�ָ��  ƫ����  Դorigin
		char* writebuffer = new char[1024];
		char ch = 0;              //����λ
		char pos = 0;             //�����ַ�ch�Ƿ���8λ
		size_t writesize = 0;        //д���ܻ����
		while (true)
		{
			//fIN���ϴ�readʱ�Ѿ���EOF�� 
			size_t readsize = fread(readbuffer, 1, 1024, fIN);
			if (readsize == 0)
				break;
			for (int i = 0; i < readsize; ++i)
			{
				//��ȡ�ַ�����
				string& strcode = _charInfo[readbuffer[i]]._strcode;
				for (char j = 0; j < strcode.size();++j)
				{
					ch <<= 1;
					if (strcode[j] == '1')
						ch |= 1;
					++pos;
					if (8 == pos)   //��ǰ�ַ��Ѿ�д��
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
		if (pos < 8)          //���һ������8λ���ַ����Ʋ���
		{
			ch<<=(8 - pos);
			writebuffer[writesize++] = ch;
		}
			

		//readsize<1024  ���ǵ�д��fOutָ����ļ���
		fwrite(writebuffer, 1, writesize, fOut);

		//�ͷſռ�ر��ļ�
		delete[] readbuffer;
		delete[] writebuffer;

		fclose(fIN);
		fclose(fOut);
	}

   //��ѹ��---ѹ���ļ�ֻ�ж�������Ϣ
	void UnCompressFile(const string& filepath)
	{
		//1.0 ��ѹ���ļ�
		FILE* fIn = fopen(filepath.c_str(), "r");
 		assert(fIn);
	    //2.0 ��ȡ�ļ���׺��
		string strFilePosFix;
		ReadLine(fIn,strFilePosFix);
		//3.0 ��ȡ����
		string strInfo;
		ReadLine(fIn, strInfo);
		size_t linecount = atoi(strInfo.c_str());

		for (int i = 0; i < linecount;++i)
		{
			strInfo = "";
			ReadLine(fIn, strInfo);         //A ��1
			_charInfo[strInfo[0]]._count=atoi(strInfo.c_str() + 2);
		}

		HuffmanTree<CharInfo>  hft(_charInfo, 256, CharInfo(0));

		//��ѹ�����ļ�·��
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
			//�õ�һ������һ��
			for (size_t i = 0; i < readsize; ++i)
			{
				//8 ��
				pos = 7;
				while (pos>=0)
				{
					
					if (readbuffer[i] & (1 << pos))     //   ������
						pCur = pCur->_right;
					else                                //   ������
						pCur = pCur->_left;
					if (pCur->_left == NULL&&pCur->_right == NULL)  // Ҷ�ӽڵ�
					{
						writebuffer[writesize++] = pCur->_weight._ch;
						++UnFilesize;

						if (writesize == 1024)
						{
							fwrite(writebuffer, 1, 1024, fout);
							writesize = 0;
						}
						if (UnFilesize == Filesize)    // ��ѹ���
						{
							fwrite(writebuffer, 1, writesize, fout);
							fflush(fout);
							break;
						}
						//��Ҷ�ӽڵ��ת��root�������±���
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
	//��ȡ�ַ��Ĺ���������
	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* root)
	{
		if (NULL == root)
			return;
		// 1.0 ��Ҷ�ӽڵ�
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);
		if (NULL == root->_left&&NULL == root->_right)   //Ҷ�ӽڵ�
		{
			HuffmanTreeNode<CharInfo>* pCur = root;
			HuffmanTreeNode<CharInfo>* parent = root->_parent;
			string& strcode=_charInfo[pCur->_weight._ch]._strcode;
			//2.0 �õ�����
			while (parent)
			{
				if (parent->_left == pCur)
					strcode += '0';
				else
					strcode += '1';
				pCur = parent;
				parent = pCur->_parent;
			}
			//3.0 ����
			std::reverse(strcode.begin(), strcode.end());    
		}
	}
	//�õ��ļ���׺
	string GetFilePosFix(const string& filepath)
	{
		return filepath.substr(filepath.find_last_of('.'));
	}
	//һ�ζ�ȡһ��
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
	//�õ��ļ���
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