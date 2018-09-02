#include<iostream>
#include<assert.h>
#pragma once
using namespace std;
template<class T>
struct ListNode
{
	ListNode(const T& data = T())
	:_pre(NULL)
	, _next(NULL)
	, _data(data)
	{}
	ListNode* _pre;
	ListNode* _next;
	T _data;
};

template<class T>
class List_iterator
{
public:
	typedef ListNode<T> Node;
	typedef Node* PNode;
	typedef List_iterator<T> self;

	List_iterator()
		:_pHead(NULL)
	{}
	List_iterator(PNode& pCur)
		:_pHead(pCur)
	{}
	List_iterator(const self& l)
		:_pHead(l->_pHead)
	{}
	self& operator++()
	{
		_pHead = _pHead->_next;
		return *this;
	}
	self operator++(int)
	{
		PNode temp(*this);
		_pHead = _pHead->_next;
		return temp;
	}
	self& operator--()
	{
		_pHead = _pHead->_pre;
		return *this;
	}
	self operator--(int)
	{
		PNode temp(*this);
		_pHead = _pHead->_pre;
		return temp;
	}
	bool operator==(const self&  l)
	{
		return _pHead == l._pHead;
	}
	bool operator!=(const self&  l)
	{
		return _pHead != l._pHead;
	}
	T& operator*()
	{
		return _pHead->_data;
	}
	T* operator->()
	{
		/*return &(operator*());*/
		return &(_pHead->_data);
	}
private:
	PNode _pHead;
};
template<class T>
class List
{
public:
	typedef ListNode<T> Node;
	typedef Node* PNode;
	typedef List_iterator<T>  iterator;
	List()
	{
		Create_pHead();
	}
	List(T* a, size_t size)
	{
		Create_pHead();
		for (int i = 0; i < size; ++i)
			PushBack(a[i]);
	}
	List(const List<T>& l)
	{
		if (l.Empty())
			return;
		PNode pCur = l._pHead->_next;
		while (pCur != l._pHead)
		{
			PushBack(pCur->_data);
			pCur = pCur->_next;
		}
	}
	iterator Begin()
	{
		return iterator(_pHead->_next);
	}
	iterator End()
	{
		return iterator(_pHead);
	}
	List<T>& operator=(const List<T>& l)
	{
		if (!Empty())
			this->~List();
		if (this != &l)   //自拷贝
		{
			PNode pCur = l._pHead->_next;
			while (pCur != l._pHead)
			{
				PushBack(pCur->_data);
				pCur = pCur->_next;
			}
		}
		return *this;
	}

	void PushBack(const T& data)
	{
		//确定尾节点
		PNode lastnode = _pHead->_pre;
		PNode newnode = new Node(data);
		//链接
		lastnode->_next = newnode;
		newnode->_pre = lastnode;
		_pHead->_pre = newnode;
		newnode->_next = _pHead;
	}
	void PopBack()
	{
		if (_pHead->_next == _pHead)  //空链表
			return;
		//最后一个节点的前驱
		PNode tail = _pHead->_pre;
		PNode last_pre = tail->_pre;
		_pHead->_pre = last_pre;
		last_pre->_next = _pHead;

		delete tail;
	}
	void PushFront(const T& data)
	{
		PNode newnode = new Node(data);
		newnode->_next = _pHead->_next;
		newnode->_next->_pre = newnode;
		_pHead->_next = newnode;
		newnode->_pre = _pHead;
	}
	void PopFront()
	{
		if (_pHead->_next == _pHead)  //空链表
			return;

		PNode pDel = _pHead->_next;
		_pHead->_next = pDel->_next;
		_pHead->_next->_pre = _pHead;
		delete pDel;
	}
	PNode Find(const T& data)
	{
		if (!Empty())
		{
			size_t size = Size();
			PNode pCur = _pHead->_next;
			while (pCur != _pHead)
			{
				if (pCur->_data == data)
					return pCur;
				pCur = pCur->_next;
			}
			return NULL;
		}
		return NULL;
	}
	PNode Erase(PNode pos)         //返回删除位置的下一个节点
	{
		PNode ret = pos->_next;
		pos->_pre->_next = pos->_next;
		pos->_next->_pre = pos->_pre;
		delete pos;
		return ret;
	}
	PNode Insert(PNode pos, const T& data)    //在pos之前插入
	{
		PNode newnode = new Node(data);
		//先链接pos->_pre与newnode
		pos->_pre->_next = newnode;
		newnode->_pre = pos->_pre;
		//再链接newnode和pos
		pos->_pre = newnode;
		newnode->_next = pos;

		return newnode;
	}
	bool Empty()
	{
		return _pHead->_next == _pHead;
	}
	size_t Size()
	{
		size_t count = 0;
		PNode pCur = _pHead->_next;
		while (pCur != _pHead)
		{
			pCur = pCur->_next;
			++count;
		}
		return count;
	}
	//清空链表节点
	void Clear()
	{
		PNode pDel = _pHead->_next;
		while (!Empty())
		{
			pDel = _pHead->_next;
			_pHead->_next = pDel->_next;

			delete pDel;
		}
		_pHead->_pre = _pHead;
	}
	~List()
	{
		Clear();
		delete _pHead;
		_pHead = NULL;
	}
protected:
	void Create_pHead()
	{
		_pHead = new Node;
		_pHead->_next = _pHead;
		_pHead->_pre = _pHead;
	}
private:
	PNode _pHead;
};

void List_Test()
{
	List<int> L;
	L.PushBack(1);
	L.PushBack(2);
	L.PushBack(3);
	L.PushBack(4);
	List<int>::iterator it = L.Begin();
	while (it != L.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	L.PopBack();

	L.PushFront(-1);
	L.PushFront(-2);
	L.PopFront();
	L.Insert(L.Find(-1), -3);
	L.Erase(L.Find(1));

	it = L.Begin();
	while (it != L.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}