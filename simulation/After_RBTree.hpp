#pragma once
#include<iostream>
//加迭代器后改变：  1.0 调整完后的root需要重置  2.0 循环条件加 _pHead!=parent  
// 3.0 判断parent为root的条件 pHead== pparent  内部链接 _pHead和_pHead->_parent即root
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
	//节点颜色默认为红色--为黑色的话一定不满足性质3一定要调整  
	RBTreeNode(const K& key=K(),const V& value=V())  //如果插入位置的双亲为黑色 默认节点为红色不用调整
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
	RBTree_iterator(const self& s)   //浅拷贝
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
	//下一个节点++
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
			//if (parent == NULL)   //此时_PNode为根节点  
			//	_PNode = NULL;

			// 避免  树的根节点没有右孩子  且迭代器起始位置在根节点
			if (parent&&_PNode!=parent->_right)
				_PNode = parent;
		}
	}
	//上一个节点 --
	void _Decreasement(PNode& _PNode)
	{
		//迭代器在end位置
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
			//else      //parent为NULL
			//	_PNode = parent;
			//begin()不能--
			_PNode = parent;
		}
	}
private:
	PNode _PNode;
};
//性质1：没有两个连着的红色节点 2：根为黑色 3:每条路径上黑色节点数目相等
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
	//插入
	pair<iterator,bool> Insert(const pair<K, V>& value)
	{
		return _Insert(GetRoot(), value);
	}
	//中序遍历
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
			cout << "违反了性质2:root点为黑色" << endl;
			return false;
		}
		//1.0 统计单条路径上黑色节点的数目
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
	//左单旋
	void rotateL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		//1.0 链接节点
		parent->_right = pSubRL;
		PNode pparent = parent->_parent;     //parent的双亲更新之前一定要把以前的双亲保存起来
		pSubR->_left = parent;
		parent->_parent = pSubR;

		pSubR->_parent = pparent;
		if (pSubRL)            //pSubRL为NULL的情况--右单支
			pSubRL->_parent = parent;
		//2.0 判断parent是不是根节点
		if (_pHead== pparent)  //parent为根节点
		{
			_pHead->_parent= pSubR;
			pSubR->_parent = _pHead;
		}
		//3.0 链接pparent和pSubR
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
		//1.0 链接
		parent->_left = pSubLR;
		pSubL->_right = parent;
		PNode pparent = parent->_parent;
		parent->_parent = pSubL;

		pSubL->_parent = pparent;
		if (pSubLR)            //pSubLR为NULL的情况：左单支
			pSubLR->_parent = parent;
		//2.0 判断parent是不是根节点
		if (pparent==_pHead)   //parent为根节点
		{
			_pHead->_parent = pSubL;
			pSubL->_parent = _pHead;
		}
		//3.0链接pparent和pSubL
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
			//性质2 根为黑色
			root->_color = BLACK;
			root->_parent = _pHead;
		}
		else
		{
			//1.0 寻找插入位置	
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
				else     //该key值已经存在 插入失败
					return pair<iterator, bool>(iterator(pCur),false);
			}
			//循环结束 Pre为待插入节点位置的双亲  pCur为NULL

			newnode=pCur = new Node(value.first,value.second);
			//2.0 插入元素
			if (parent->_value.first >value.first)
			{
				parent->_left = pCur;
			}
			else
			{
				parent->_right = pCur;
			}
			pCur->_parent = parent;
			//3.0更新节点颜色
			if (parent->_color == BLACK)  //情况2：插入位置的双亲为黑色  不用调整
				;
			while (_pHead!=parent&&parent&&parent->_color == RED)    //防止向上更新时候达到_pHead继续调整
			{
				PNode grandparent = parent->_parent;  //grandparent颜色一定为黑色
				if (grandparent->_left == parent)
				{
					PNode uncle = grandparent->_right;
					if (uncle&&uncle->_color == RED) //情况3：uncle节点存在且为红色
					{
						grandparent->_color = RED;
						parent->_color = BLACK;
						uncle->_color = BLACK;
						//grandparent的_parent 的颜色可能为红色需要向上更新pCur和parent
						pCur = grandparent;             //注意此时的pCur要求是红色
						parent = pCur->_parent;
						//可能会造成根节点颜色变为红色在外循环一律把_root置为黑色
					}
					else//情况3：uncle存在且为黑色或者uncle不存在
					{
						//情况5：在情况四的基础上 pCur为parent的右孩子
						if (parent->_right == pCur)
						{
							rotateL(parent);
							//交换pCur和parent指针变为情况4
							swap(pCur, parent);
						}
						//情况4情况5的交集
						//右单旋--parent和grandparent交换颜色
						rotateR(grandparent);
						swap(parent->_color, grandparent->_color);
					}
				}
				else           //grandparent->_right==parent 对称于上述代码
				{
					PNode uncle = grandparent->_left;
					if (uncle&&uncle->_color == RED)   //情况2：uncle节点存在且为RED
					{
						grandparent->_color = RED;
						parent->_color = BLACK;
						uncle->_color = BLACK;
						pCur = grandparent;
						parent = pCur->_parent;
					}
					else       //情况3： uncle不存在或者 uncle为黑色
					{
						if (parent->_left == pCur)  //情况5: 情况4的基础上 pCur为parent的左孩子
						{
							rotateR(parent);
							swap(pCur, parent);    //交换pCur和parent变为情况4
						}
						//情况4情况5的交集
						rotateL(grandparent);
						swap(parent->_color, grandparent->_color);
					} //else
				}//else
			}//while
		}//else
		root = GetRoot();            //旋转之后的root节点可能已经改变并不是原来的根
	    root->_color = BLACK;
		//修改_pHead的左右指针域
		_pHead->_left =MostLeft() ;
		_pHead->_right = MostRight();
		return make_pair(iterator(newnode),true);        //根/pCur
	}
	//递归中序遍历
	void _InOrder(PNode root)
	{
		if (root)
		{
			_InOrder(root->_left);
			cout << root->_value.first << "->";
			_InOrder(root->_right);
		}
	}
	//判断一棵树是不是红黑树  不是---红红相连、每条路径上黑色节点数目相等，根节点为和黑色
	bool _Is_RBTree(PNode root, size_t count, size_t i)     //i是为了让下一层的节点知道上层公有多少个黑色节点
	{
		if (NULL == root)
			return true;    //空树也是红黑树
		if (root->_color == BLACK)
			++i;
		PNode parent = root->_parent;
		if (parent&&parent->_color == RED&&root->_color == RED)  //红红相连违反性质1
		{
			cout << "违反了性质1红红相连" << endl;
			return false;
		}
		if (root->_left == NULL&&root->_right == NULL)  //叶子节点
		{
			//查看统计的黑色节点数目是不是等于上面确定的单条路径的黑色节点数目
			if (i == count)
				return true;
			else
			{
				cout << "违反了性质3:单条路径上黑色节点数目相等" << endl;
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
	//正向打印
	RBTree<int, int>::iterator it = rbt.Begin();
	while (it != rbt.End())
	{
		cout << it->first<<"->";
		++it;
	}
	cout<<"NULL"<< endl;
	//反向打印
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


