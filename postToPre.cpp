#include <iostream>
#include <vector>

using namespace std;

class Node {
public:
	int data;
	Node* par;
	vector<Node*> chi_v;

	Node(int data)
	{
		this->data = data;
		this->par = NULL;
	}
};

class Tree {
public:
	Node* root;
	vector<Node*> node_v;

	Tree(int);
	void insertNode(int, int);
	void delNode(int);
	void preorder(Node*);
	void postorder(Node*);
};

Tree::Tree(int data)
{
	Node* node = new Node(data);
	this->root = node;
	this->node_v.push_back(node);
}

void Tree::insertNode(int par_data, int data)
{
	Node* node = new Node(data);

	for (int i = 0; i < node_v.size(); i++)
	{
		if (node_v[i]->data == par_data)
		{
			node_v[i]->chi_v.push_back(node);
			node_v.push_back(node);
			node->par = node_v[i];
			return;
		}
	}

	cout << -1 << endl;
}

void Tree::delNode(int data)
{
	Node* curNode;
	Node* parNode;

	for (int i = 0; i < node_v.size(); i++)
	{
		if (node_v[i]->data == data)
		{
			if (node_v[i] == root)
			{
				cout << -1 << endl;
				return;
			}

			curNode = node_v[i];
			parNode = curNode->par;
			for (Node* child : curNode->chi_v)
			{
				parNode->chi_v.push_back(child);
				child->par = parNode;
			}

			for (int j = 0; j < parNode->chi_v.size(); j++)
			{
				if (parNode->chi_v[j]->data == data)
				{
					parNode->chi_v.erase(parNode->chi_v.begin() + j);
				}
			}

			node_v.erase(node_v.begin() + i);
			delete curNode;
			return;
		}
	}

	cout << -1 << endl;
}

void Tree::preorder(Node* node)
{
	if (!node) return;

	cout << node->data << " ";

	for (int i = 0; i < node->chi_v.size(); i++)
	{
		preorder(node->chi_v[i]);
	}

}

void Tree::postorder(Node* node)
{
	if (!node) return;

	for (int i = 0; i < node->chi_v.size(); i++)
	{
		postorder(node->chi_v[i]);
	}

	if (node == root)
		cout << 0 << " ";
	else
		cout << node->par->data << " ";
}

int* getReach(int* depth, int data)	// 배열 내에서 숫자의 변화가 어디서 일어나는지 저장
{
	int Cnt = 0, idx = 0, height = 1;

	while (depth[idx + 1] != 0)
	{
		if (depth[idx] > depth[idx + 1])
			Cnt++;
		else if (depth[idx] == 1 && depth[idx + 1] == 1)
			Cnt++;
		else if (idx == 0 && depth[idx] == 1)
			Cnt++;
		idx++;
	}

	int* cntArr = new int[Cnt + 2];
	memset(cntArr, 0, (Cnt + 2) * sizeof(int));
	cntArr[0] = Cnt;
	idx = 0, Cnt = 0;

	while (depth[idx + 1] != 0)
	{
		if (depth[idx] > depth[idx + 1])
		{
			cntArr[2 + Cnt] = idx + 1;

			Cnt++;

			if (height < depth[idx])
				height = depth[idx];
		}
		else if (depth[idx] == 1 && depth[idx + 1] == 1)
		{
			cntArr[2 + Cnt] = idx + 1;
			Cnt++;
		}
		else if (idx == 0 && depth[idx] == 1)
		{
			cntArr[2 + Cnt] = idx;
			Cnt++;
		}

		idx++;
	}

	cntArr[1] = height;

	return cntArr;
}

int main()
{
int T, N, var1, var2;
	int* arrTree, *arrDepth, *arrReach;		// 트리를 서브트리 단위로 쪼개서 저장할 배열
	cin >> T;

	for (int i = 0; i < T; i++)
	{
		Tree tr(1);

		cin >> N;

		arrTree = new int[N]();
		arrDepth = new int[N]();
		memset(arrTree, 0, N * sizeof(int));
		memset(arrDepth, 0, N * sizeof(int));

		for (int j = 0; j < N; j++)
		{
			cin >> var1;
			arrTree[j] = var1;
		}

		for (int j = 0; j < N; j++)
		{
			cin >> var1;
			arrDepth[j] = var1;
		}

		arrReach = getReach(arrDepth, 1);


		/*
		for (int j = 0; j < N; j++)
		{
			cout << arrTree[j] << " ";
		}
		cout << endl;
		for (int j = 0; j < N; j++)
		{
			cout << arrDepth[j] << " ";
		}
		cout << endl;
		for (int j = 0; j < arrReach[0] + 2; j++)
		{
			cout << arrReach[j] << " ";
		}
						*/


		for (int k = 1; k <= arrReach[1]; k++)
		{
			for (int j = 0; j < arrReach[0]; j++)	// 부모 노드 싹다 입력
			{
				if (arrDepth[arrReach[j + 2]] == k)
				{
					if (k == 1)
						tr.insertNode(1, arrTree[arrReach[j + 2]]);
					else
					{
						int temp = 0;
						bool done = false;
						while (!done)
						{
							if (arrDepth[arrReach[(j + 2) + temp]] == k - 1)
							{
								tr.insertNode(arrTree[arrReach[(j + 2) + temp]], arrTree[arrReach[j + 2]]);
								done = true;
							}
							temp++;
						}
					}
				}
			}
		}

		// 나머지 leaf 노드 입력

		int temp = 0;
		static int p = 0;

		while (temp != arrReach[0])
		{
			for (p; p < N; p++)
			{
				if (p == arrReach[temp + 2])
				{ 
					temp++;
					p++;
					break;
				}
				else
				{
					tr.insertNode(arrTree[arrReach[temp + 2]], arrTree[p]);
				}
			}
		}
		
		p = 0;
		

		tr.preorder(tr.root);
		cout << endl;

		delete[] arrTree;
		delete[] arrDepth;
		delete[] arrReach;

	}

	return 0;
}
