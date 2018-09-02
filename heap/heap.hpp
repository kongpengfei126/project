#include<iostream>
#include<vector>
#pragma once
using namespace std;
//heap �ײ���vectorʵ�ֵ�

//ʹ�÷º�����ʵ�ִ�С��
template<class T>
struct More
{
	bool operator()(const T& a, const T& b)
	{
		return a > b;
	}
};
template<class T>
struct Less
{
	bool operator()(const T& a, const T& b)
	{
		return a < b;
	}
};
template<class T,class compare=More<T>>   //Ĭ�Ͻ����
class Heap
{
public:
	//heap�Ĵ��������鴴������ȫ������
	Heap()
	{}
	Heap(T* arr, size_t size)
	{
		if (arr)
		{
			for (size_t i = 0; i < size; ++i)
				_a.push_back(arr[i]);
			//���µ����ӵ�����һ����Ҷ�ӽڵ�
			size_t size = _a.size();
			for (int i = (size - 2) >> 1; i >= 0; --i)
				Adjustdown(i,size);
		}
	}
	//��ɾ��
	void Pop()
	{
		if (!_a.empty())
		{
			//1.0 pop()֮ǰ�Ȱѵ�һ��Ԫ�������һ��Ԫ��ֵ����--ɾ������ɾ���ĶѶ�
			size_t size = _a.size();
			swap(_a[0], _a[size - 1]);
			_a.pop_back();
			//2.0 �ӶѶ����µ���   --pop֮��_a.size()�Ѿ���1������ԭ����size
			Adjustdown(0,_a.size());
		}
	}
    //�Ѳ���
	void Push(const T& value)
	{
		//1.0 ��β���� 
		_a.push_back(value);
	    //2.0�Ӷ�β���ϵ���
		Adjustup(_a.size()-1);
	}
	void Show()
	{
		_Show(_a);
	}
	const T& Top()const
	{
		if (!_a.empty())
		   return _a[0];
		else
		{
			cout <<"��Ϊ��"<< endl;
		}
	}
	T& Back()
	{
		return _a[_a.size() - 1];
	}
	bool Empty()
	{
		return _a.empty();
	}
	size_t Size()
	{
		return _a.size();
	}
	void Heap_Sort()
	{
		_Heap_Sort(_a);
	}
protected:
	//������õ����µ�������ÿ�ε���������һ��Ԫ�� ���Զഫһ��������������Ҫ�������ٸ�Ԫ��
	//���µ���
	void Adjustdown(size_t parent,size_t size)
	{
		size_t child = parent * 2 + 1;
		compare com;
		while (child<size)
		{
			if (child + 1<size&&com(_a[child + 1],_a[child]))    //Խ������
				++child;
			if (com(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);     //����֮��child�������²������Ҫ��  ѭ������
				parent = child;
				child = parent * 2 + 1;
			}
			else    //parent��������  ���ý�����ôѭ������
				break;
		}
	}
	//���ϵ���
	void Adjustup(size_t child)
	{
		size_t parent = (child - 1) >> 1;
		compare com;
		while (child>0)
		{
			if (com(_a[child], _a[parent]))
			{
				//1.0 child������Ҫ�� ��parent����
				swap(_a[parent], _a[child]);
				//2.0 ���ϸ���  
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
				break;      //child��������  ���ý�����ôѭ������
		}
	}
	void _Show(vector<T>& _a)
	{
		size_t size = _a.size();
		for (size_t i= 0; i < size; ++i)
			cout <<_a[i]<< " ";
		cout << endl;
	}
	//������
	void _Heap_Sort(vector<T>& _a)
	{
		size_t size = _a.size();
		for (int i = size-1; i>=0; --i)
		{
			swap(_a[0], _a[i]);
			Adjustdown(0, i);
		}
	}
private:
	vector<T> _a;
};


void Heap_Test()
{
	//��С�Ѳ���
	int arr[10] = { 12, 15, 48, 1, 5, 56, 79, 54, 45, 21 };
	Heap<int, Less<int> > hp(arr, sizeof(arr) / sizeof(arr[0]));
	hp.Pop();
	hp.Push(22);
	hp.Show();
	cout << hp.Top()<<" ";
	cout << hp.Back() << "   ";
    //���������
	hp.Heap_Sort();
	hp.Show();

}
