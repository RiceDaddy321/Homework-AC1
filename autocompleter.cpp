#include "autocompleter.h"

Autocompleter::Autocompleter()
{
    root = nullptr;
}

void Autocompleter::insert(string x, int freq)
{
    //step 1: call the recursive helper method
    Entry * babyEntry = new Entry;
    babyEntry->s = x;
    babyEntry->freq = freq;

    //passing babyEntry by value
    insert_recurse(*babyEntry, root);

    delete babyEntry;
}

int Autocompleter::size()
{
    size_recurse(root);
}

void Autocompleter::completions(string x, vector<string> &T)
{

}

int Autocompleter::size_recurse(Node* p)
{
    if(p == nullptr)
    {
        return 0;
    }
    else
    {
        return 1 + size_recurse(p->left) + size_recurse(p->right);
    }
}

void Autocompleter::completions_recurse(string x, Node* p, vector<Entry> &C)
{

}

void Autocompleter::insert_recurse(Entry e, Node* &p)
{
    
}

void Autocompleter::rebalance(Node* &p)
{

}

void Autocompleter::right_rotate(Node* &p)
{
    //setup the variables
    Node* a = p;
    Node* b = a->right;
    Node* bl = b->left;

    //do the switcharoo
    b->left = a;
    a->right = bl;
    p = b;
}
void Autocompleter::left_rotate(Node* &p)
{

}