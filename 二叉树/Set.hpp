#include<iostream>
#include"After_RBTree.hpp"
using namespace std;
template<class K>
class Set
{
public:
	typename typedef RBTree<K, K>::iterator  iterator; 
	Set()
		:_t()
	{}
	pair<iterator, bool>Insert(const K& key)
	{
		pair<iterator, bool > it = _t.Insert(make_pair(key, 1));
		if (it.second == false)      //已经存在
			++(*(it.first)).second;
		else
			;
		return it;
	}
	bool Empty() const
	{
		return _t.Empty();
	}
	size_t Size()
	{
		return _t.Size();
	}
	iterator Begin()
	{
		return _t.Begin();
	}
	iterator End()
	{
		return _t.End();
	}
private: 
	RBTree<K, K> _t;
};
void Set_Test()
{
	Set<int> s;
	s.Insert(1);
	s.Insert(7);
	s.Insert(6);
	s.Insert(4);
	s.Insert(6);
	Set<int>::iterator   it = s.Begin();
	while (it != s.End())
	{
		cout << it->first << ":"<<it->second<<" ";         //_value.second用来保存key出现的次数
		++it;
	}
	cout << endl;
}