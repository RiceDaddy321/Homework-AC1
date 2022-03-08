#include "autocompleter.h"

/*
To - do:
Finish void::completions
Finish void::completions_recurse
Integrate to main.cpp

I'll work on these and check them against main.cpp to make sure they work.
-Miguel 
*/

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
    if(p == nullptr) // only worry about inserting
    {
        p = new Node;
        p->e = e;
    }
    else
    {
        //pick a side
        if(e.s < p->e.s)
            insert_recurse(e, p->left);
        else
            insert_recurse(e, p->right);

        //check if we screwed anything
        update_height(p);
        rebalance(p);
    }
}

void Autocompleter::rebalance(Node* &p)
{
    //check for Right rotations
    //happens when the left is bigger than right
    if((p->left->height - p->right->height) > 1)
    {
        //check for left-right rotation
        if((p->left->right->height - p->left->left->height) > 1)
            left_rotate(p->left);

        right_rotate(p);
            
    }
    //check for left rotation
    //happens when right > left
    else if((p->right->height - p->left->height) > 1)
    {
        //check for right-left rotation
        if((p->right->left->height - p->right->right->height) > 1)
            right_rotate(p->right);

        left_rotate(p);
    }

}

void Autocompleter::right_rotate(Node* &p)
{
    //setup the variables
    Node* a = p;
    Node* b = a->left;
    Node* br = b->right;

    //do the switcharoo
    p = b;
    b->right = a;
    a->left = br;

    //fix the heights
    update_height(a);
    update_height(b);    
}
void Autocompleter::left_rotate(Node* &p)
{
    //setup the variables
    Node* a = p;
    Node* b = p->right;
    Node* bl = b->left;

    //do the switch
    p = b;
    b->left = a;
    a->right = bl;

    //fix the heights
    update_height(a);
    update_height(b);
}