#include<iostream>
#include<vector>
#pragma once
using namespace std;
//heap 底部用vector实现的

//使用仿函数来实现大小堆
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
template<class T,class compare=More<T>>   //默认建大堆
class Heap
{
public:
	//heap的创建以数组创建的完全二叉树
	Heap()
	{}
	Heap(T* arr, size_t size)
	{
		if (arr)
		{
			for (size_t i = 0; i < size; ++i)
				_a.push_back(arr[i]);
			//向下调整从倒数第一个非叶子节点
			size_t size = _a.size();
			for (int i = (size - 2) >> 1; i >= 0; --i)
				Adjustdown(i,size);
		}
	}
	//堆删除
	void Pop()
	{
		if (!_a.empty())
		{
			//1.0 pop()之前先把第一个元素与最后一个元素值交换--删除还是删除的堆顶
			size_t size = _a.size();
			swap(_a[0], _a[size - 1]);
			_a.pop_back();
			//2.0 从堆顶向下调整   --pop之后_a.size()已经减1不能用原来的size
			Adjustdown(0,_a.size());
		}
	}
    //堆插入
	void Push(const T& value)
	{
		//1.0 堆尾插入 
		_a.push_back(value);
	    //2.0从堆尾向上调整
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
			cout <<"堆为空"<< endl;
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
	//排序会用到向下调整而且每次调整都会少一个元素 所以多传一个参数来控制需要调整多少个元素
	//向下调整
	void Adjustdown(size_t parent,size_t size)
	{
		size_t child = parent * 2 + 1;
		compare com;
		while (child<size)
		{
			if (child + 1<size&&com(_a[child + 1],_a[child]))    //越界限制
				++child;
			if (com(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);     //交换之后child可能向下不满足堆要求  循环起来
				parent = child;
				child = parent * 2 + 1;
			}
			else    //parent满足需求  不用交换那么循环结束
				break;
		}
	}
	//向上调整
	void Adjustup(size_t child)
	{
		size_t parent = (child - 1) >> 1;
		compare com;
		while (child>0)
		{
			if (com(_a[child], _a[parent]))
			{
				//1.0 child不满足要求 和parent交换
				swap(_a[parent], _a[child]);
				//2.0 向上更新  
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
				break;      //child满足需求  不用交换那么循环结束
		}
	}
	void _Show(vector<T>& _a)
	{
		size_t size = _a.size();
		for (size_t i= 0; i < size; ++i)
			cout <<_a[i]<< " ";
		cout << endl;
	}
	//堆排序
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
	//大小堆测试
	int arr[10] = { 12, 15, 48, 1, 5, 56, 79, 54, 45, 21 };
	Heap<int, Less<int> > hp(arr, sizeof(arr) / sizeof(arr[0]));
	hp.Pop();
	hp.Push(22);
	hp.Show();
	cout << hp.Top()<<" ";
	cout << hp.Back() << "   ";
    //堆排序测试
	hp.Heap_Sort();
	hp.Show();

}
