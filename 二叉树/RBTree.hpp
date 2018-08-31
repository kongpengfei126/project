#pragma once
#include<iostream>
enum color
{
	RED,
	BLACK
};
using namespace std;
template<class K,class V>
class RBTreeNode
{
public:
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	pair<K, V> _value;
	color _color;
	//�ڵ���ɫĬ��Ϊ��ɫ--Ϊ��ɫ�Ļ�һ������������3һ��Ҫ����  
	RBTreeNode(pair<K,V> value)  //�������λ�õ�˫��Ϊ��ɫ Ĭ�Ͻڵ�Ϊ��ɫ���õ���
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_value(value.first,value.second)
		,_color(RED)
		{}
};
//����1��û���������ŵĺ�ɫ�ڵ� 2����Ϊ��ɫ 3:ÿ��·���Ϻ�ɫ�ڵ���Ŀ���
template<class K, class V>
class RBTree
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;


	RBTree()
		:_root(NULL)
	{}
	//����
	bool Insert(pair<K,V>& value)
	{
		return _Insert(_root,value);
	}
	//�������
	void InOrder()
	{
		_InOrder(_root);
	}
	bool Is_RBTree()
	{
		if (_root->_color == RED)
		{
			cout <<"Υ��������2:root��Ϊ��ɫ"<< endl;
			return false;
		}
		//1.0 ͳ�Ƶ���·���Ϻ�ɫ�ڵ����Ŀ
		PNode pCur = _root;
		size_t count = 0, i = 0;
		while (pCur->_left)
		{
			if (pCur->_color == BLACK)
				++count;
			else
				;
			pCur = pCur->_left;
		}
		return _Is_RBTree(_root,count,i);
	}
	void Set_root_RED()
	{
		_root->_color = RED;
	}
protected:
	//����
	void rotateL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		//1.0 ���ӽڵ�
		parent->_right = pSubRL;
		PNode pparent = parent->_parent;     //parent��˫�׸���֮ǰһ��Ҫ����ǰ��˫�ױ�������
		pSubR->_left = parent;
		parent->_parent = pSubR;

		pSubR->_parent = pparent;
		if (pSubRL)            //pSubRLΪNULL�����--�ҵ�֧
			pSubRL->_parent = parent;
		//2.0 �ж�parent�ǲ��Ǹ��ڵ�
		if (NULL == pparent)  //parentΪ���ڵ�
		{
			_root = pSubR;
		}
		//3.0 ����pparent��pSubR
		else
		{
			if (pparent->_left == parent)
				pparent->_left = pSubR;
			else
				pparent->_right = pSubR;
			pSubR->_parent = pparent;
		}
	}
	void rotateR(PNode& parent)
	{
		PNode pSubL = parent->_left;
		PNode pSubLR = pSubL->_right;
	    //1.0 ����
		parent->_left = pSubLR;
		pSubL->_right = parent;
		PNode pparent = parent->_parent;
		parent->_parent = pSubL;

		pSubL->_parent = pparent;
		if (pSubLR)            //pSubLRΪNULL���������֧
		pSubLR->_parent = parent;
		//2.0 �ж�parent�ǲ��Ǹ��ڵ�
		if (NULL == pparent)   //parentΪ���ڵ�
		{
			_root = pSubL;
		}
		//3.0����pparent��pSubL
		else
		{
			if (pparent->_left == parent)
				pparent->_left = pSubL;
			else
				pparent->_right = pSubL;
			pSubL->_parent = pparent;
		}
	}
	bool _Insert(PNode& root, pair<K, V>& value)
	{
		if (NULL == root)
		{
			root = new Node(value);
			//����2 ��Ϊ��ɫ
			root->_color = BLACK;
			return true;
		}
        //1.0 Ѱ�Ҳ���λ��	
		PNode pCur = root;
		PNode parent = NULL;
		while (pCur)
		{
			if (pCur->_value.first >value.first)
			{
				parent = pCur;
				pCur = pCur->_left;
			}
			else if (pCur->_value.first <value.first)
			{
				parent = pCur;
				pCur = pCur->_right;
			}
			else     //��keyֵ�Ѿ����� ����ʧ��
				return false;
		}
		//ѭ������ PreΪ������ڵ�λ�õ�˫��  pCurΪNULL
		pCur = new Node(value);
		//2.0 ����Ԫ��
		if (parent->_value.first >value.first)
		{
			parent->_left = pCur;
		}
		else
		{
			parent->_right = pCur;
		}
		pCur->_parent = parent;
		//3.0���½ڵ���ɫ
		if (parent->_color == BLACK)  //���2������λ�õ�˫��Ϊ��ɫ  ���õ���
			;
		while (parent&&parent->_color == RED)
		{
			PNode grandparent = parent->_parent;  //grandparent��ɫһ��Ϊ��ɫ
			if (grandparent->_left == parent)
			{
				PNode uncle = grandparent->_right;
				if (uncle&&uncle->_color == RED) //���3��uncle�ڵ������Ϊ��ɫ
				{
					grandparent->_color = RED;
					parent->_color = BLACK;
					uncle->_color = BLACK;
					//grandparent��_parent ����ɫ����Ϊ��ɫ��Ҫ���ϸ���pCur��parent
					pCur = grandparent;             //ע���ʱ��pCurҪ���Ǻ�ɫ
					parent = pCur->_parent;
					//���ܻ���ɸ��ڵ���ɫ��Ϊ��ɫ����ѭ��һ�ɰ�_root��Ϊ��ɫ
				}
				else//���3��uncle������Ϊ��ɫ����uncle������
				{
					//���5��������ĵĻ����� pCurΪparent���Һ���
					if (parent->_right == pCur)
					{
						rotateL(parent);
						//����pCur��parentָ���Ϊ���4
						swap(pCur, parent);
					}
					//���4���5�Ľ���
					//�ҵ���--parent��grandparent������ɫ
					rotateR(grandparent);
				    swap(parent->_color, grandparent->_color);
				}
			}
			else           //grandparent->_right==parent �Գ�����������
			{
				PNode uncle = grandparent->_left;
				if (uncle&&uncle->_color == RED)   //���2��uncle�ڵ������ΪRED
				{
					grandparent->_color = RED;
					parent->_color = BLACK;
					uncle->_color = BLACK;
					pCur = grandparent;
					parent = pCur->_parent;
				}
				else       //���3�� uncle�����ڻ��� uncleΪ��ɫ
				{
					if (parent->_left == pCur)  //���5: ���4�Ļ����� pCurΪparent������
					{
						rotateR(parent);
						swap(pCur, parent);    //����pCur��parent��Ϊ���4
					}
					//���4���5�Ľ���
					rotateL(grandparent);
					swap(parent->_color, grandparent->_color);
				} //else
			}//else
		}//while
		root->_color = BLACK;
		return true;
	}
	//�ݹ��������
	void _InOrder(PNode root)
	{
		if (root)
		{
			_InOrder(root->_left);
			cout <<root->_value.first<< "->";
			_InOrder(root->_right);
		}
	}
	//�ж�һ�����ǲ��Ǻ����  ����---���������ÿ��·���Ϻ�ɫ�ڵ���Ŀ��ȣ����ڵ�Ϊ�ͺ�ɫ
	bool _Is_RBTree(PNode root,size_t count,size_t i)     //i��Ϊ������һ��Ľڵ�֪���ϲ㹫�ж��ٸ���ɫ�ڵ�
	{
		if (NULL == root)
			return true;    //����Ҳ�Ǻ����
		if (root->_color == BLACK)
			++i;
		PNode parent = root->_parent;
		if (parent&&parent->_color == RED&&root->_color == RED)  //�������Υ������1
		{
			cout <<"Υ��������1�������"<< endl;
			return false;
		}
		if (root->_left == NULL&&root->_right == NULL)  //Ҷ�ӽڵ�
		{
			//�鿴ͳ�Ƶĺ�ɫ�ڵ���Ŀ�ǲ��ǵ�������ȷ���ĵ���·���ĺ�ɫ�ڵ���Ŀ
			if (i == count)
				return true;
			else
			{
				cout << "Υ��������3:����·���Ϻ�ɫ�ڵ���Ŀ���" << endl;
				return false;
			}
		}
		return _Is_RBTree(root->_left,count,i) && _Is_RBTree(root->_right,count,i);
	}
private:
	PNode _root;
};

void RBTree_Test()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	int size = sizeof(a) / sizeof(a[0]);
	RBTree<int, int> rbt;
	for (int i = 0; i < size; ++i)
	{
		rbt.Insert(pair<int, int>(a[i], i));
	}
	rbt.InOrder();
	cout <<"NULL"<< endl;
	cout<<rbt.Is_RBTree();
	cout << endl;
	//rbt.Set_root_RED();
	//cout << rbt.Is_RBTree();
}


