// Recursive CPP program for level
// order traversal of Binary Tree
#include <bits/stdc++.h>
#include <chrono>
#include <omp.h>
#include <cstdlib>
using namespace std;

// A binary tree node has data,
// pointer to left child
// and a pointer to right child
class node {
public:
	int data;
	node *left, *right;
};

// Function prototypes
void printCurrentLevelPARA(node* root, int level);
void printCurrentLevelSEQ(node* root, int level);
int height(node* node);
node* newNode(int data);

// Function to print level order traversal a tree
void printLevelOrderSEQ(node* root)
{
	int h = height(root);
	int i;
	for (i = 1; i <= h; i++)
		printCurrentLevelSEQ(root, i);
}

void printLevelOrderPARA(node* root)
{
	int h = height(root);
	int i;
	for (i = 1; i <= h; i++)
		printCurrentLevelPARA(root, i);
}

// Print nodes at a current level
void printCurrentLevelSEQ(node* root, int level)
{
	if (root == NULL)
		return;
	if (level == 1)
		cout << root->data << " ";
	else if (level > 1) {
		printCurrentLevelSEQ(root->left, level - 1);
		printCurrentLevelSEQ(root->right, level - 1);
	}
}

//Parallel BFS OpenMp
void printCurrentLevelPARA(node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        cout << root->data << " ";
    else if (level > 1) {
        #pragma omp task
        printCurrentLevelPARA(root->left, level - 1);
        
        #pragma omp task
        printCurrentLevelPARA(root->right, level - 1);
        
        #pragma omp taskwait
    }
}



// Compute the "height" of a tree -- the number of
// nodes along the longest path from the root node
// down to the farthest leaf node.
int height(node* node)
{
	if (node == NULL)
		return 0;
	else {
		
		// Compute the height of each subtree
		int lheight = height(node->left);
		int rheight = height(node->right);

		// Use the larger one
		if (lheight > rheight) {
			return (lheight + 1);
		}
		else {
			return (rheight + 1);
		}
	}
}

// Helper function that allocates
// a new node with the given data and
// NULL left and right pointers.
node* newNode(int data)
{
	node* Node = new node();
	Node->data = data;
	Node->left = NULL;
	Node->right = NULL;

	return (Node);
}

// Driver code
int main()
{
	node* root = newNode(1);
	root->left = newNode(2);
	root->right = newNode(3);
	root->left->left = newNode(4);
	root->left->right = newNode(5);

	auto startSeq = chrono::steady_clock::now();
	cout << "Level Order traversal of binary tree is (Sequential)\n";
	printLevelOrderSEQ(root);
	cout<<endl;
    	auto endSeq = chrono::steady_clock::now();
    	cout << "Sequential Elapsed time for BFS(SEQ) in microseconds: " << chrono::duration_cast<chrono::microseconds>(endSeq - startSeq).count() << " microsecs" << endl;

	
	
	auto startPara = chrono::steady_clock::now();
	cout << "Level Order traversal of binary tree is (Parallel)\n";
	printLevelOrderPARA(root);
	cout<<endl;
	auto endPara = chrono::steady_clock::now();
	cout << "Sequential Elapsed time for BFS(PARA) in microseconds: " << chrono::duration_cast<chrono::microseconds>(endPara - startPara).count() << " microsecs" << endl;
	
	float speedup = (chrono::duration_cast<chrono::microseconds>(endSeq - startSeq).count() / chrono::duration_cast<chrono::microseconds>(endPara - startPara).count());
	cout<<"SpeedUp = " <<speedup<<endl;
	
	return 0;
}




