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
	//节点颜色默认为红色--为黑色的话一定不满足性质3一定要调整  
	RBTreeNode(pair<K,V> value)  //如果插入位置的双亲为黑色 默认节点为红色不用调整
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_value(value.first,value.second)
		,_color(RED)
		{}
};
//性质1：没有两个连着的红色节点 2：根为黑色 3:每条路径上黑色节点数目相等
template<class K, class V>
class RBTree
{
public:
	typedef RBTreeNode<K, V> Node;
	typedef Node* PNode;


	RBTree()
		:_root(NULL)
	{}
	//插入
	bool Insert(pair<K,V>& value)
	{
		return _Insert(_root,value);
	}
	//中序遍历
	void InOrder()
	{
		_InOrder(_root);
	}
	bool Is_RBTree()
	{
		if (_root->_color == RED)
		{
			cout <<"违反了性质2:root点为黑色"<< endl;
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
		return _Is_RBTree(_root,count,i);
	}
	void Set_root_RED()
	{
		_root->_color = RED;
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
		if (NULL == pparent)  //parent为根节点
		{
			_root = pSubR;
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
		if (NULL == pparent)   //parent为根节点
		{
			_root = pSubL;
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
	bool _Insert(PNode& root, pair<K, V>& value)
	{
		if (NULL == root)
		{
			root = new Node(value);
			//性质2 根为黑色
			root->_color = BLACK;
			return true;
		}
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
				return false;
		}
		//循环结束 Pre为待插入节点位置的双亲  pCur为NULL
		pCur = new Node(value);
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
		while (parent&&parent->_color == RED)
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
		root->_color = BLACK;
		return true;
	}
	//递归中序遍历
	void _InOrder(PNode root)
	{
		if (root)
		{
			_InOrder(root->_left);
			cout <<root->_value.first<< "->";
			_InOrder(root->_right);
		}
	}
	//判断一棵树是不是红黑树  不是---红红相连、每条路径上黑色节点数目相等，根节点为和黑色
	bool _Is_RBTree(PNode root,size_t count,size_t i)     //i是为了让下一层的节点知道上层公有多少个黑色节点
	{
		if (NULL == root)
			return true;    //空树也是红黑树
		if (root->_color == BLACK)
			++i;
		PNode parent = root->_parent;
		if (parent&&parent->_color == RED&&root->_color == RED)  //红红相连违反性质1
		{
			cout <<"违反了性质1红红相连"<< endl;
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


