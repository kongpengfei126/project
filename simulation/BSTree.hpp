#pragma once
#include<iostream>
using namespace std;
template<class T>
//二叉搜索树节点构造
class BSTreeNode
{
public:
	BSTreeNode<T>* _left;
	BSTreeNode<T>* _right;
	T _val;
	BSTreeNode(const T& val)
		:_left(NULL)
		, _right(NULL)
		, _val(val)
	{}
	BSTreeNode(BSTreeNode<T>& bst)
		:_left(bst._left)
		, _right(bst._right)
		, _val(bst._val)
	{}
};
//二叉树的构造
template<class T>
class BSTree
{
public:
	typedef BSTreeNode<T> Node;
	typedef Node* PNode;
	BSTree()
		:_root(NULL)
	{}
	//二叉搜索树插入元素需要确定位置不能用数组构造
	bool Insert(const T& val)
	{
		/*return _Insert1(_root, val);*/
		return _Insert2(_root, val);
	}
	bool Delete(const T& val)
	{
		/*return _Delete1(_root, val);*/
		return _Delete2(_root, val);
	}
	bool Find(const T& val)
	{
		return _Find1(_root, val);
		return _Find2(_root, val);
	}
	void Order()
	{
		_Order(_root);
	}
	void Clear()
	{
		_Clear(_root);
	}
protected:
	//插入(循环)
	bool _Insert1(PNode& root, const T& val)           //插入的话 头结点可能会为NULL一开始 所以传引用
	{
		if (NULL == root)          //根节点为NULL
		{
			root = new Node(val);
			if (root == NULL)     //new失败
				return false;
			return true;
		}
		else
		{
			//1.0 找插入位置
			PNode pCur = root;
			PNode Pre = NULL;           //最后pCur会走到NULL位置此时需要用其双亲节点Pre来确定其插入位置
			while (pCur)
			{
				if (val > pCur->_val)
				{
					Pre = pCur;
					pCur = pCur->_right;
				}
				else if (val < pCur->_val)
				{
					Pre = pCur;
					pCur = pCur->_left;
				}
				else
					return false;
			}
			//此时Pre为插入位置的双亲节点   插入位置pCur为NULL
			//2.0 插入元素
			pCur = new Node(val);
			if (val < Pre->_val)
				Pre->_left = pCur;
			else
				Pre->_right = pCur;

		}
	}
	//插入(递归)
	bool _Insert2(PNode& root, const T& val)
	{
		if (NULL == root)          //根节点为NULL
		{
			root = new Node(val);
			if (root == NULL)     //new失败
				return false;
			return true;
		}
		else
		{
			if (val < root->_val)
				_Insert2(root->_left, val);
			if (val>root->_val)
				_Insert2(root->_right, val);
			else
				return false;
		}
	}
	//删除(循环)
	bool _Delete1(PNode& root, const T& val)
	{
		PNode pCur =root;
		PNode parent = NULL;
		// 1.0 找到删除位置
		while (pCur)
		{
			if (pCur->_val == val)
				break;
			else if (val > pCur->_val)
			{
				parent = pCur;
				pCur = pCur->_right;
			}
			else
			{
				parent = pCur;
				pCur = pCur->_left;
			}
		}
		if (NULL == pCur)       //没找到 或者是root为NULL
			return false;
		else      //找到了
		{
			// 情况1：无左孩子
			if (NULL == pCur->_left)
			{
				if (pCur == root)
					root = root->_right;
				else
				{
					if (parent->_left == pCur)   //待删除节点是其双亲的左孩子
						parent->_left = pCur->_right;
					else                        //待删除节点是其双亲的右孩子
						parent->_right = pCur->_right;
				}
			}
			//情况2：无右孩子
			else if (NULL == pCur->_right)
			{
				if (pCur == root)
					root = root->_left;
				else
				{
					if (parent->_left == pCur)
						parent->_left = pCur->_left;
					else
						parent->_right = pCur->_left;
				}
			}
			//情况3：左孩子有孩子都存在
			else
			{
				//找待删除节点的右孩子的最左侧孩子
				parent = NULL;
				PNode pDel = pCur;
				pDel = pDel->_right;
				while (pDel->_left)
				{
					parent = pDel;
					pDel = pDel->_left;
				}
				//while结束pDel为为其待删除节点的替代节点 parent为其待删除节点的替代节点的双亲节点
				pCur->_val = pDel->_val;
				if (pCur->_right == pDel)  //pCur的右孩子没有左孩子
					pCur->_right = pDel->_right;
				//else if (pCur->_right == parent)       //pCur的右孩子只有一个左孩子
				//{
				//	parent->_left = pDel->_right;
				//}
				else                       //pCur的右孩子有左孩子
				{
			    	parent->_left = pDel->_right;
				}
				pCur = pDel;
			}
			delete pCur;
			pCur = NULL;
			return true;
		}
	}
	bool _Delete2(PNode& root, const T& val)
	{
		if (NULL == root)
			return false;
		if (root->_val>val)
			return _Delete2(root->_left, val);
		else if (root->_val<val)
			return _Delete2(root->_right, val);
		else      //找到了
		{
			PNode pCur = root;   //pCur定位到待删除元素的位置
			// 情况1：无左孩子
			if (NULL == pCur->_left)
			{
				//待删除的节点指向其右孩子
				root = pCur->_right;
				delete pCur;
				pCur = NULL;
			}
			//情况2：无右孩子
			else if (NULL == pCur->_right)
			{
				//待删除的节点指向其左孩子
				root = pCur->_left;
				delete pCur;
				pCur = NULL;
			}
			//情况3：左孩子有孩子都存在
			else
			{
				//找待删除节点的右孩子的最左侧孩子
				PNode pDel = pCur->_right;
				while (pDel->_left)
				{
					pDel = pDel->_left;
				}
				//while结束pDel为为其待删除节点的替代节点 
				pCur->_val = pDel->_val;
				_Delete2(pCur->_right, pDel->_val);
			}
		}
	}
	//查找(循环)
	bool _Find1(PNode root,const T& val)
	{
		while (root)
		{
			if (root->_val == val)
			{
				return true;
			}
			if (root->_val > val)
				root = root->_left;
			else
				root = root->_right;
		}
		return false;           //遍历完树没找到或者树为NULL
	}
	bool _Find2(PNode root, const T& val)
	{
		if (NULL == root)
			return false;
		if (root->_val == val)
			return true;
		else if (root->_val > val)
			_Find2(root->_left, val);
		else
			_Find2(root->_right, val);
	}
	void _Order(PNode root)
	{
		if (root == NULL)
			return;
		_Order(root->_left);
		cout <<root->_val<< "->";
		_Order(root->_right);
	}
	void _Clear(PNode& root)      //后序删除
	{
		if (root == NULL)
			return;
		_Clear(root->_left);
		_Clear(root->_right);
		delete root;
		root = NULL;
	}
private:
	PNode _root;
};


void BSTreeTest()
{
	BSTree<int> bst;
	int arr[9] = { 1, 3, 9, 2, 7, 4, 5, 8, 6 };
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
	{
		bst.Insert(arr[i]);
	}
	//bst.Delete(7);
	//int arr1[] = { 2, 1, 5, 3, 8, 6, 9, 7 };
	//int size1 = sizeof(arr1) / sizeof(arr1[0]);
	//BSTree<int> bst1;
	//for (int i = 0; i < size1; ++i)
	//{
	//	bst1.Insert(arr1[i]);
	//}
	//bst1.Delete(5);
	bst.Delete(7);
	cout<<bst.Find(7)<<" "<<bst.Find(5)<<" ";
	int arr1[] = { 2, 1, 5, 3, 8, 6, 9, 7 };
	int size1 = sizeof(arr1) / sizeof(arr1[0]);
	BSTree<int> bst1;
	for (int i = 0; i < size1; ++i)
	{
		bst1.Insert(arr1[i]);
	}
	bst1.Delete(5);
	cout << bst1.Find(7) << " " << bst1.Find(5)<<" ";
	bst.Order();
	cout <<"NULL"<<endl;
	bst1.Order();
	cout << "NULL" << endl;
}