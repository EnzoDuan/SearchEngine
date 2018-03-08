#pragma warning (disable:4996)
#include<iostream>         
#include<cstdlib>
#include<ctime>  
#define BTREELENGTH 50
#define BTLEN (sizeof(BTNode))
#define MAXINT 10000000

typedef enum status
{
	TRUE,
	FALSE,
	OK,
	ERROR,
	OVER_FLOW,
	EMPTY
}Status;

typedef int KeyType;

//**********************************B��****************************************
#define  m  40   // B���Ľף�����Ϊ4
typedef struct
{
	KeyType  key;
	char     data;
} Record;

typedef struct BTNode
{
	int             keynum;				// ����йؼ��ָ����������Ĵ�С
	struct BTNode  *parent;				// ָ��˫�׽��
	KeyType         key[m + 1];			// �ؼ���������0�ŵ�Ԫδ��
	struct BTNode  *ptr[m + 1];			// ����ָ������
										// Record *recptr[m + 1];   // ��¼ָ��������0�ŵ�Ԫδ��
										//�ڴ���������Զ�������
} BTNode, *BTree;						// B������B��������
typedef struct
{
	BTNode  *pt;						// ָ���ҵ��Ľ��
	int      i;							// 1..m���ڽ���еĹؼ������
	int      tag;						// 1:���ҳɹ���0:����ʧ��
} Result;								// ��B���Ĳ��ҽ������   
					//**********************************B��****************************************

					//**********************************����***************************************
typedef struct LNode {
	BTree data;     // ������
	struct LNode *next;     // ָ����
} LNode, *LinkList;
//**********************************����***************************************

/*class BTree {
public:
	Status InitQueue_L(LinkList &L);
	LNode* MakeNode_L(BTree e);

};
*/
Status InitQueue_L(LinkList &L)
{ // ��ʼ��һ��ֻ��ͷ���Ŀյ�����L
	if (NULL == (L = (LNode*)malloc(sizeof(LNode)))) // �����½��
		return OVER_FLOW;
	L->next = NULL;
	return OK;
}
/***
*  @name           LNode* MakeNode_L(BTree e)
*  @description    ������н��
*  @return         ���ؽ���ַ
*  @notice
***/
LNode* MakeNode_L(BTree e)
{ // ����������Ϊe�ĵ�������
	LNode *p;
	p = new LNode; // ������ռ�
	if (p != NULL)
	{
		p->data = e;
		p->next = NULL;
	}
	return p;
}
/***
*  @name           Status Enqueue_L(LNode *p, BTree e)
*  @description    ���е����
*  @return         �ɹ�����OK�����򷵻�ERROR
*  @notice
***/
Status Enqueue_L(LNode *p, BTree e)
{ //��p���֮�����q���
	if (NULL == p) return ERROR; // ����������
	while (p->next != NULL)
		p = p->next;
	p->next = MakeNode_L(e);         // ��Ӧͼ4.11��b���Ģڣ��޸�p����ָ����
	return OK;
}

/***
*  @name           Status Dequeue_L(LNode *p, BTree &e)
*  @description    ���еĳ���
*  @return         �ɹ�����OK�����򷵻�ERROR
*  @notice
***/
Status Dequeue_L(LNode *p, BTree &e)
{
	// ɾ��p����ֱ�Ӻ�̽�㲢�ò���e���ر�ɾ����ֵ
	LNode *q;
	if (NULL == p || NULL == p->next) return ERROR; // ɾ��λ�ò����� 
	q = p->next;
	p->next = q->next; // �޸ı�ɾ���q��ָ����
	e = q->data;
	free(q); // �ͷŽ��q
	return OK;
}

/***
*  @name           void DestroyQueue(LinkList L)
*  @description    ���е�����
*  @return         �޷���
*  @notice
***/
void DestroyQueue(LinkList L)
{
	// ������������
	LinkList p;
	if (L != NULL)
	{
		p = L;
		L = L->next;
		free(p);
		DestroyQueue(L);
	}
}
/***
*  @name           Status  IfEmpty(LinkList L)
*  @description    �ж϶����Ƿ�Ϊ��
*  @return         �շ���TRUE�����շ���FALSE�����򷵻�ERROR
*  @notice
***/
Status  IfEmpty(LinkList L)
{
	if (L == NULL) return ERROR;
	if (L->next == NULL) return TRUE;
	return FALSE;
}
/***
*  @name           Status ergodic(BTree T, LinkList L, int newline, int sum)
*  @description    print��Ҫ�õ��ĵݹ��������
*  @return         �ɹ�����OK
*  @notice         �˴��õ�����
***/
Status ergodic(BTree T, LinkList L, int newline, int sum)
{
	int index;
	BTree p;
	if (T != NULL)
	{
		printf("[ ");
		Enqueue_L(L, T->ptr[0]);
		for (index = 1; index <= T->keynum; index++)
		{
			printf("%d ", T->key[index]);
			Enqueue_L(L, T->ptr[index]);
		}
		sum += T->keynum + 1;
		printf("]");
		if (newline == 0)
		{
			printf("\n");
			newline = sum - 1;
			sum = 0;
		}
		else
		{
			--newline;
		}
	}
	if (IfEmpty(L) == FALSE)
	{
		Dequeue_L(L, p);
		ergodic(p, L, newline, sum);
	}
	return OK;
}
/***
*  @name           Status print(BTree T)
*  @description    ��α������ֲ����B��
*  @return         �ɹ�����OK
*  @notice
***/
Status print(BTree T)
{
	LinkList L;
	if (T == NULL)
	{
		printf("[ ]\n");
		return OK;
	}
	InitQueue_L(L);
	ergodic(T, L, 0, 0);
	DestroyQueue(L);
	return OK;
}

/***
*  @name           Status findMax(BTree T, BTree &p,int ans)
*  @description    Ѱ�����ؼ��ֵĽ��,TΪҪѰ�ҵ�����pΪ���صĽڵ�,ansΪ�ڼ���
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
Status findMax(BTree T, BTree &p, int &ans)
{
	if (T == NULL)
		return ERROR;
	p = T;
	while (p->ptr[p->keynum] != NULL)
	{
		p = p->ptr[p->keynum];
	}
	ans = p->keynum;
	return OK;
}
/***
*  @name           Status findMin(BTree T, BTree &p,int ans)
*  @description    Ѱ����С�ؼ��ֵĽ��,TΪҪѰ�ҵ�����pΪ���صĽڵ�,ansΪ�ڼ���
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
/***
*  @name           Status findBTree(BTree T, BTree &p, int &ans, KeyType k)
*  @description    Ѱ�� ,TΪҪѰ�ҵ�����pΪ���صĽڵ㣬ansΪ�ڼ���Ԫ�أ�kΪҪ�ҵ�ֵ
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
Status findBTree(BTree T, BTree &p, int &ans, KeyType k)
{
	BTree q;
	int index = 1;
	KeyType  keynow;
	if (T == NULL)
		return ERROR;
	q = T;
	keynow = T->key[1];
	while (q != NULL)                 //��ȵı���
	{
		index = 1;
		keynow = q->key[index];
		while (index <= q->keynum) //�ڵ��ڶԸ���ֵ���б���
		{
			if (k == keynow)           //�ҵ�Ԫ��
			{
				p = q;
				ans = index;
				return TRUE;
			}
			if (k > keynow)
			{
				if (index == q->keynum)
				{
					if (q->ptr[index] == NULL)
					{
						p = q;
						ans = q->keynum + 1;
						return FALSE;
					}
					q = q->ptr[index];
					break;
				}
				++index;
				keynow = q->key[index];
				continue;
			}
			if (k < keynow)
			{
				if (q->ptr[index - 1] == NULL)
				{
					p = q;
					ans = index;
					return FALSE;
				}
				q = q->ptr[index - 1];
				break;
			}
		}
	}

	return ERROR;
}
/***
*  @name           Status renewParent(BTree p)
*  @description    ���ߺ���������ְ���˭
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
Status renewParent(BTree p)
{
	int index;
	if (p == NULL) return ERROR;
	for (index = 0; index <= p->keynum; ++index)
	{
		if (p->ptr[index] != NULL)
		{
			p->ptr[index]->parent = p;
			renewParent(p->ptr[index]);
		}
	}
	return OK;
}
/***
*  @name           int whichSon(BTree T)
*  @description    �ҳ��Ǹ��׵ĵڼ�������
*  @return         �ɹ����صڼ�������,���򷵻�-1
*  @notice
***/
int whichSon(BTree T)
{
	int index = -1;
	if (T == NULL) return -1;
	for (index = 0; index <= T->parent->keynum; ++index) //�ҳ��Ǹ��׵ĵڼ�������
	{
		if (T->parent->ptr[index] == T) return index;
	}
	return -1;
}
/***
*  @name           status splitBTree(BTree T)
*  @description    �ݹ�ʵ�ַ��ѽڵ����
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
Status splitBTree(BTree T) //��ʱ���ѵĽڵ�һ�����ǳ������ֵ�ġ�
{
	BTree t1, t2;
	int index, index_1;
	if (T->parent == NULL)
	{
		t1 = (BTree)malloc(BTLEN);
		if (NULL == t1) return OVER_FLOW;
		t2 = (BTree)malloc(BTLEN);
		if (NULL == t2) return OVER_FLOW;

		t1->keynum = m / 2;
		t2->keynum = m - (m / 2) - 1;
		t1->parent = T;
		t2->parent = T;
		for (index = 0; index <= m; ++index)  //��ȫ����ʼ��
		{
			t1->ptr[index] = NULL;
			t1->key[index] = 0;
			t2->ptr[index] = NULL;
			t2->key[index] = 0;
		}
		for (index = 0; index <= m / 2; ++index)  //��ʼ��t1
		{
			t1->ptr[index] = T->ptr[index];
			t1->key[index] = T->key[index];
		}
		t2->ptr[0] = T->ptr[(m / 2) + 1];
		for (index = (m / 2) + 2; index <= m; ++index)  //��ʼ��t2
		{
			t2->ptr[index - ((m / 2) + 1)] = T->ptr[index];
			t2->key[index - ((m / 2) + 1)] = T->key[index];
		}
		T->keynum = 1;
		T->ptr[0] = t1;
		T->ptr[1] = t2;
		T->key[1] = T->key[m / 2 + 1];
		for (index = 2; index <= m; ++index)  //��ʼ��T
		{
			T->ptr[index] = NULL;
			T->key[index] = 0;
		}
		return OK;
	}

	index = whichSon(T);
	for (index_1 = T->parent->keynum; index_1 > index; --index_1) //�ڳ����׵�λ��
	{
		T->parent->ptr[index_1 + 1] = T->parent->ptr[index_1];
		T->parent->key[index_1 + 1] = T->parent->key[index_1];
	}
	T->parent->keynum++;
	T->parent->key[index + 1] = T->key[m / 2 + 1];
	t2 = T->parent->ptr[index + 1] = (BTree)malloc(BTLEN);
	if (NULL == t2) return OVER_FLOW;
	for (index = 0; index <= m; ++index)  //��ȫ����ʼ��
	{
		t2->ptr[index] = NULL;
		t2->key[index] = 0;
	}
	t2->keynum = m - (m / 2) - 1;
	t2->parent = T->parent;
	t2->ptr[0] = T->ptr[(m / 2) + 1];
	for (index = (m / 2) + 2; index <= m; ++index)  //��ʼ��t2
	{
		t2->ptr[index - ((m / 2) + 1)] = T->ptr[index];
		t2->key[index - ((m / 2) + 1)] = T->key[index];
	}
	T->keynum = m / 2;
	for (index = (m / 2) + 1; index <= m; ++index)  //��ʼ��t2
	{
		T->ptr[index] = NULL;
		T->key[index] = 0;
	}
	if (T->parent->keynum == m)
	{
		splitBTree(T->parent);
	}
	return OK;
}
/***
*  @name           Status insertBTree(BTree &T, Record e)
*  @description    ����ʵ��Ԫ�صĲ���
*  @return         �ɹ�����OK,��������򷵻�FALSE�����򷵻�ERROR
*  @notice
***/
Status insertBTree(BTree &T, Record e)
{
	BTree p;
	int index, temp;
	Status find_flag;
	if (NULL == T)
	{
		T = (BTree)malloc(BTLEN);
		if (NULL == T) return OVER_FLOW;
		T->keynum = 1;
		T->parent = NULL;
		for (index = 0; index <= m; ++index)
		{
			T->ptr[index] = NULL;
			T->key[index] = 0;
		}
		T->key[1] = e.key;
		return OK;
	}
	find_flag = findBTree(T, p, temp, e.key);
	if (find_flag == TRUE)
	{
		return FALSE;
	}
	if (find_flag == FALSE)
	{                                //����������ֱ�Ӳ���
		p->keynum++;
		for (index = p->keynum; index > temp; --index)
		{
			p->key[index] = p->key[index - 1];
			p->ptr[index] = p->ptr[index - 1];
		}
		p->ptr[temp] = NULL;
		p->key[temp] = e.key;
		if (p->keynum == m)      //��������÷���
		{
			splitBTree(p);
		}
		renewParent(T);
		return OK;
	}
	return ERROR;
}
/***
*  @name           Status borrowBNode(BTree &T)
*  @description    �ݹ�ʵ�֣����ֵܽ�Ԫ�أ�������ֵܺϲ�
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice         �������Ӧ����TΪ��Ԫ�ؽ��
***/
Status borrowBNode(BTree T)
{
	int mynum, bronum, index;
	BTree b = NULL, f = NULL;
	if (T == NULL) return ERROR;
	f = T->parent;
	if (f == NULL)//���Ǹ��׽�㲻���ڵ����
	{
		if (T->keynum == 0)
		{
			f = T->ptr[0];
			if (f == NULL)
			{
				free(T);
				return EMPTY;
			}
			for (index = 0; index <= f->keynum; index++)
			{
				T->key[index] = f->key[index];
				T->ptr[index] = f->ptr[index];
			}
			T->keynum = f->keynum;
			free(f);
			renewParent(T);
		}
		return OK;
	}
	mynum = whichSon(T);
	if (mynum == 0)
		bronum = 1;
	else
		bronum = mynum - 1;
	b = f->ptr[bronum];
	if (b->keynum == (m + 1) / 2 - 1) //����ֵܰﲻ������
	{
		//��ô�ͺ�����ֵܺ���
		if (bronum < mynum)                    //����Ҳ��ǵ�һ��
		{
			b->keynum++;
			b->key[b->keynum] = f->key[mynum];
			b->ptr[b->keynum] = T->ptr[0];
			for (index = 1; index <= T->keynum; index++)
			{
				b->key[index + b->keynum] = T->key[index];
				b->ptr[index + b->keynum] = T->ptr[index];
				b->keynum++;
			}
			free(T);
			for (index = mynum; index <= f->keynum; index++)
			{
				f->key[index] = f->key[index + 1];
				f->ptr[index] = f->ptr[index + 1];
			}
			f->keynum--;
		}
		else
		{
			T->keynum++;
			T->key[T->keynum] = f->key[bronum];
			T->ptr[T->keynum] = b->ptr[0];
			for (index = 1; index <= b->keynum; index++)
			{
				T->key[index + T->keynum] = b->key[index];
				T->ptr[index + T->keynum] = b->ptr[index];
				T->keynum++;
			}
			free(b);
			for (index = bronum; index <= f->keynum; index++)
			{
				f->key[index] = f->key[index + 1];
				f->ptr[index] = f->ptr[index + 1];
			}
			f->keynum--;
		}
		renewParent(f);
		if (f->keynum == (m + 1) / 2 - 2)
		{
			//���ý��ֵܵĺ���
			return borrowBNode(f);
		}
	}
	else//����ֵ��ܹ�����
	{
		if (bronum < mynum)                    //����Ҳ��ǵ�һ��
		{
			for (index = 1; index <= T->keynum; index++)
			{
				T->key[index + 1] = T->key[index];
				T->ptr[index + 1] = T->ptr[index];
			}
			T->ptr[1] = T->ptr[0];
			T->key[1] = f->key[mynum];
			T->ptr[0] = b->ptr[b->keynum];
			T->keynum++;
			f->key[mynum] = b->key[b->keynum];
			b->key[b->keynum] = 0;
			b->ptr[b->keynum] = NULL;
			b->keynum--;

		}
		else                                    //������ǵ�һ��
		{
			T->keynum++;
			T->key[T->keynum] = f->key[1];
			T->ptr[T->keynum] = b->ptr[0];
			f->key[1] = b->key[1];
			b->ptr[0] = b->ptr[1];
			for (index = 1; index <= b->keynum; index++)
			{
				b->key[index] = b->key[index + 1];
				b->ptr[index] = b->ptr[index + 1];
			}
			b->keynum--;
		}
	}
	return OK;
}

/***
*  @name           Status deleteBTreeRecord(BTree &T, Record e)
*  @description    ʵ��B��Ԫ�ص�ɾ��
*  @return         �ɹ�����OK,���򷵻�ERROR
*  @notice
***/
Status deleteBTreeRecord(BTree &T, Record e)
{
	BTree p, q;
	int num, temp, index;
	Status find_flag;
	if (T == NULL)
		return ERROR;
	find_flag = findBTree(T, p, temp, e.key);
	if (find_flag == FALSE)
	{
		return FALSE;
	}
	if (find_flag == TRUE)
	{
		//deleteBTreeBNode(p,temp);
		if (p->ptr[temp] == NULL)                //�����Ҷ�ӽڵ�Ļ�
		{
			for (index = temp; index <= p->keynum; ++index)
			{
				p->key[index] = p->key[index + 1];
				p->ptr[index] = p->ptr[index + 1];
			}
			p->keynum--;
			if (p->keynum == (m + 1) / 2 - 2)
			{
				//���ý��ֵܵĺ���
				if (borrowBNode(p) == EMPTY) T = NULL;
				else renewParent(T);
			}
			return OK;
		}
		else                                    //����Ҷ�ӽ��Ļ�
		{
			//����
			findMax(p->ptr[temp - 1], q, num);//���ص�qһ������Ҷ�ӽڵ�
			p->key[temp] = q->key[num];
			q->key[num] = 0;
			q->keynum--;
			if (q->keynum == (m + 1) / 2 - 2)
			{
				//���ý��ֵܵĺ���
				if (borrowBNode(q) == EMPTY) T = NULL;
				else renewParent(T);
			}
			return OK;
		}
		return OK;
	}
	return ERROR;
}
/***
*  @name           Status initBTree(BTree &t)
*  @description    ��ʼ��һ����B��
*  @return         �ɹ�����OK
*  @notice
***/
Status initBTree(BTree &t)
{
	t = NULL;
	return OK;
}

/***
������
***/
int  main()
{
	//Set();
	return 0;
}