#pragma once
#include<iostream>
using namespace std;
template<class T>
//�����������ڵ㹹��
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
//�������Ĺ���
template<class T>
class BSTree
{
public:
	typedef BSTreeNode<T> Node;
	typedef Node* PNode;
	BSTree()
		:_root(NULL)
	{}
	//��������������Ԫ����Ҫȷ��λ�ò��������鹹��
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
	//����(ѭ��)
	bool _Insert1(PNode& root, const T& val)           //����Ļ� ͷ�����ܻ�ΪNULLһ��ʼ ���Դ�����
	{
		if (NULL == root)          //���ڵ�ΪNULL
		{
			root = new Node(val);
			if (root == NULL)     //newʧ��
				return false;
			return true;
		}
		else
		{
			//1.0 �Ҳ���λ��
			PNode pCur = root;
			PNode Pre = NULL;           //���pCur���ߵ�NULLλ�ô�ʱ��Ҫ����˫�׽ڵ�Pre��ȷ�������λ��
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
			//��ʱPreΪ����λ�õ�˫�׽ڵ�   ����λ��pCurΪNULL
			//2.0 ����Ԫ��
			pCur = new Node(val);
			if (val < Pre->_val)
				Pre->_left = pCur;
			else
				Pre->_right = pCur;

		}
	}
	//����(�ݹ�)
	bool _Insert2(PNode& root, const T& val)
	{
		if (NULL == root)          //���ڵ�ΪNULL
		{
			root = new Node(val);
			if (root == NULL)     //newʧ��
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
	//ɾ��(ѭ��)
	bool _Delete1(PNode& root, const T& val)
	{
		PNode pCur =root;
		PNode parent = NULL;
		// 1.0 �ҵ�ɾ��λ��
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
		if (NULL == pCur)       //û�ҵ� ������rootΪNULL
			return false;
		else      //�ҵ���
		{
			// ���1��������
			if (NULL == pCur->_left)
			{
				if (pCur == root)
					root = root->_right;
				else
				{
					if (parent->_left == pCur)   //��ɾ���ڵ�����˫�׵�����
						parent->_left = pCur->_right;
					else                        //��ɾ���ڵ�����˫�׵��Һ���
						parent->_right = pCur->_right;
				}
			}
			//���2�����Һ���
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
			//���3�������к��Ӷ�����
			else
			{
				//�Ҵ�ɾ���ڵ���Һ��ӵ�����ຢ��
				parent = NULL;
				PNode pDel = pCur;
				pDel = pDel->_right;
				while (pDel->_left)
				{
					parent = pDel;
					pDel = pDel->_left;
				}
				//while����pDelΪΪ���ɾ���ڵ������ڵ� parentΪ���ɾ���ڵ������ڵ��˫�׽ڵ�
				pCur->_val = pDel->_val;
				if (pCur->_right == pDel)  //pCur���Һ���û������
					pCur->_right = pDel->_right;
				//else if (pCur->_right == parent)       //pCur���Һ���ֻ��һ������
				//{
				//	parent->_left = pDel->_right;
				//}
				else                       //pCur���Һ���������
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
		else      //�ҵ���
		{
			PNode pCur = root;   //pCur��λ����ɾ��Ԫ�ص�λ��
			// ���1��������
			if (NULL == pCur->_left)
			{
				//��ɾ���Ľڵ�ָ�����Һ���
				root = pCur->_right;
				delete pCur;
				pCur = NULL;
			}
			//���2�����Һ���
			else if (NULL == pCur->_right)
			{
				//��ɾ���Ľڵ�ָ��������
				root = pCur->_left;
				delete pCur;
				pCur = NULL;
			}
			//���3�������к��Ӷ�����
			else
			{
				//�Ҵ�ɾ���ڵ���Һ��ӵ�����ຢ��
				PNode pDel = pCur->_right;
				while (pDel->_left)
				{
					pDel = pDel->_left;
				}
				//while����pDelΪΪ���ɾ���ڵ������ڵ� 
				pCur->_val = pDel->_val;
				_Delete2(pCur->_right, pDel->_val);
			}
		}
	}
	//����(ѭ��)
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
		return false;           //��������û�ҵ�������ΪNULL
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
	void _Clear(PNode& root)      //����ɾ��
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