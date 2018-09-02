#pragma once
#include<iostream>
#include<queue>
#include<stack>
using namespace std;
template<class T>
class TreeNode
{
public:
	TreeNode(T data)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _val(data)
		, Ltag(0)
		, Rtag(0)
		{}
	//TreeNode<T>* _parent;
	TreeNode<T>* _left;
	TreeNode<T>* _right;
	TreeNode<T>* _parent;
	bool Ltag;          // 1Ϊ���ӵ�ǰ���ͺ��  0��Ӧ �����Һ���
	bool Rtag;
	T _val;
};
template<class T>
class BTree
{
public:
	typedef TreeNode<T> Node;
	typedef Node* PNode;
	BTree()
		:_pHead(NULL)
	{}
	//���鹹�������
	BTree(T* array, size_t size, T invalid)
	{
		size_t index = 0;
		PNode parent = NULL;
		CreateBTree(_pHead,array, size, index, invalid,parent);
	}
   //�������죨����� 
	BTree(const BTree& bt)
	{
		_pHead = copyBTree(bt._pHead);
	}
	//��ֵ���������
	BTree& operator=(const BTree& bt)
	{
		if (this != &bt)     //�����Կ���
		{
			//1.0 �����Լ�
			this->~BTree();
			//2.0 ������������캯��
			this->_pHead=this->copyBTree(bt._pHead);
		}
		return *this;
	}
	//�ݹ����
	void PreOrder1()  
	{ 
		cout << "�ݹ��������:";
		_PreOrder1(_pHead);
		cout << "NULL" << endl;
	}
	void InOrder1()
	{
		cout << "�ݹ��������:";
		_InOrder1(_pHead);
		cout <<"NULL"<<endl;
	}
	void PostOrder1()
	{
		cout << "�ݹ�������:";
		_PostOrder1(_pHead);
		cout << "NULL" << endl;
	}
	void LevelOrder1()
	{
		cout << "�ݹ�������:";
		_LevelOrder1(_pHead);
		cout << "NULL" << endl;
	}
	//�ǵݹ����
	void PreOrder2()
	{
		cout << "�ǵݹ��������:";
		_PreOrder2(_pHead);
	}
	void InOrder2()
	{
		cout << "�ǵݹ��������:";
		_InOrder2(_pHead);
	}
	void PostOrder2()
	{
		cout << "�ǵݹ��������:";
		_PostOrder2(_pHead);
	}
	void LevelOrder2()
	{
		cout << "�ǵݹ��������:";
		_LevelOrder2(_pHead);

	}
	//�������
	size_t Depth()
	{
		return _Depth(_pHead);
	}
	bool Is_completeBTree()
	{
		cout <<"�ǲ�����ȫ������" ;
		return Is_completeBTree(_pHead);
	}
	size_t GetLevelKNodeNum(size_t k)
	{
		cout << "��"<<k<<"��ڵ����:";
		return _GetLevelKNodeNum(_pHead,k);
	}
	size_t NodeSize()
	{
		/*return NodeSize1(_pHead);*/  //��ʽ1
		return NodeSize2(_pHead);      //��ʽ2
	}
	size_t LeafSize()
	{
		cout << "Ҷ�ӽڵ����:";
		return LeafSize(_pHead);
	}
	PNode Find_data_isK(T& data)
	{
		cout << "����ֵΪ"<<data<<"�Ľڵ�:";
		return Find_data_isK(_pHead,data);
	}
	//�ڵ��Ƿ��ٶ�������
	bool InBTree(PNode node)
	{
		cout << "�ڵ��Ƿ��ڶ�������:";
		return InBTree(_pHead,node);
	}
	//�Ƿ�Ϊ��������
	bool Is_FullBTree()
	{
		cout << "�Ƿ�Ϊ��������" << " ";
		return Is_FullBTree(_pHead);
	}
	//ǰ��������������     2n��ָ����  ��Ч��ֻ��n-1�����ڵ�û��  NULL�ڵ����2*n-��n-1��
	void PreThreadBTree() 
	{
		PNode node = NULL;
	    _PreThreadBTree(_pHead,node);
	}
	//ǰ���������������ǵݹ����
	void PreOrderThreadNor()
	{
		_PreOrderThreadNor(_pHead);
	}
	//����������������
	void InThreadBTree()
	{
		PNode node = NULL;
		InThreadBTree(_pHead, node);
	}
	//����ǵݹ�����������Ķ�����
	void InOrderThreadNor()
	{
		_InOrderThreadNor(_pHead);
	}
	//����������������
	void PostThreadBTree()
	{
		PNode Pre = NULL;
		PostThreadBTree(_pHead,Pre);
	}
	//�����������������ǵݹ����
	void PostOrderThreadNor()
	{
		_PostOrderThreadNor(_pHead);
	}
protected:
	//���򴴽�������
    void CreateBTree(PNode& root,T* array, size_t size, size_t& index, T invalid,PNode& parent)
	{
	    if (index < size&&array[index] != invalid)
	    {
		  root = new Node(array[index]);
		  root->_parent = parent;
          CreateBTree(root->_left,array,size, ++index, invalid,root);
		  CreateBTree(root->_right,array, size, ++index, invalid,root);
	     }
    }
	//���򿽱�������
	PNode copyBTree(PNode node)
	{
		if (NULL == node)
			return NULL;
		PNode newnode = new Node(node->_val);
		newnode->_left = copyBTree(node->_left);
		newnode->_right = copyBTree(node->_right);
		return newnode;
	}
	//ǰ��������������
	void _PreThreadBTree(PNode root,PNode& Pre)
	{
		if (NULL == root)
			return;
		//��������ǰ�ڵ�����
		if (NULL == root->_left)
		{
			root->_left = Pre;
			root->Ltag = 1;
		}
		//������ǰһ���ڵ���Һ���
		if (NULL != Pre&&NULL == Pre->_right)
		{
			Pre->_right = root;
			Pre->Rtag = 1;
		}
		else if (Pre != NULL)
			Pre->Rtag = 0;
		Pre = root;
		if (root->Ltag==0)
			_PreThreadBTree(root->_left,Pre);
		if (root->Rtag==0)
		    _PreThreadBTree(root->_right,Pre);
	}
	//ǰ���������������ǵݹ����
	void _PreOrderThreadNor(PNode root)
	{
		PNode pCur = root;
		while (pCur)
		{
			while (pCur->Ltag == 0)   //���ӵ����        ��ӡ��֧
			{
				cout <<pCur->_val<< "->";
				pCur=pCur->_left;
			}
            //pCur_>Ltag==1;         //������
			cout << pCur->_val << "->"; 
			pCur = pCur->_right;              //�Һ��ӻ�������������������
		}
		cout <<"NULL"<<endl;
	}
	//����������������---�ڶ������������������������һ���ڵ�
	void InThreadBTree(PNode root,PNode& Pre)      //��һ���ڵ�ǰ�������һ���ڵ��̲���������ΪNULL
	{
		if (root)
		{
			//1.0 ����������
			InThreadBTree(root->_left,Pre);          //��������ǰ�ڵ�����Ӻ�ǰһ���ڵ���Һ���
			//2.0 root��������
			if (NULL == root->_left)
			{
				root->_left = Pre;
				root->Ltag = 1;
			}
			else
				root->Ltag = 0;    //����û����Ϊ��ʼ����Ĭ�Ͼ���Ϊ0
			//3.0 �Һ��ӵ�������
			if (NULL != Pre&&NULL == Pre->_right)      //˳��
			{
				Pre->_right = root;
				Pre->Rtag = 1;
			}
			else if (NULL!= Pre)
				Pre->Rtag = 0;
			Pre = root;
			InThreadBTree(root->_right, Pre);
		}
	}
	//�����������������ǵݹ����
	void _InOrderThreadNor(PNode root)
	{
		if (NULL == root)
			return;
		PNode pCur = root;
		while (pCur)
		{
			while (pCur->Ltag==0&&pCur->_left)          //���ӱ�־Ϊ1������ʱ �����½���
			{
				pCur = pCur->_left;
			}
			cout << pCur->_val<<"->";
			while (pCur->Rtag == 1)                    //ֻҪpCur���Һ��ӱ�־Ϊ1ʱΪ����������
			{
				pCur = pCur->_right;
				cout << pCur->_val;
			}
			//pCur->Rtag == 0
			pCur=pCur->_right;
		}
		cout << "NULL"<<endl;
	}
	//����������������
	void PostThreadBTree(PNode root,PNode& Pre)
	{
		if (NULL == root)
			return;
		PostThreadBTree(root->_left,Pre);             //����������
		PostThreadBTree(root->_right,Pre);            //�Һ���������
		if (NULL == root->_left)                  //��������ǰ�ڵ�����
		{
			root->_left = Pre;
			root->Ltag = 1;
		}
		if (NULL != Pre&&NULL == Pre->_right)     //������ǰһ���ڵ��Һ���
		{
			Pre->_right = root;
			Pre->Rtag = 1;
		}
		else if (NULL != Pre)
			Pre->Rtag = 0;
		Pre = root;          //����Pre
	}
	//�����������������ķǵݹ����
	void _PostOrderThreadNor(PNode root)
	{
		if (NULL == root)
			return;
		PNode pCur = root;
		PNode Pre = NULL;
		while(pCur)
		{
			//1.0 �ߵ������ڵ�
			while (pCur->_left != Pre&&pCur->Ltag == 0)         //ÿ�ΰ�pCur���µ�parentʱ���������Ѿ���������
			{                                                   //������ҪpCur->_left!=Pre;
				pCur = pCur->_left;                            
			}
			//ѭ���������������ڵ���Ϊ�����ڵ㱻��������  pCur->Ltag == 1
			//pCur��������������
			pCur = pCur->_left;
            //2.0 �ҵ�һ����������Ľڵ�
			while (pCur->Rtag == 1)  //�����˵��Һ���   
			{                                                  
				cout <<pCur->_val<< "->";
				Pre = pCur;
				pCur = pCur->_right;
			}
			//3.0�ж��ǲ��ǵ��˸��ڵ�
			if (pCur == root)
			{
				cout << pCur->_val << "->";
				return;
			}
			//4.0 ѭ��������ǰ��㲢����Ϊ��˫�׽ڵ�
			while(pCur&&Pre== pCur->_right)           //�ҵ�֧�����
			{
				cout << pCur->_val << "->";
				Pre = pCur;
				pCur = pCur->_parent;
			}
			//5.0 Pre��= pCur->_right ��pCur->_left==Pre,pCur->Rtag==0
			if (pCur&&pCur->Rtag == 0&&pCur->_right!=NULL)             //��������3���ܻ����ͷ����ӡ�����
			{
				pCur = pCur->_right;
			}
			else if (pCur&&pCur->Rtag == 0)  //pCurָ��ͷ
			{
				cout << pCur->_val << "->"<<"NULL";        
				return;
			}
		}
		cout << "NULL";
	}
	//�ݹ��������
	void _PreOrder1(PNode root)
	{
		if (root)
		{
			cout <<root->_val<< "-->";
			_PreOrder1(root->_left);
			_PreOrder1(root->_right);
		}
	}
	//�ݹ��������
	void _InOrder1(PNode root)
	{
		if (root)
		{
			_InOrder1(root->_left);
			cout << root->_val << "-->";
			_InOrder1(root->_right);
		}
	}
	//�ݹ�������
	void _PostOrder1(PNode root)
	{
		if (root)
		{
			_PostOrder1(root->_left);
			_PostOrder1(root->_right);
			cout << root->_val << "-->";
		}
	}
	//�ݹ�������  ---˼�������ģ��Сroot�ĵ�k��ڵ�   root��һ���k-1��      
	void  levelorderK(PNode node, size_t K)                     //��k��ı���
	{
		if (K == 0 || NULL==node)
			return;
		if (K == 1)                          //�ýڵ�����Ҫ��Ĳ���
		{
			cout <<node->_val<< "-->";
			return;
		}
		levelorderK(node->_left, K-1);
		levelorderK(node->_right,K-1);
	}
	void _LevelOrder1(PNode root)
	{
		if (NULL == root)
			return;
		size_t depth = _Depth(root);
		for (size_t i = 1; i <=depth; ++i)
		{
			levelorderK(root, i);         //i�������
		}
	}
    //�ǵݹ��������
	void _PreOrder2(PNode root)
	{
		stack<PNode> s;
		PNode pCur;
		if (root)
		  s.push(root);
		while (!s.empty())
		{
			pCur=s.top();
			cout <<pCur->_val<< "-->";
			s.pop();
			if (pCur->_right)         //��ѹ�Һ��Ӻ����Ӳ��ܱ�֤ջ��ʼ������֧�Ľڵ�
				s.push(pCur->_right);
			if (pCur->_left)
				s.push(pCur->_left);
		}
		cout <<"NULL"<<endl;
	}
	//�ǵݹ��������
	void _InOrder2(PNode root)
	{
		stack<PNode> s;         //�˴�����ѹջroot��Ϊ������֧��ջʱ������
		PNode pCur = root;         
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_left;
			}
			pCur = s.top();
			cout <<pCur->_val<< "-->";
			s.pop();
			pCur = pCur->_right;
		}
		cout <<"NULL"<<endl;
	}
	//�ǵݹ�������
	void _PostOrder2(PNode root)
	{
		stack<PNode> s;
		PNode pCur = root;
		PNode Pre = NULL;
		PNode pTop = NULL;
		while (pCur || !s.empty())
		{
			while (pCur)     //Ϊ�˱����ΈR���ظ��ڵ�  ��ջ��_rightΪ�յ������ pCurһֱ����NULL
			{
				s.push(pCur);
				pCur = pCur->_left;
			}
			pTop = s.top();
			if (NULL == pTop->_right || Pre == pTop->_right) //���ﲻ����pCur����ѭ������ͬһ��pCur�ᱻ��ΈR��
			{
				cout << pTop->_val << "-->";
				s.pop();
				Pre = pTop;
			}
			else
				pCur = pTop->_right;
		}
		cout <<"NULL"<<endl;
	}
	//�ǵݹ�������
	void _LevelOrder2(PNode root)
	{
		queue<PNode> q;
		if (root)
		   q.push(root);
		size_t size = 0;
		PNode pCur = NULL;
		while (!q.empty()) 
		{
			//size=q.size();          //size��ʾ��ǰ��Ľڵ���Ŀ
			//while (size--)          //��������ڷֲ��ӡ
			//{
			//	pCur = q.front();
			//	cout <<pCur->_val<< "-->";
			//	q.pop();
			//	if (pCur->_left)
			//		q.push(pCur->_left);
			//	if (pCur->_right)
			//		q.push(pCur->_right);
			//}
			pCur = q.front();
			cout <<pCur->_val<< "-->";
			q.pop();
    	    if (pCur->_left)
     		   q.push(pCur->_left);
			if (pCur->_right)
			   q.push(pCur->_right);

		}
		cout <<"NULL"<<endl;
	}
	size_t _Depth(PNode root)
	{
		if (NULL == root)
			return 0;
		size_t leftsize = _Depth(root->_left);
		size_t rightsize = _Depth(root->_right);
		return leftsize > rightsize ? leftsize + 1 : rightsize + 1;
	}
	//������� ��ǰ�� NULL֮������зǿսڵ�
	bool Is_completeBTree(PNode root)
	{
		queue<PNode> q;
		q.push(root);
		PNode pCur = root;
		//1.0 �����е�Ԫ������а���NULL�ڵ�  ֻҪ��ͷΪNULL�Ǿ��˳�ѭ���жϺ�����û�зǿսڵ�   ������û������
		while (pCur)
		{
			q.pop();
			q.push(pCur->_left);
		    q.push(pCur->_right);
			pCur = q.front();
		}
		while (!q.empty())
		{
			if (NULL == q.front())
				q.pop();
			else              //�зǿսڵ�
				return false;

		}
		//pCurΪ��--��ͷΪ��  qΪ��
		return true;
	}
	//�õ���k��ڵ����
	size_t _GetLevelKNodeNum(PNode root,size_t k)
	{
		if (k == 0 || NULL == root)
			return 0;
		if (k == 1)                    //�ýڵ�����Ҫ��Ĳ���
			return 1;
		return _GetLevelKNodeNum(root->_left,k-1)+_GetLevelKNodeNum(root->_right,k-1);
	}
	//�������ڵ����
	size_t NodeSize1(PNode root)
	{
		if (NULL == root)
			return 0;
		size_t depth = _Depth(root);
		size_t count= 0;
		for (size_t i = 1; i <= depth; ++i)
		{
			count += _GetLevelKNodeNum(root, i);         //i�������
		}
		return count;
	}
	size_t NodeSize2(PNode root)
	{
		if (NULL == root)
			return 0;
           
		return NodeSize2(root->_left)+NodeSize2(root->_right)+1;
	}
	//Ҷ�ӽڵ����
	size_t LeafSize(PNode root)
	{
		if (NULL == root)
			return 0;
		if (NULL == root->_left&&NULL == root->_right);
		    return 1;
	    return LeafSize(root->_left) + LeafSize(root->_right);
	}
	PNode Find_data_isK(PNode root, T& data)
	{
		if (root == NULL)
			return NULL;
		if (root->_val == data)
			return root;
		PNode temp = NULL;
		if (root->_left)               //���������
			temp=Find_data_isK(root->_left, data);
		if (temp)      //�������ҵ���
			return temp;
		return Find_data_isK(root->_right, data);	  //�������Ҳ�����������
	}
	//���������Ƿ��иýڵ�
	bool InBTree(PNode root,PNode node)
	{
		if (root == NULL || node == NULL)
			return false;
		if (root == node)
			return true;
		//��true���Ϊtrue
		return InBTree(root->_left,node) || InBTree(root->_right,node);
	}
	//�ж϶������Ƿ�Ϊ�������� k���Ӧ2^k+1���ڵ�
	bool Is_FullBTree(PNode root)
	{
		size_t depth = Depth();
		size_t nodesize=NodeSize();
		/*cout<<"���������ڵ�ӦΪ"<<pow(2, depth) - 1 << "�� ";*/
		return (pow(2,depth)-1)==nodesize;
	}
private:
	PNode _pHead;
};
void BTreeTest()
{
	char arr[7] = { 'A', 'C', '#', 'B', '#', '#', '#' };
	BTree<char> bt(arr, sizeof(arr) / sizeof(arr[0]), '#');
	//BTree<char> bt1(bt);
	//BTree<char> bt2;
	//bt2 = bt1;
	//bt2.PreOrder1();
	//bt2.InOrder1();
	//bt2.PostOrder1();
	//bt2.LevelOrder1();
	//cout << endl;
	//bt2.PreOrder2();
	//bt2.InOrder2();
	//bt2.PostOrder2();
	//bt2.LevelOrder2();
	//cout << "Depth:";
	//cout << bt2.Depth() << endl;
	//cout << bt2.Is_completeBTree() << " ";
	//cout << bt2.GetLevelKNodeNum(1) << " ";
	//cout << bt2.GetLevelKNodeNum(3) << " ";
	//cout << bt2.GetLevelKNodeNum(3) << endl;
	//cout << "�ڵ����:";
	//cout << bt2.NodeSize() << " " << endl;
	//cout << bt2.LeafSize();
	//char a = 'B';
	//char v = 'V';
	//cout<<bt2.Find_data_isK(a)->_val<<" ";
	///*cout << bt2.Find_data_isK(v)->_val << " ";*/   //���û����ô�� �Ҳ������صĽڵ�ΪNULLȡ��_val�Ƿ������ڴ�����
	//cout << bt2.InBTree(bt2.Find_data_isK(a)) << " ";
	//cout << bt2.InBTree(NULL) << " "<<endl;
	//cout << bt2.Is_FullBTree()<<" ";
	//char arr1[16] = "ABD##E##CF##G##";
	//BTree<char> bt4(arr1, sizeof(arr1) / sizeof(arr1[0]), '#');
	//cout << bt4.Is_FullBTree() << " ";
    //�������������
	//bt2.PreThreadBTree();
	//bt2.PreOrderThreadNor();
	//�������������
	//bt2.InThreadBTree();
    //bt2.InOrderThreadNor();
	//�������������
	//bt.PostThreadBTree();
	//bt.PostOrderThreadNor();
}