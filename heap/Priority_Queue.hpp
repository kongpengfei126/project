#include<iostream>
#include"heap.hpp"
#pragma once
using namespace std;
template<class T, class compare = More<T>>
class Priority_Queue
{
public:
	void Pop()
	{
		return hp.Pop();
	}
	void Push(const T& value)
	{
		return hp.Push(value);
	}
	//∂”Õ∑
	const T& Front()const
	{
		return hp.Top();
	}
	//∂”Œ≤
	T& Back()
	{
		return hp.Back();
	}
private:
	Heap<T,compare> hp;
};
void Priority_Queue_Test()
{
	Priority_Queue<int, Less<int>> pq;
	int arr[10] = { 12, 15, 48, 1, 5, 56, 79, 54, 45, 21 };
	int size=sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
		pq.Push(arr[i]);
	pq.Pop();
	pq.Pop();
	cout<<pq.Front()<< " ";
	cout << pq.Back() << " ";
}
