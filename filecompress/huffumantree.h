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
		, _parent(NULL)              // Ҷ�ӽڵ㵽���ڵ�·��Ψһ---Ҷ�ӽڵ�ı���
		, _weight(weight)
	{}
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _weight;   //Ȩֵ
};
template<class T>
class HuffmanTree                 //Ҷ�ӽڵ㵽��·��Ψһ--�ַ����벻���Ǳ���ַ������ǰ׺
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
		//Ϊ��С����׼��
		struct LessNode
		{
			bool operator()(const PNode l, const PNode r) //Node&
			{
				return l->_weight > r->_weight;
			}
		};
		//�������ȼ�����
		priority_queue<PNode,vector<PNode>,LessNode> minheap;
		for (int i = 0; i <size; ++i)
		{
			if (a[i]!=invalid)
			{
				PNode temp = new Node(a[i]);
				minheap.push(temp);
			}
		}
		//�����������
		while((int)minheap.size()>1)
		{
			//��2��1-----ֱ��ʣ��1����Ϊhuffman���ĸ�
			PNode left = minheap.top();
			minheap.pop();
			PNode right = minheap.top();
			minheap.pop();
			PNode parent = new Node(left->_weight + right->_weight); //�������ڵ� 
			parent->_left = left;   //����left��right��parent
			left->_parent = parent;
			parent->_right = right;
			right->_parent = parent;
			//�Ѹ��ڵ���뵽minheap�ÿ��minheap.Size()�����1.
			minheap.push(parent);
		}
		_Root = minheap.top();  //_Root��Ϊ��������
		minheap.pop();  //��С��Ϊ��
	}
	~HuffmanTree()
	{
		_Destroy(_Root);
		_Root = NULL;
	}
	//�õ����������ĸ���Ϊ�˸��õ�����������ĺ�������
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