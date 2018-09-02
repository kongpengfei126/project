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
	bool Ltag;          // 1为链接的前驱和后继  0对应 左孩子右孩子
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
	//数组构造二叉树
	BTree(T* array, size_t size, T invalid)
	{
		size_t index = 0;
		PNode parent = NULL;
		CreateBTree(_pHead,array, size, index, invalid,parent);
	}
   //拷贝构造（深拷贝） 
	BTree(const BTree& bt)
	{
		_pHead = copyBTree(bt._pHead);
	}
	//赋值运算符重载
	BTree& operator=(const BTree& bt)
	{
		if (this != &bt)     //不是自拷贝
		{
			//1.0 析构自己
			this->~BTree();
			//2.0 深拷贝调拷贝构造函数
			this->_pHead=this->copyBTree(bt._pHead);
		}
		return *this;
	}
	//递归遍历
	void PreOrder1()  
	{ 
		cout << "递归先序遍历:";
		_PreOrder1(_pHead);
		cout << "NULL" << endl;
	}
	void InOrder1()
	{
		cout << "递归中序遍历:";
		_InOrder1(_pHead);
		cout <<"NULL"<<endl;
	}
	void PostOrder1()
	{
		cout << "递归后序遍历:";
		_PostOrder1(_pHead);
		cout << "NULL" << endl;
	}
	void LevelOrder1()
	{
		cout << "递归层序遍历:";
		_LevelOrder1(_pHead);
		cout << "NULL" << endl;
	}
	//非递归遍历
	void PreOrder2()
	{
		cout << "非递归先序遍历:";
		_PreOrder2(_pHead);
	}
	void InOrder2()
	{
		cout << "非递归先序遍历:";
		_InOrder2(_pHead);
	}
	void PostOrder2()
	{
		cout << "非递归先序遍历:";
		_PostOrder2(_pHead);
	}
	void LevelOrder2()
	{
		cout << "非递归先序遍历:";
		_LevelOrder2(_pHead);

	}
	//树的深度
	size_t Depth()
	{
		return _Depth(_pHead);
	}
	bool Is_completeBTree()
	{
		cout <<"是不是完全二叉树" ;
		return Is_completeBTree(_pHead);
	}
	size_t GetLevelKNodeNum(size_t k)
	{
		cout << "第"<<k<<"层节点个数:";
		return _GetLevelKNodeNum(_pHead,k);
	}
	size_t NodeSize()
	{
		/*return NodeSize1(_pHead);*/  //方式1
		return NodeSize2(_pHead);      //方式2
	}
	size_t LeafSize()
	{
		cout << "叶子节点个数:";
		return LeafSize(_pHead);
	}
	PNode Find_data_isK(T& data)
	{
		cout << "查找值为"<<data<<"的节点:";
		return Find_data_isK(_pHead,data);
	}
	//节点是否再二叉树中
	bool InBTree(PNode node)
	{
		cout << "节点是否在二叉树中:";
		return InBTree(_pHead,node);
	}
	//是否为满二叉树
	bool Is_FullBTree()
	{
		cout << "是否为满二叉树" << " ";
		return Is_FullBTree(_pHead);
	}
	//前序线索化二叉树     2n个指针域  有效的只有n-1个根节点没有  NULL节点个数2*n-（n-1）
	void PreThreadBTree() 
	{
		PNode node = NULL;
	    _PreThreadBTree(_pHead,node);
	}
	//前序线索化二叉树非递归遍历
	void PreOrderThreadNor()
	{
		_PreOrderThreadNor(_pHead);
	}
	//中序线索化二叉树
	void InThreadBTree()
	{
		PNode node = NULL;
		InThreadBTree(_pHead, node);
	}
	//中序非递归遍历线索化的二叉树
	void InOrderThreadNor()
	{
		_InOrderThreadNor(_pHead);
	}
	//后序线索化二叉树
	void PostThreadBTree()
	{
		PNode Pre = NULL;
		PostThreadBTree(_pHead,Pre);
	}
	//后序线索化二叉树非递归遍历
	void PostOrderThreadNor()
	{
		_PostOrderThreadNor(_pHead);
	}
protected:
	//先序创建二叉树
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
	//先序拷贝二叉树
	PNode copyBTree(PNode node)
	{
		if (NULL == node)
			return NULL;
		PNode newnode = new Node(node->_val);
		newnode->_left = copyBTree(node->_left);
		newnode->_right = copyBTree(node->_right);
		return newnode;
	}
	//前序线索化二叉树
	void _PreThreadBTree(PNode root,PNode& Pre)
	{
		if (NULL == root)
			return;
		//线索化当前节点左孩子
		if (NULL == root->_left)
		{
			root->_left = Pre;
			root->Ltag = 1;
		}
		//线索化前一个节点的右孩子
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
	//前序线索化二叉树非递归遍历
	void _PreOrderThreadNor(PNode root)
	{
		PNode pCur = root;
		while (pCur)
		{
			while (pCur->Ltag == 0)   //左孩子的情况        打印左单支
			{
				cout <<pCur->_val<< "->";
				pCur=pCur->_left;
			}
            //pCur_>Ltag==1;         //左线索
			cout << pCur->_val << "->"; 
			pCur = pCur->_right;              //右孩子还是右线索都来到这里
		}
		cout <<"NULL"<<endl;
	}
	//中序线索化二叉树---第二个参数用来保存中序遍历上一个节点
	void InThreadBTree(PNode root,PNode& Pre)      //第一个节点前驱和最后一个节点后继不用线索化为NULL
	{
		if (root)
		{
			//1.0 左孩子线索化
			InThreadBTree(root->_left,Pre);          //线索化当前节点的左孩子和前一个节点的右孩子
			//2.0 root的线索化
			if (NULL == root->_left)
			{
				root->_left = Pre;
				root->Ltag = 1;
			}
			else
				root->Ltag = 0;    //可以没有因为初始化已默认经置为0
			//3.0 右孩子的线索化
			if (NULL != Pre&&NULL == Pre->_right)      //顺序
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
	//中序线索化二叉树非递归遍历
	void _InOrderThreadNor(PNode root)
	{
		if (NULL == root)
			return;
		PNode pCur = root;
		while (pCur)
		{
			while (pCur->Ltag==0&&pCur->_left)          //左孩子标志为1即线索时 往左下角走
			{
				pCur = pCur->_left;
			}
			cout << pCur->_val<<"->";
			while (pCur->Rtag == 1)                    //只要pCur的右孩子标志为1时为线索往后走
			{
				pCur = pCur->_right;
				cout << pCur->_val;
			}
			//pCur->Rtag == 0
			pCur=pCur->_right;
		}
		cout << "NULL"<<endl;
	}
	//后序线索化二叉树
	void PostThreadBTree(PNode root,PNode& Pre)
	{
		if (NULL == root)
			return;
		PostThreadBTree(root->_left,Pre);             //左孩子线索化
		PostThreadBTree(root->_right,Pre);            //右孩子线索化
		if (NULL == root->_left)                  //线索化当前节点左孩子
		{
			root->_left = Pre;
			root->Ltag = 1;
		}
		if (NULL != Pre&&NULL == Pre->_right)     //线索化前一个节点右孩子
		{
			Pre->_right = root;
			Pre->Rtag = 1;
		}
		else if (NULL != Pre)
			Pre->Rtag = 0;
		Pre = root;          //更新Pre
	}
	//后序线索化二叉树的非递归遍历
	void _PostOrderThreadNor(PNode root)
	{
		if (NULL == root)
			return;
		PNode pCur = root;
		PNode Pre = NULL;
		while(pCur)
		{
			//1.0 走到最左侧节点
			while (pCur->_left != Pre&&pCur->Ltag == 0)         //每次把pCur更新到parent时其左子树已经遍历过了
			{                                                   //所以需要pCur->_left!=Pre;
				pCur = pCur->_left;                            
			}
			//循环结束后在最左侧节点因为最左侧节点被线索化了  pCur->Ltag == 1
			//pCur到后序遍历的起点
			pCur = pCur->_left;
            //2.0 找第一个后序遍历的节点
			while (pCur->Rtag == 1)  //线索了的右孩子   
			{                                                  
				cout <<pCur->_val<< "->";
				Pre = pCur;
				pCur = pCur->_right;
			}
			//3.0判断是不是到了根节点
			if (pCur == root)
			{
				cout << pCur->_val << "->";
				return;
			}
			//4.0 循环遍历当前结点并更新为其双亲节点
			while(pCur&&Pre== pCur->_right)           //右单支的情况
			{
				cout << pCur->_val << "->";
				Pre = pCur;
				pCur = pCur->_parent;
			}
			//5.0 Pre！= pCur->_right 即pCur->_left==Pre,pCur->Rtag==0
			if (pCur&&pCur->Rtag == 0&&pCur->_right!=NULL)             //不加条件3可能会出现头不打印的情况
			{
				pCur = pCur->_right;
			}
			else if (pCur&&pCur->Rtag == 0)  //pCur指向头
			{
				cout << pCur->_val << "->"<<"NULL";        
				return;
			}
		}
		cout << "NULL";
	}
	//递归先序遍历
	void _PreOrder1(PNode root)
	{
		if (root)
		{
			cout <<root->_val<< "-->";
			_PreOrder1(root->_left);
			_PreOrder1(root->_right);
		}
	}
	//递归中序遍历
	void _InOrder1(PNode root)
	{
		if (root)
		{
			_InOrder1(root->_left);
			cout << root->_val << "-->";
			_InOrder1(root->_right);
		}
	}
	//递归后序遍历
	void _PostOrder1(PNode root)
	{
		if (root)
		{
			_PostOrder1(root->_left);
			_PostOrder1(root->_right);
			cout << root->_val << "-->";
		}
	}
	//递归层序遍历  ---思想问题规模减小root的第k层节点   root下一层的k-1层      
	void  levelorderK(PNode node, size_t K)                     //第k层的遍历
	{
		if (K == 0 || NULL==node)
			return;
		if (K == 1)                          //该节点在所要求的层上
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
			levelorderK(root, i);         //i代表层数
		}
	}
    //非递归先序遍历
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
			if (pCur->_right)         //先压右孩子后左孩子才能保证栈顶始终是左单支的节点
				s.push(pCur->_right);
			if (pCur->_left)
				s.push(pCur->_left);
		}
		cout <<"NULL"<<endl;
	}
	//非递归中序遍历
	void _InOrder2(PNode root)
	{
		stack<PNode> s;         //此处不先压栈root是为了在左单支入栈时条件简单
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
	//非递归后序遍历
	void _PostOrder2(PNode root)
	{
		stack<PNode> s;
		PNode pCur = root;
		PNode Pre = NULL;
		PNode pTop = NULL;
		while (pCur || !s.empty())
		{
			while (pCur)     //为了避免多次圧入重复节点  在栈顶_right为空的情况下 pCur一直保持NULL
			{
				s.push(pCur);
				pCur = pCur->_left;
			}
			pTop = s.top();
			if (NULL == pTop->_right || Pre == pTop->_right) //这里不能用pCur来做循环条件同一个pCur会被多次圧入
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
	//非递归层序遍历
	void _LevelOrder2(PNode root)
	{
		queue<PNode> q;
		if (root)
		   q.push(root);
		size_t size = 0;
		PNode pCur = NULL;
		while (!q.empty()) 
		{
			//size=q.size();          //size表示当前层的节点数目
			//while (size--)          //这个适用于分层打印
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
	//层序遍历 当前层 NULL之后如果有非空节点
	bool Is_completeBTree(PNode root)
	{
		queue<PNode> q;
		q.push(root);
		PNode pCur = root;
		//1.0 将所有的元素入队列包括NULL节点  只要队头为NULL那就退出循环判断后面有没有非空节点   有则不是没有则是
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
			else              //有非空节点
				return false;

		}
		//pCur为空--队头为空  q为空
		return true;
	}
	//得到第k层节点个数
	size_t _GetLevelKNodeNum(PNode root,size_t k)
	{
		if (k == 0 || NULL == root)
			return 0;
		if (k == 1)                    //该节点在所要求的层上
			return 1;
		return _GetLevelKNodeNum(root->_left,k-1)+_GetLevelKNodeNum(root->_right,k-1);
	}
	//二叉树节点个数
	size_t NodeSize1(PNode root)
	{
		if (NULL == root)
			return 0;
		size_t depth = _Depth(root);
		size_t count= 0;
		for (size_t i = 1; i <= depth; ++i)
		{
			count += _GetLevelKNodeNum(root, i);         //i代表层数
		}
		return count;
	}
	size_t NodeSize2(PNode root)
	{
		if (NULL == root)
			return 0;
           
		return NodeSize2(root->_left)+NodeSize2(root->_right)+1;
	}
	//叶子节点个数
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
		if (root->_left)               //先序遍历找
			temp=Find_data_isK(root->_left, data);
		if (temp)      //左子树找到了
			return temp;
		return Find_data_isK(root->_right, data);	  //左子树找不到走右子树
	}
	//二叉树里是否有该节点
	bool InBTree(PNode root,PNode node)
	{
		if (root == NULL || node == NULL)
			return false;
		if (root == node)
			return true;
		//有true结果为true
		return InBTree(root->_left,node) || InBTree(root->_right,node);
	}
	//判断二叉树是否为满二叉树 k层对应2^k+1个节点
	bool Is_FullBTree(PNode root)
	{
		size_t depth = Depth();
		size_t nodesize=NodeSize();
		/*cout<<"满二叉树节点应为"<<pow(2, depth) - 1 << "个 ";*/
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
	//cout << "节点个数:";
	//cout << bt2.NodeSize() << " " << endl;
	//cout << bt2.LeafSize();
	//char a = 'B';
	//char v = 'V';
	//cout<<bt2.Find_data_isK(a)->_val<<" ";
	///*cout << bt2.Find_data_isK(v)->_val << " ";*/   //这个没法这么测 找不到返回的节点为NULL取其_val非法访问内存会出错
	//cout << bt2.InBTree(bt2.Find_data_isK(a)) << " ";
	//cout << bt2.InBTree(NULL) << " "<<endl;
	//cout << bt2.Is_FullBTree()<<" ";
	//char arr1[16] = "ABD##E##CF##G##";
	//BTree<char> bt4(arr1, sizeof(arr1) / sizeof(arr1[0]), '#');
	//cout << bt4.Is_FullBTree() << " ";
    //先序线索化检测
	//bt2.PreThreadBTree();
	//bt2.PreOrderThreadNor();
	//中序线索化检测
	//bt2.InThreadBTree();
    //bt2.InOrderThreadNor();
	//后序线索化检测
	//bt.PostThreadBTree();
	//bt.PostOrderThreadNor();
}