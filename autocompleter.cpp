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
    return size_recurse(root);
}
void Autocompleter::completions(string x, vector<string> &T)
{
    vector<Entry> top_three_completions;
    completions_recurse(x, root, top_three_completions);

    for (Entry completion : top_three_completions)
        T.push_back(completion.s);
}
int Autocompleter::height()
{
    return height(root);
};
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
        update_top_trends(p);
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
        update_top_trends(p);
        rebalance(p);
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
void Autocompleter::update_height(Node*& p)
{
    if (p != nullptr)
    {
        p->height = 1 + max(height(p->left), height(p->right));
    }
};
void Autocompleter::update_top_trends(Node* p)
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
};