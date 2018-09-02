
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
	int _bf;      //ƽ������---���㷽��  �������߶�-�������߶�
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
	//����
	bool Insert(const K& key, const V& value)
	{
		return _Insert(_root, key, value);
	}
	//��������ڵ�key
	void InOrder()
	{
		_InOrder(_root);
	}
	//��������ڵ�ƽ������
	void InOrderbf()
	{
		_InOrderbf(_root);
	}
	//�ж��ǲ���ƽ����
	bool IsBalanceTree()
	{
		/*return _IsBalanceTree1(_root);*/
		return _IsBalanceTree2(_root);
	}
	//���ĸ߶�
	size_t Depth()
	{
		return _Depth(_root);
	}
protected:
	//����
	void rotateL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		
		parent->_right=pSubRL;
		//pSub˫�׵ĸ���һ��Ҫ�ڱ��� ��ԭ��˫��֮�����
		PNode pparent = parent->_parent;
		pSubR->_left = parent;
		parent->_parent = pSubR;
		if (pSubRL)              //pSubRLΪNULL��������ҵ�֧
			pSubRL->_parent = parent;
		if (pparent== NULL)   //parentΪroot
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
		//����ƽ������
		parent->_bf = pSubR->_bf = 0;
	}
	//�ҵ���
	void rotateR(PNode& parent)
	{
		PNode pSubL = parent->_left;
		PNode pSubLR = pSubL->_right;
		//1.0 ����parent�Լ�pSubL pSubLR��ָ����
		parent->_left=pSubLR;
		pSubL->_right = parent;
		if (pSubLR)                 //pSubLR ���ܻ�ΪNULL����� ��֧
		    pSubLR->_parent = parent;
		//parent��˫��һ��Ҫ��parent˫�׸���֮ǰ����
		PNode pparent = parent->_parent;
		parent->_parent = pSubL;
		//2.0 �ж�parent�ǲ���_root�ڵ�
		if (NULL == pparent)   //parentΪroot�ڵ�
		{
			_root = pSubL;
			_root->_parent = NULL;
		}
		else
		{
		//����parent˫�׵�ָ����
			if (pparent->_left == parent)
				pparent->_left = pSubL;
			else
				pparent->_right = pSubL;
			pSubL->_parent = pparent;
		}
		//3.0 ����ƽ������
		pSubL->_bf=parent->_bf = 0;
	}
	//����˫��
	void rotateLR(PNode& parent)
	{
		PNode pSubL = parent->_left;
		PNode pSubLR = pSubL->_right;
		int bf = pSubLR->_bf;           //pSubLR->_bf�жϵı���Ϊԭ���Ķ����ǽ���������˫����pSubLR->_bf
		
		rotateL(parent->_left);
		rotateR(parent);
		//����ƽ������---3�����
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
		else           //pSubLR->_bf==0�����     Insert(7,3,5)
		{
			pSubL->_bf=parent->_bf = 0;
		}
	}
	//����˫��
	void rotateRL(PNode& parent)
	{
		PNode pSubR = parent->_right;
		PNode pSubRL = pSubR->_left;
		int bf = pSubRL->_bf;
		rotateR(parent->_right);
		rotateL(parent);
		//����ƽ������---3�����
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
		//1.0 �Ҳ���λ��
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
			else     //��keyֵ�Ѿ�����
				return false;
		}
		//ѭ������pCurΪNULL parentΪ������ڵ��˫�׽ڵ�
		//2.0 ����ڵ�
		pCur = new Node(key, value);
		if (key > parent->_key)
			parent->_right = pCur;
		else
			parent->_left = pCur;
		//***����pCur��˫�׽ڵ�
		pCur->_parent = parent;

		//3.0 ����ƽ������
		while (parent)           //������pCur  ���ܳ���pCurֻ��ͷparentָ��NULL�����
		{
			if (parent->_left == pCur)
				parent->_bf--;
			else
				parent->_bf++;
			if (parent->_bf == 0)
				return true;
			else if (parent->_bf == 1 || parent->_bf == -1)//���ϲ鿴�Ƿ���Ҫ��ת ������Ҫ�õ�parentָ����
			{                                             //�Լ�ѭ��
				pCur = parent;
				parent = pCur->_parent;
			}
			else   //parent->_bf == 2 || parent->_bf == -2
			{
	            //ͬ�ŵ��� ���˫��
				if (parent->_bf == 2)
				{
					//����--����
					if (pCur->_bf == 1)
						rotateL(parent);
					//����--����˫��
					else
						rotateRL(parent);
				}
				else
				{
					//����--�ҵ���
					if (pCur->_bf == -1)
						rotateR(parent);
					//����--����˫��
					else
						rotateLR(parent);
				}
				//��ת������ѭ��
				break;
			}
		}
		//whileѭ������ �����������������
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
		if (abs(root->_bf)>1)      // root->_bf>1|| root->_bf<-1ʱ�ĳ���
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
		if (abs(distance) > 1&&root->_bf!=distance)        //AVL���ж��Ƿ�Ϊƽ����ע��ڶ�������
			return false;
		return _IsBalanceTree2(root->_left) && _IsBalanceTree2(root->_right);
	}
private:
	PNode _root;
};
void AVL_Test()
{
	//��������
	//AVLTree<int, int> avlt;
	//int arr[] = { 7, 5, 9, 8, 10, 11 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//�ҵ�������
	//AVLTree<int, int> avlt;
	//int arr[] = {10, 8, 11, 7, 9, 6};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//����˫������1��pSubLR->_bf == -1
	//AVLTree<int, int> avlt;
	//int arr[] = {10,11,5,3,9,7};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//����˫������1��pSubLR->_bf == 1
	//AVLTree<int, int> avlt;
	//int arr[] = { 10, 11, 5, 3, 7, 9 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//����˫������1��pSubLR->_bf == 0
	//AVLTree<int, int> avlt;
	//int arr[] = { 7,3,5 };
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//����˫������1��pSubRL->_bf ==-1
	//AVLTree<int, int> avlt;
	//int arr[] = {4,3,7,6,8,5};
	//int size = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < size; ++i)
	//	avlt.Insert(arr[i], i);
	//����˫������1��pSubRL->_bf ==1
	AVLTree<int, int> avlt;
	int arr[] = { 4, 3, 7,8, 5,6};
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
		avlt.Insert(arr[i], i);
	//����˫������1��pSubRL->_bf ==0
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