#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<queue>
#pragma once
using namespace std;

//海量数据topk的问题
//水果出现频度最大的前k个
//1.0 vector存储所有的水果
struct compare
{
	bool operator()(map<string, int>::iterator _left, map<string, int>::iterator _right)
	{
		return _left->second < _right->second;
	}
};
void GetFavoriteFruit(vector<string>& fruit, size_t k)
{
	//定义仿函数--比较器
	//2.0 map统计水果的频度
	map<string, int> m;
	int size = fruit.size();
	for (int i = 0; i < size; ++i)
	{
		m[fruit[i]]++;                  //map[a]    --结果返回为第一个参数a 对应的第二个参数
		//pair<map<string, int>::iterator,bool > ret;
		//ret = m.insert(make_pair(fruit[i], 1));           //构造匿名键值对
		//if (ret.second == false)
		//	++ret.first->second;
	}
	//3.0 用优先级队列 建立小堆
	priority_queue<map<string, int>::iterator, vector<map<string, int>::iterator>, compare> _pq;
	map<string, int>::iterator it = m.begin();
	while (it != m.end())
	{
		_pq.push(it);          //push内容取决于构造函数的第一个参数
		++it;
	}
	//4.0 打印出现次数最多的前k种水果
	while (k--)
	{
		cout << _pq.top()->first << "-" << _pq.top()->second << "  ";
		_pq.pop();
	}
}
void Topk_Test()
{
	vector<string> V;
	V.push_back("苹果");
	V.push_back("香蕉");
	V.push_back("西瓜");
	V.push_back("葡萄");
	V.push_back("哈密瓜");
	V.push_back("菠萝");
	V.push_back("橘子");
	V.push_back("火龙果");
	V.push_back("橙子");
	V.push_back("香蕉");
	V.push_back("葡萄");
	V.push_back("橘子");
	V.push_back("橘子");
	V.push_back("香蕉");
	V.push_back("香蕉");
	GetFavoriteFruit(V, 5);
}