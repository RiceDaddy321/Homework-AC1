#ifndef AUTOCOMPLETER_H
#define AUTOCOMPLETER_H

#include <vector>
#include <string>
using namespace std;

class Autocompleter
{
	// For the mandatory running times below:
	// n is the number of strings in the dictionary.
	// Assume that the length of every string is O(1).

public:
	// Creates a new Autocompleter with an empty dictionary.
	//
	// Must run in O(1) time.
	Autocompleter();

	// Adds a string x to the dictionary.
	// If x is already in the dictionary, does nothing.
	//
	// Must run in O(log(n)) time.
	void insert(string x, int freq);

	// Returns the number of strings in the dictionary
	// of possible completions.
	//
	// Must run in O(n) time.
	int size();

	// Fills the vector T with the three most-frequent completions of x.
	// If x has less than three completions, then
	// T is filled with all completions of x.
	// The completions appear in T from most to least frequent.
	//
	// Must run fast.  In particular, you should not search all nodes in the
	// tree for possible completions.
	// Instead, only search regions of the tree for which a completion could
	// be present, which will yield a run time bound of O(k log n ) time,
	// where k is the number of completions in the tree.
	void completions(string x, vector<string> &T);

	//Reports height of the AVL tree, runs in O(1) time.
	int height()
	{
		return height(root);
	}

private:
	// A helper class that stores a string and a frequency.
	class Entry
	{
	public:
		string s;
		int freq;
	};

	// A helper class that implements a binary search tree node.
	class Node
	{
	public:
		Node()
		{
			height = 0;
			left = right = nullptr;
		}

		Node(Entry e)
		{
			this->e = e;
			height = 0;
			left = right = nullptr;
		}

		Entry e;
		int height;
		Node* left;
		Node* right;
		vector<Entry> top_three;
	};

	// A convenience method for getting the height of a subtree.
	// Returns the height of the subtree rooted at p.
	// Useful for rebalance().
	static int height(Node* p)
	{
		if (p == nullptr)
			return -1;
		return p->height;
	}

	// Root of the binary-search-tree-based data structure
	Node* root;


	// Optional helper methods (you'll probably want them)

	// Returns the size of the binary tree rooted at p.
	//
	// Should run in O(n) time.
	int size_recurse(Node* p);

	// Fills C with the completions of x in the BST rooted at p.
	void completions_recurse(string x, Node* p, vector<Entry> &C);

	// Inserts an Entry into an AVL tree rooted at p.
	//
	// Should run in O(log(n)) time.
	void insert_recurse(Entry e, Node* &p);

	// Rebalances the AVL tree rooted at p.
	// Helpful for insert().
	// Should be called on every node visited during
	// the search in reverse search order.
	//
	// Should run in O(1) time.
	void rebalance(Node* &p);

	// Perform left and right rotations
	// of an AVL tree rooted at p (helpful for implementing rebalance).
	//
	// Should run in O(1) time.
	void right_rotate(Node* &p);
	void left_rotate(Node* &p);


	//A useful method to update
	//the height of a node,
	//assuming subtrees already have
	//the correct height.
	void update_height(Node*& p)
	{
		if (p != nullptr)
			p->height = 1 + max(height(p->left), height(p->right));
	}

	void update_top_trends(Node* p)
	{
		if (p != nullptr)
		{
			//let's look at both sides and compare their top trends
			int right_cursor = 0;
			int left_cursor = 0;

			p->top_three.clear();

			if (p->left == nullptr && p->right == nullptr)
			{
				p->top_three.push_back(p->e);
				return;
			}

			//check if left is empty
			if (p->left == nullptr)
			{
				p->top_three = p->right->top_three;
				return;
			}
			//check if right is empty
			if (p->right == nullptr)
			{
				p->top_three = p->left->top_three;
				return;
			}

			//fills up the top_three
			while (p->top_three.size() < 3 && p->right->top_three.size() > right_cursor && p->left->top_three.size() > left_cursor)
			{
				if (p->right->top_three[right_cursor].freq > p->left->top_three[left_cursor].freq)
				{
					p->top_three.push_back(p->right->top_three[right_cursor]);
					right_cursor++;

					//check if the right is empty
					if (right_cursor == p->right->top_three.size())
						for (int i = 0; i < p->left->top_three.size(); i++)
							p->top_three.push_back(p->left->top_three[i]);
				}
				else
				{
					p->top_three.push_back(p->left->top_three[left_cursor]);
					left_cursor++;

					//check if the left is empty
					if (left_cursor == p->left->top_three.size())
						for (int i = 0; i < p->right->top_three.size(); i++)
							p->top_three.push_back(p->right->top_three[i]);
				}
			}
		}
	}
};

#endif