#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<queue>
#pragma once
using namespace std;

//��������topk������
//ˮ������Ƶ������ǰk��
//1.0 vector�洢���е�ˮ��
struct compare
{
	bool operator()(map<string, int>::iterator _left, map<string, int>::iterator _right)
	{
		return _left->second < _right->second;
	}
};
void GetFavoriteFruit(vector<string>& fruit, size_t k)
{
	//����º���--�Ƚ���
	//2.0 mapͳ��ˮ����Ƶ��
	map<string, int> m;
	int size = fruit.size();
	for (int i = 0; i < size; ++i)
	{
		m[fruit[i]]++;                  //map[a]    --�������Ϊ��һ������a ��Ӧ�ĵڶ�������
		//pair<map<string, int>::iterator,bool > ret;
		//ret = m.insert(make_pair(fruit[i], 1));           //����������ֵ��
		//if (ret.second == false)
		//	++ret.first->second;
	}
	//3.0 �����ȼ����� ����С��
	priority_queue<map<string, int>::iterator, vector<map<string, int>::iterator>, compare> _pq;
	map<string, int>::iterator it = m.begin();
	while (it != m.end())
	{
		_pq.push(it);          //push����ȡ���ڹ��캯���ĵ�һ������
		++it;
	}
	//4.0 ��ӡ���ִ�������ǰk��ˮ��
	while (k--)
	{
		cout << _pq.top()->first << "-" << _pq.top()->second << "  ";
		_pq.pop();
	}
}
void Topk_Test()
{
	vector<string> V;
	V.push_back("ƻ��");
	V.push_back("�㽶");
	V.push_back("����");
	V.push_back("����");
	V.push_back("���ܹ�");
	V.push_back("����");
	V.push_back("����");
	V.push_back("������");
	V.push_back("����");
	V.push_back("�㽶");
	V.push_back("����");
	V.push_back("����");
	V.push_back("����");
	V.push_back("�㽶");
	V.push_back("�㽶");
	GetFavoriteFruit(V, 5);
}