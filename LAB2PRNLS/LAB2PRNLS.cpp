#include <iostream>
#include <List>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;


enum printType {
    valueHeight,
    value,
    preOrder, //прямой
    inOrder, //симметричный 
    postOrder  //обратный
};

template <typename T>
class OptimalTree {
private:
    class node {
    public:
        int parentIndex = -1;
        int leftIndex = -1;
        int rightIndex = -1;
        int h;

        T value;
    };

    list<node*> _list;
    int _maxHeight = 0;

    void _push(T value) {
        node* n = new node();
        n->value = value;
        if (_list.size() == 0) {
            _list.push_back(n);
            _maxHeight = 1;
        }
        else {
            _push(n, _root(), 0);
        }
    }

    node* _root() {
        return _list.front();
    }

    void _push(node* n, node* parent, int h) {
        h += 1;
        if (n->value < parent->value) {
            if (parent->leftIndex == -1) {
                n->parentIndex = _list.size() - 1;
                parent->leftIndex = _list.size();
                n->h = h;
                _list.push_back(n);
            }
            else {
                _push(n, _get(parent->leftIndex), h);
            }
        }
        else if (n->value > parent->value) {
            if (parent->rightIndex == -1) {
                n->parentIndex = _list.size() - 1;
                parent->rightIndex = _list.size();
                n->h = h;
                _list.push_back(n);
            }
            else {
                _push(n, _get(parent->rightIndex), h);
            }
        }
        if (h > _maxHeight) {
            _maxHeight = h;
        }
    }

    node* _get(const int index) {
        int i = 0;
        node* n = new node();
        for (typename std::list<node*>::iterator it = _list.begin();; it++) {
            n = *it;
            if (i == index) {
                break;
            }
            if (it == _list.end())
                break;
            i++;
        }
        return n;
    }

    T* tempArr;
    int tempIndex = 0;
    void _getValues(node* n) {
        if (n->leftIndex != -1) {
            _getValues(_get(n->leftIndex));
        }
        tempArr[tempIndex] = n->value;
        tempIndex += 1;
        if (n->rightIndex != -1) {
           _getValues(_get(n->rightIndex));
        }
    }

    void _optimize() {
        tempIndex = 0;
        tempArr = new T[_list.size()];
        _getValues(_root());
        _createBalancedTree();
    }

    void _createBalancedTree() {
        int size = _list.size();
        _list.clear();
        _createBalancedTree(tempArr, 0, size - 1);
    }

    void _createBalancedTree(T* arr, int start, int end) {
        if (end < start)
            return;
        int mid = (start+end) / 2;
        T arrMid = arr[mid];
        
        _push(arrMid);
        _createBalancedTree(arr, start, mid - 1);
        _createBalancedTree(arr, mid + 1, end);
    }

public:
    OptimalTree() {
    }

    void push(T value) {
        _push(value);
        if (_list.size() > 3)
            _optimize();
    }

    void print(printType type) {
        if (!empty()) {
            switch (type) {
            case valueHeight:
                printH(_root());
                break;
            case inOrder:
                printInOrder(_root());
                break;
            case postOrder:
                printPostOrder(_root());
                break;
            default:
                return;
            }
        }
    }

    bool empty() {
        return _list.size() == 0;
    }

    void printH(node* n) { // print wiht heights
        if (n->leftIndex != -1) {
            printH(_get(n->leftIndex));
        }
        cout << "(value:" << n->value << ", height:" << n->h << ")" << endl;
        if (n->rightIndex != -1) {
            printH(_get(n->rightIndex));
        }
    }
     
    void printPreOrder(node* n) {
        cout << "{ " << n->value << " }";
        if (n->leftIndex != -1) {
            printPreOrder(_get(n->leftIndex));
        }
        if (n->rightIndex != -1) {
            printPreOrder(_get(n->rightIndex));
        }
    }

    void printInOrder(node* n){
        if (n->leftIndex != -1) {
            printInOrder(_get(n->leftIndex));
        }
        cout << "{ " << n->value << " }";
        if (n->rightIndex != -1) {
            printInOrder(_get(n->rightIndex));
        }
    }

    void printPostOrder(node* n){
        if (n->rightIndex != -1) {
            printPostOrder(_get(n->rightIndex));
        }
        cout << "{ " << n->value << " }";
        if (n->leftIndex != -1) {
            printPostOrder(_get(n->leftIndex));
        }
    }

    void printListValues() {
        for (const auto i: _list) {
            cout << i->value;
        }
    }

    void unionWith(OptimalTree<T> tree) { //прямым обходом tree
        preOrderWalk(tree._root(), tree);
    }

    void preOrderWalk(node* n, OptimalTree<T> tree) {
        push(n->value);
        if (n->leftIndex != -1) {
            preOrderWalk(tree._get(n->leftIndex), tree);
        }
        if (n->rightIndex != -1) {
            preOrderWalk(tree._get(n->rightIndex), tree);
        }
    }
};

template<typename C>
class Estimator {       //класс для оценки сортировок
private:
    C container1;        //структура данных
    C container2;
public:
    void createRandomValues(const int size, int maxPosibleValue) {  //заполнение массива для counting sort
        container1 = C();
        container2 = C();
        for (int i = 0; i < size; i++)
            container1.push(rand() % (maxPosibleValue + 1));
        for (int i = 0; i < size; i++)
            container2.push(rand() % (maxPosibleValue + 1));
    }

    double diffclock(clock_t clock1, clock_t clock2) {
        return double(clock1 - clock2);
    }

    void testUnion() {  //запуск сортирвоки и ввывод данных о выполнении
        printType t1 = postOrder;
        printType t2 = inOrder;
        cout << "A: ";
        container1.print(t1);
        cout << endl;
        cout << "B: ";
        container2.print(t2);
        cout << endl;
        container1.unionWith(container2); 
        cout << "A U B = Result A: ";
        container1.print(t1);
        cout << endl;
    }
};

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    Estimator<OptimalTree<int>> estimator = Estimator<OptimalTree<int>>();
    int maxValue = 50;
    int sizesOfTrees[] = { 5 }; //кол-во элементов на которых тестируются структуры
    int countOfTestings = sizeof(sizesOfTrees) / sizeof(*sizesOfTrees);

    for (int i = 0; i < countOfTestings; i++) {
        estimator.createRandomValues(sizesOfTrees[i], maxValue);
        estimator.testUnion();
    }

    return 0;
}