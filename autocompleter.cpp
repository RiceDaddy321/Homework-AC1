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
    return size_recurse(root);
}

void Autocompleter::completions(string x, vector<string> &T)
{
    vector<Entry> top_three_completions;
    completions_recurse(x, root, top_three_completions);

    for (Entry completion : top_three_completions)
        T.push_back(completion.s);
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
    if(p == nullptr)
    {
        return; //do nothing
    }
    if(p->e.s.substr(0, x.size()) == x) //the current word matches has x as a prefix
    {
        C = p->top_three;
    }
    else //still not found
    {
        if(p->e.s.substr(0, x.size()) < x) //go to the right
            completions_recurse(x, p->right, C);
        else
            completions_recurse(x, p->left, C);
    }
    
}

void Autocompleter::insert_recurse(Entry e, Node* &p)
{
    if(p == nullptr) // only worry about inserting
    {
        p = new Node;
        p->e = e;
        p->top_three.push_back(e);
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
        //for the case that we don't need to rebalance
        update_top_trends(p);
    }
}

void Autocompleter::rebalance(Node* &p)
{
    //check for Right rotations
    //happens when the left is bigger than right
    if((height(p->left) - height(p->right)) > 1)
    {
        //check for left-right rotation
        if((height(p->left->right) - height(p->left->left)) > 1)
            left_rotate(p->left);

        right_rotate(p);
            
    }
    //check for left rotation
    //happens when right > left
    else if((height(p->right) - height(p->left)) > 1)
    {
        //check for right-left rotation
        if((height(p->right->left) - height(p->right->right)) > 1)
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

    //fix the top_trends
    update_top_trends(a);
    update_top_trends(b);
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

    //fix the top_trends
    update_top_trends(a);
    update_top_trends(b);
}