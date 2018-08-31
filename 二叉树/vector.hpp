#include<iostream>
#include<assert.h>
#pragma once
#pragma warning(disable:4996)
using namespace std;
template<class T>
class Vector
{
public:
	Vector()
		:_start(NULL)
		, _finish(NULL)
		, _endofstorage(NULL)
	{}
	Vector(T* a, size_t size)
		:_start(new T[size])
	{
		for (intv i = 0; i < size; ++i)
		{
			_start[i] = a[i];
		}
		//更新尾部指针
		_finish = _start + size;
		_endofstorage = _finish;
	}
	Vector(const Vector<T>& v)
	{
		int size = v.size();
		_start = new T[size];
		for (intv i = 0; i < size; ++i)
		{
			_start[i] = v[i];
		}
		_finish = _start + size;
		_endofstorage = _finish;
	}
	Vector<T>& operator=(const Vector<T>& v)
	{
		for (size_t i = 0; i < v.Size(); ++i)
			PushBack(v._start[i]);
		return *this;
	}
	~Vector()
	{
		delete[] _start;
		_finish = _endofstorage = _start = NULL;
	}
	void PushBack(const T& data)
	{
		//检测容量--增容
		_CheckCapacity();
		(*_finish) = data;
		++_finish;
	}
	void PopBack()
	{
		--_finish;
	}
	void Insert(size_t pos, const T& data)
	{
		//参数检测
		size_t size = Size();
		if (pos > size)
			return;
		_CheckCapacity();
		for (size_t i = size-1; i>=pos; --i)
		{
			_start[i+1] = _start[i];
		}
		_start[pos] = data;
		++_finish;
	}
	void Erase(size_t pos)
	{
		size_t size = Size();
		if (pos >=size)
			return;
		for (size_t i = pos+1; i <size; ++i)
		{
			_start[i-1] = _start[i];
		}
		--_finish;
	}
	size_t Size() const
	{
		return _finish - _start;
	}
	size_t Capacity() const
	{
		return _endofstorage-_start;
	}
	bool Empty()
	{
		return _start == _finish;
	}
	T& operator[](size_t index)
	{
		assert(index < Size());
		return _start[index];
	}
	void Clear()
	{
		_finish = _start;
	}
	void Resize(size_t newsize,const T& data=T())
	{
		size_t oldsize = Size();
		if (newsize>oldsize)
		{
			for (size_t i = oldsize; i <newsize; ++i)
			{
				_CheckCapacity();
				_start[i] = data;
				++_finish;
			}
		}
		else
		{
			_finish = _start + newsize;
		}
	}
protected:
	void _CheckCapacity()
	{
		if (_finish == _endofstorage)
		{
			size_t oldsize = Size();
			size_t newsize = 2 * oldsize + 3;
			T* temp = new T[newsize];

			//拷贝内容
			if (_start)
			{
				//1.0 memcpy()     效率高  自定义类型程序崩溃     两个指针指向同一片空间--释放后成为野指针    
				/*memcpy(temp, _start, oldsize*sizeof(T));*/  
				//2.0 for
				for (size_t i = 0; i < oldsize; ++i)      //不出错 效率低
					temp[i] = _start[i];
				delete[] _start;
			}
			_start = temp;
			_finish = _start + oldsize;          //+Size()有问题  _finish为原来的
			_endofstorage = _start + oldsize * 2 + 3;
		}
	}
	//判定自定义类型
	//bool IsPoDType()
	//{
	//	char* str[] = { "char", "int", "short", "long", "long long" };
	//	const char* pType = typeid(T).name();
	//	for (int i = 0; i < sizeof(str) / sizeof(str[0]); ++i)
	//	{
	//		if (0 == strcmp(str[i], pType))
	//			reurn true;
	//	}
	//	return false;
private:
	T* _start;
	T* _finish;         //size有效元素
	T* _endofstorage;   //容量
};



void vector_test()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	for (size_t i = 0; i < v.Size(); ++i)
		cout << v[i] << " ";
	cout << endl;
	v.PushBack(4);
	cout <<v.Size()<< endl;
	cout <<v.Capacity()<<endl;
	for (size_t i = 0; i < v.Size(); ++i)
		cout <<v[i]<<" ";
	cout << endl;
	v.Insert(5, 5);
	v.Erase(3);
	v.Resize(6,0);
	Vector<int> v2;
	v2 = v;
	for (size_t i = 0; i < v2.Size(); ++i)
		cout << v2[i] << " ";
	cout << endl;
}

class String
{
public:
	String(char* str = "")
	{
		if (NULL == str)
		{
			_str = new char[1];
			*_str = '\0';
		}
		else
		{
			_str = new char[strlen(str) + 1];
			strcpy(_str, str);
		}
	}
	String(const String& str)
		:_str(new char[strlen(str._str) + 1])
	{
		strcpy(_str, str._str);
	}
	String& operator=(const String& s)
	{
		if (&s != this)
		{
			char* temp = new char[strlen(s._str) + 1];
			strcpy(temp, s._str);

			delete[] _str;
			_str = temp;
		}
		return *this;
	}
	~String()
	{
		if (_str)
			delete[] _str;
	}
	friend ostream& operator<<(ostream& _cout, const String& s);
private:
	char* _str;
};
ostream& operator<<(ostream& _cout, const String& s)
{
	cout << s._str;
	return _cout;
}
void vector_test_string()
{
	Vector<String> v;
	v.PushBack("1111");
	v.PushBack("2222");
	v.PushBack("3333");

	for (size_t i = 0; i < v.Size(); ++i)
		cout << v[i] << " ";
	cout << endl;

	v.PushBack("4444");
	cout << v.Size() << endl;
	cout << v.Capacity() << endl;

	for (size_t i = 0; i < v.Size(); ++i)
		cout << v[i] << " ";
	cout << endl;
}