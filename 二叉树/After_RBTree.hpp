#pragma once
#include<iostream>
//�ӵ�������ı䣺  1.0 ��������root��Ҫ����  2.0 ѭ�������� _pHead!=parent  
// 3.0 �ж�parentΪroot������ pHead== pparent  �ڲ����� _pHead��_pHead->_parent��root
enum color
{
	RED,
	BLACK
};
using namespace std;
template<class K, class V>
class RBTreeNode
{
public:
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	pair<K, V> _value;
	color _color;
	//�ڵ���ɫĬ��Ϊ��ɫ--Ϊ��ɫ�Ļ�һ������������3һ��Ҫ����  
	RBTreeNode(const K& key=K(),const V& value=V())  //�������λ�õ�˫��Ϊ��ɫ Ĭ�Ͻڵ�Ϊ��ɫ���õ���
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _value(key,value)
		, _color(RED)
	{}
};
template<class K, class V>
class RBTree_iterator
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
	typedef RBTree_iterator<K, V> self;
	RBTree_iterator(PNode node = NULL)
		:_PNode(node)
	{}
	RBTree_iterator(const self& s)   //ǳ����
		:_PNode(s._PNode)
	{}
	pair<K, V>& operator*()
	{
		return _PNode->_value;
	}
	pair<K, V>* operator->()
	{
		return &(operator*());
	}
	self& operator++()
	{
		_Increasement(_PNode);
		return *this;
	}
	self operator++(int)
	{
		self temp(*this);
		_Increasement(_PNode);
		return temp;
	}
	self& operator--()
	{
		_Decreasement(_PNode);
		return *this;
	}
	self operator--(int)
	{
		self temp(*this);
		_Decreasement(_PNode);
		return temp;
	}
	bool operator!=(self& s)
	{ 
		return _PNode != s._PNode;
	}
	bool operator==(const self& s)
	{
		return _PNode == s._PNode;
	}
protected:
	//��һ���ڵ�++
	void _Increasement(PNode& _PNode)
	{
		if (_PNode->_right)
		{
			_PNode = _PNode->_right;
			while (_PNode->_left)
			{
				_PNode = _PNode->_left;
			}
		}
		else
		{
			PNode parent = _PNode->_parent;
			while (parent&&parent->_right == _PNode)
			{
				_PNode = parent;
				parent = _PNode->_parent;
			}
			//if (parent&&parent->_left == _PNode)
			//	_PNode = parent;
			//if (parent == NULL)   //��ʱ_PNodeΪ���ڵ�  
			//	_PNode = NULL;

			// ����  ���ĸ��ڵ�û���Һ���  �ҵ�������ʼλ���ڸ��ڵ�
			if (parent&&_PNode!=parent->_right)
				_PNode = parent;
		}
	}
	//��һ���ڵ� --
	void _Decreasement(PNode& _PNode)
	{
		//��������endλ��
		if (_PNode->_parent->_parent == _PNode&&_PNode->_color == RED)
		{
			_PNode = _PNode->_right;
		}
		else if (_PNode->_left)
		{
			_PNode = _PNode->_left;
			while (_PNode->_right)
				_PNode = _PNode->_right;
		}
		else
		{
			PNode parent = _PNode->_parent;
			while (parent&&parent->_left == _PNode)
			{
				_PNode = parent;
				parent = _PNode->_parent;
			}
			//if (parent&&parent->_right == _PNode)
			//	_PNode = parent;
			//else      //parentΪNULL
			//	_PNode = parent;
			//begin()����--
			_PNode = parent;
		}
	}
private:
	PNode _PNode;
};
//����1��û���������ŵĺ�ɫ�ڵ� 2����Ϊ��ɫ 3:ÿ��·���Ϻ�ɫ�ڵ���Ŀ���
template<class K, class V>
class RBTree
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;
	typedef RBTree_iterator<K,V> iterator;


	iterator Begin()
	{
		return iterator(_pHead->_left);
	}
	iterator End()
	{
		return iterator(_pHead);
	}

	RBTree()
		:_pHead(new Node)
	{}
	//����
	pair<iterator,bool> Insert(const pair<K, V>& value)
	{
		return _Insert(GetRoot(), value);
	}
	//�������
	void InOrder()
	{
		_InOrder(GetRoot());
		cout <<"NULL" ;
	}
	bool Is_RBTree()
	{
		PNode& _root = GetRoot();
		if (_root->_color == RED)
		{
			cout << "Υ��������2:root��Ϊ��ɫ" << endl;
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
		return _Is_RBTree(_root, count, i);
	}
	void Set_root_RED()
	{
		PNode& root = GetRoot();
		root->_color = RED;
	}
	PNode& GetRoot()
	{
		return _pHead->_parent;
	}
	bool Empty() const
	{
		return  NULL == GetRoot();
	}
	size_t Size() const
	{
		size_t count = 0;
		iterator it = iterator(_pHead->_left);
		iterator end = iterator(_pHead);
		while (it != end)
		{
			++count;
			++it;
		}
		return count;
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
		if (_pHead== pparent)  //parentΪ���ڵ�
		{
			_pHead->_parent= pSubR;
			pSubR->_parent = _pHead;
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
		if (pparent==_pHead)   //parentΪ���ڵ�
		{
			_pHead->_parent = pSubL;
			pSubL->_parent = _pHead;
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
	pair<iterator, bool> _Insert(PNode& root, const pair<K, V>& value)
	{
		PNode newnode = NULL;
		if (NULL == root)
		{
			newnode=root = new Node(value.first,value.second);
			//����2 ��Ϊ��ɫ
			root->_color = BLACK;
			root->_parent = _pHead;
		}
		else
		{
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
					return pair<iterator, bool>(iterator(pCur),false);
			}
			//ѭ������ PreΪ������ڵ�λ�õ�˫��  pCurΪNULL

			newnode=pCur = new Node(value.first,value.second);
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
			while (_pHead!=parent&&parent&&parent->_color == RED)    //��ֹ���ϸ���ʱ��ﵽ_pHead��������
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
		}//else
		root = GetRoot();            //��ת֮���root�ڵ�����Ѿ��ı䲢����ԭ���ĸ�
	    root->_color = BLACK;
		//�޸�_pHead������ָ����
		_pHead->_left =MostLeft() ;
		_pHead->_right = MostRight();
		return make_pair(iterator(newnode),true);        //��/pCur
	}
	//�ݹ��������
	void _InOrder(PNode root)
	{
		if (root)
		{
			_InOrder(root->_left);
			cout << root->_value.first << "->";
			_InOrder(root->_right);
		}
	}
	//�ж�һ�����ǲ��Ǻ����  ����---���������ÿ��·���Ϻ�ɫ�ڵ���Ŀ��ȣ����ڵ�Ϊ�ͺ�ɫ
	bool _Is_RBTree(PNode root, size_t count, size_t i)     //i��Ϊ������һ��Ľڵ�֪���ϲ㹫�ж��ٸ���ɫ�ڵ�
	{
		if (NULL == root)
			return true;    //����Ҳ�Ǻ����
		if (root->_color == BLACK)
			++i;
		PNode parent = root->_parent;
		if (parent&&parent->_color == RED&&root->_color == RED)  //�������Υ������1
		{
			cout << "Υ��������1�������" << endl;
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
		return _Is_RBTree(root->_left, count, i) && _Is_RBTree(root->_right, count, i);
	}
	PNode MostLeft()
	{
		PNode pCur = GetRoot();
		if (pCur==NULL)
			return NULL;
		while (pCur->_left)
			pCur = pCur->_left;
		return pCur;
	}
	PNode MostRight()
	{
		PNode pCur = GetRoot();
		if (pCur==NULL)
			return NULL;
		while (pCur->_right)
			pCur = pCur->_right;
		return pCur;
	}
private:
	PNode _pHead;
};

void After_RBTree_Test()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	int size = sizeof(a) / sizeof(a[0]);
	RBTree<int, int> rbt;
	for (int i = 0; i < size; ++i)
		rbt.Insert(pair<int, int>(a[i], i));
	rbt.InOrder();
	cout << endl;
	//�����ӡ
	RBTree<int, int>::iterator it = rbt.Begin();
	while (it != rbt.End())
	{
		cout << it->first<<"->";
		++it;
	}
	cout<<"NULL"<< endl;
	//�����ӡ
	it = rbt.End();
	--it;
	while (it != rbt.Begin())
	{
		cout << it->first << "->";
		--it;
	}
	cout <<it->first<<"->NULL" << endl;

	cout<<rbt.Is_RBTree();
	cout << endl;
	//rbt.Set_root_RED();
	//cout << rbt.Is_RBTree();
}


