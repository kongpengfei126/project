
#include<iostream>
using namespace std;

//AVL==heap+BSTree
template<class K,class V>
class AVLTreeNode
{
public:
	AVLTreeNode<K,V>* _left;
	AVLTreeNode<K,V>* _right;
	AVLTreeNode<K,V>* _parent;
	K _key;
	V _value;
	int _bf;      //平衡因子---计算方法  右子树高度-左子树高度
	AVLTreeNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		,_parent(NULL)
		, _key(key)
		, _value(value)
		, _bf(0)
	{}
};
template<class K,class V>
class AVLTree
{
public:
	typedef AVLTreeNode<K, V> Node;
	typedef Node* PNode;
	AVLTree()
		:_root(NULL)
	{}
	//插入
	bool Insert(const K& key, const V& value)
	{
		return _Insert(_root, key, value);
	}
	//中序遍历节点key
	void InOrder()
	{
		_InOrder(_root);
	}
	//中序遍历节点平衡因子
	void InOrderbf()
	{
		_InOrderbf(_root);
	}
	//判断是不是平衡树
	bool IsBalanceTree()
	{
		/*return _IsBalanceTree1(_root);*/
		return _IsBalanceTree2(_root);
	}
	//树的高度
	size_t Depth()
	{
		return _Depth(_root);
	}
protected:
	//左单旋
	void rotateL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		
		parent->_right=pSubRL;
		//pSub双亲的更新一定要在保存 它原来双亲之后进行
		PNode pparent = parent->_parent;
		pSubR->_left = parent;
		parent->_parent = pSubR;
		if (pSubRL)              //pSubRL为NULL的情况：右单支
			pSubRL->_parent = parent;
		if (pparent== NULL)   //parent为root
		{
			_root = pSubR;
			_root->_parent = NULL;
		}
		else
		{
			if (pparent->_left == parent)
				pparent->_left = pSubR;
			else
				pparent->_right = pSubR;
			pSubR->_parent = pparent;
		}
		//更新平衡因子
		parent->_bf = pSubR->_bf = 0;
	}
	//右单旋
	void rotateR(PNode& parent)
	{
		PNode pSubL = parent->_left;
		PNode pSubLR = pSubL->_right;
		//1.0 更改parent以及pSubL pSubLR的指针域
		parent->_left=pSubLR;
		pSubL->_right = parent;
		if (pSubLR)                 //pSubLR 可能会为NULL情况： 左单支
		    pSubLR->_parent = parent;
		//parent额双亲一定要在parent双亲更新之前保存
		PNode pparent = parent->_parent;
		parent->_parent = pSubL;
		//2.0 判断parent是不是_root节点
		if (NULL == pparent)   //parent为root节点
		{
			_root = pSubL;
			_root->_parent = NULL;
		}
		else
		{
		//更新parent双亲的指针域
			if (pparent->_left == parent)
				pparent->_left = pSubL;
			else
				pparent->_right = pSubL;
			pSubL->_parent = pparent;
		}
		//3.0 更新平衡因子
		pSubL->_bf=parent->_bf = 0;
	}
	//左右双旋
	void rotateLR(PNode& parent)
	{
		PNode pSubL = parent->_left;
		PNode pSubLR = pSubL->_right;
		int bf = pSubLR->_bf;           //pSubLR->_bf判断的必须为原来的而不是进行了左右双旋的pSubLR->_bf
		
		rotateL(parent->_left);
		rotateR(parent);
		//更新平衡因子---3种情况
		if (bf == -1)
		{
			parent->_bf = -1;
			pSubL->_bf = pSubLR->_bf = 0;
		}
		else if (bf == 1)
		{
			pSubL->_bf = -1;
			pSubLR->_bf = parent->_bf = 0;
		}
		else           //pSubLR->_bf==0的情况     Insert(7,3,5)
		{
			pSubL->_bf=parent->_bf = 0;
		}
	}
	//右左双旋
	void rotateRL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		int bf = pSubRL->_bf;
		rotateR(parent->_right);
		rotateL(parent);
		//更新平衡因子---3种情况
		if (bf == -1)
		{
			pSubR->_bf = 1;
			pSubRL->_bf=parent->_bf = 0;
		}
		else if (bf == 1)
		{
			parent->_bf = -1;
			pSubR->_bf = pSubRL->_bf = 0;
		}
		else
			pSubR->_bf = parent->_bf = 0;

	}
	bool _Insert(PNode& root, const K& key, const V& value)
	{
		if (NULL == root)
		{
			root = new Node(key, value);
			return true;
		}
		//1.0 找插入位置
		PNode parent = NULL;
		PNode pCur = root;
		while (pCur)
		{
			if (key > pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_right;
			}
			else if (key < pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_left;
			}
			else     //该key值已经存在
				return false;
		}
		//循环结束pCur为NULL parent为待插入节点的双亲节点
		//2.0 插入节点
		pCur = new Node(key, value);
		if (key > parent->_key)
			parent->_right = pCur;
		else
			parent->_left = pCur;
		//***更新pCur的双亲节点
		pCur->_parent = parent;

		//3.0 更新平衡因子
		while (parent)           //不能用pCur  可能出现pCur只想头parent指向NULL的情况
		{
			if (parent->_left == pCur)
				parent->_bf--;
			else
				parent->_bf++;
			if (parent->_bf == 0)
				return true;
			else if (parent->_bf == 1 || parent->_bf == -1)//向上查看是否需要旋转 所以需要用到parent指针域
			{                                             //以及循环
				pCur = parent;
				parent = pCur->_parent;
			}
			else   //parent->_bf == 2 || parent->_bf == -2
			{
	            //同号单旋 异号双旋
				if (parent->_bf == 2)
				{
					//右右--左单旋
					if (pCur->_bf == 1)
						rotateL(parent);
					//右左--右左双旋
					else
						rotateRL(parent);
				}
				else
				{
					//左左--右单旋
					if (pCur->_bf == -1)
						rotateR(parent);
					//左右--左右双旋
					else
						rotateLR(parent);
				}
				//旋转完后结束循环
				break;
			}
		}
		//while循环结束 插入操作结束返回真
		return true;
	}
	void _InOrder(PNode root)
	{
		if (root)
		{
			_InOrder(root->_left);
			cout <<root->_key<< "->";
			_InOrder(root->_right);
		}
	}
	void _InOrderbf(PNode root)
	{
		if (root)
		{
			_InOrderbf(root->_left);
			cout << root->_bf << "->";
			_InOrderbf(root->_right);
		}
	}
	bool _IsBalanceTree1(PNode root)
	{
		if (NULL == root)
			return true;
		if (abs(root->_bf)>1)      // root->_bf>1|| root->_bf<-1时的场景
			return false;
		return _IsBalanceTree1(root->_left) && _IsBalanceTree1(root->_right);
	}
	size_t _Depth(PNode root)
	{
		if (NULL == root)
			return 0;
		size_t leftdepth = _Depth(root->_left);
		size_t rightdepth = _Depth(root->_right);
		return leftdepth > rightdepth ? leftdepth + 1 : rightdepth + 1;
	}
	bool _IsBalanceTree2(PNode root)
	{
		if (NULL == root)
			return true;
		int distance = _Depth(root->_right) - _Depth(root->_left);
		if (abs(distance) > 1&&root->_bf!=distance)        //AVL树判断是否为平衡树注意第二个条件
			return false;
		return _IsBalanceTree2(root->_left) && _IsBalanceTree2(root->_right);
	}
private:
	PNode _root;
};
void AVL_Test()
{
	//左单旋测试
	//AVLTree<int, int> avlt;
	//int arr[] = { 7, 5, 9, 8, 10, 11 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//右单旋测试
	//AVLTree<int, int> avlt;
	//int arr[] = {10, 8, 11, 7, 9, 6};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//左右双旋测试1：pSubLR->_bf == -1
	//AVLTree<int, int> avlt;
	//int arr[] = {10,11,5,3,9,7};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//左右双旋测试1：pSubLR->_bf == 1
	//AVLTree<int, int> avlt;
	//int arr[] = { 10, 11, 5, 3, 7, 9 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//左右双旋测试1：pSubLR->_bf == 0
	//AVLTree<int, int> avlt;
	//int arr[] = { 7,3,5 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//右左双旋测试1：pSubRL->_bf ==-1
	//AVLTree<int, int> avlt;
	//int arr[] = {4,3,7,6,8,5};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//右左双旋测试1：pSubRL->_bf ==1
	AVLTree<int, int> avlt;
	int arr[] = { 4, 3, 7,8, 5,6};
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
		avlt.Insert(arr[i], i);
	//右左双旋测试1：pSubRL->_bf ==0
	//AVLTree<int, int> avlt;
	//int arr[] = {3,7,5};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	avlt.InOrder();
	cout << "NULL" << endl;
	avlt.InOrderbf();
	cout << "NULL" << endl;
	cout<<avlt.IsBalanceTree()<<endl;
	cout << avlt.Depth() << " ";
}