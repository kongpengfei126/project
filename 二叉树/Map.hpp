#include<iostream>
#include<string>
#include"After_RBTree.hpp"
using namespace std;

template<class K,class V>
class Map
{
public:
	typedef pair<K, V> valuetype;
	typename typedef RBTree<K, V>::iterator iterator;  //���߱�����   ����������
	Map()
		:_t()
	{}
	pair<iterator, bool> Insert(const valuetype& v)
	{
		return _t.Insert(v);
	}
	bool Empty()
	{
		return _t.Empty();
	}
	size_t Size()
	{
		return _t.Size();
	}
	V& operator[](const K& key)
	{
		iterator ret=_t.Insert(pair<K, V>(key, V())).first;
		return ret->second;
	}
	iterator Begin()
	{
		return _t.Begin();
	}
	iterator End()
	{
		return _t.End();
	}
protected:

private:
	RBTree<K, V> _t;
};
void Map_Test()
{
	Map<string, string> m;
	cout<<m.Insert(make_pair("�ļ�", "��")).second;
	cout<<m.Insert(make_pair("���", "��")).second;
	cout<<m.Insert(make_pair("����", "��")).second;
	cout<<m.Insert(make_pair("����", "��")).second;
	cout<<m.Insert(make_pair("�ı�", "ģʽ")).second;

	cout<<m.Insert(make_pair("�ı�", "sd")).second;
	cout << m.Size()<<" ";
	cout << m["�ı�"];
	Map<string, string>::iterator it = m.Begin();
	while (it!=m.End())
	{
		cout << it->first << ":" << it->second << " ";
		++it;
	}

}