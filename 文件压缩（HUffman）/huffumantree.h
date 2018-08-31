#pragma once
#include<vector>
#include<string>
#include<fstream>
#include<queue>
#include<assert.h>
#include<iostream>
using namespace std;
template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode()
	{}
	HuffmanTreeNode(const T& weight)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)              // 叶子节点到根节点路径唯一---叶子节点的编码
		, _weight(weight)
	{}
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _weight;   //权值
};
template<class T>
class HuffmanTree                 //叶子节点到根路径唯一--字符编码不会是别的字符编码的前缀
{
public:
	typedef HuffmanTreeNode<T> Node;
	typedef Node* PNode;
	HuffmanTree()
		:_Root(NULL)
	{}
	HuffmanTree(T* a, int size, const T& invalid)
	{
		assert(a);
		//为建小堆做准备
		struct LessNode
		{
			bool operator()(const PNode l, const PNode r) //Node&
			{
				return l->_weight > r->_weight;
			}
		};
		//构造优先级队列
		priority_queue<PNode,vector<PNode>,LessNode> minheap;
		for (int i = 0; i <size; ++i)
		{
			if (a[i]!=invalid)
			{
				PNode temp = new Node(a[i]);
				minheap.push(temp);
			}
		}
		//构造哈夫曼树
		while((int)minheap.size()>1)
		{
			//出2进1-----直到剩余1个即为huffman树的根
			PNode left = minheap.top();
			minheap.pop();
			PNode right = minheap.top();
			minheap.pop();
			PNode parent = new Node(left->_weight + right->_weight); //构建父节点 
			parent->_left = left;   //链接left，right，parent
			left->_parent = parent;
			parent->_right = right;
			right->_parent = parent;
			//把父节点插入到minheap里，每次minheap.Size()都会减1.
			minheap.push(parent);
		}
		_Root = minheap.top();  //_Root即为哈弗曼树
		minheap.pop();  //最小堆为空
	}
	~HuffmanTree()
	{
		_Destroy(_Root);
		_Root = NULL;
	}
	//得到哈夫曼树的根是为了给得到哈夫曼编码的函数传参
	PNode GetRoot()
	{
		return _Root;
	}
protected:
	void _Destroy(PNode root)
	{
		if (NULL == root)
			return;
		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
		root = NULL;
	}

private:
		PNode _Root;
};
//void HuffmanTreeTest()
//{
//	int a[5] = { 2, 5, 3, 7, 1 };
//	int size = sizeof(a) / sizeof(a[0]);
//	HuffmanTree<int> ht(a, size, 0);
//}