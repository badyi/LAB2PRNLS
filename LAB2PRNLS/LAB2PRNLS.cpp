#include <iostream>
#include <List>
#include <string>
using namespace std;

template <typename T>
class OptimalTree {
public:

    class node {
    public:
        //node* parent;
        int parentIndex = -1;
        //node* left;
        int leftIndex = -1;
        //node* right;
        int rightIndex = -1;
        int h;

        T value;
    };

    list<node*> list;
    int maxHeight = 0;

public:
    OptimalTree() {
    }

    void push(T value) {
        node* n = new node();
        n->value = value;
        //todo optimization by weight
        if (list.size() == 0) {
            list.push_back(n);
            maxHeight = 1;
        } else {
            push(n, root(), 0);
        }
    }

    node* root() {
        return list.front();
    }

    void push(node* n, node* parent, int h) {
        h+=1;
        if (n->value < parent->value) {
            if (parent->leftIndex == -1) {
                n->parentIndex = list.size() - 1;
                parent->leftIndex = list.size();
                n->h = h;
                list.push_back(n);
            }
            else {
                push(n, get(parent->leftIndex), h);
            }
        }
        else if (n->value > parent->value) {
            if (parent->rightIndex == -1) {
                n->parentIndex = list.size() - 1;
                parent->rightIndex = list.size();
                n->h = h;
                list.push_back(n);
            }
            else {
                push(n, get(parent->rightIndex), h);
            }
        }
        if (h > maxHeight) {
            maxHeight = h;
        }
    }

    node* get(const int index) {
        int i = 0;
        node* n = new node();
        for (typename std::list<node*>::iterator it = list.begin();; it++) {
            n = *it;
            if (i == index) {
                break;
            }
            if (it == list.end())
                break;
            i++;
        }
        return n;
    }

    void print() {
        if (!empty())
            print(root());
    }

    bool empty() {
        return list.size() == 0;
    }

    void print(node* n) {
        if (n->leftIndex != -1) {
            print(get(n->leftIndex));
        }
        cout << "(value:" << n->value << ", height:" << n->h << ")" << endl;
        if (n->rightIndex != -1) {
            print(get(n->rightIndex));
        }
    }

    void printListValues() {
        for (const auto i: list) {
            cout << i->value;
        }
    }
};

int main()
{
    OptimalTree<int> tree = OptimalTree<int>();

    tree.push(5);
    tree.push(6);
    tree.push(7);
    tree.push(8);
    tree.push(5);
    tree.push(1);
    tree.push(3); 
    tree.push(2);
    tree.push(4);
    tree.print();
    return 0;
}