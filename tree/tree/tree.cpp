#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;


enum Color { BLACK, RED };

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;
    Node* Parent;
};

struct trunk {
    string str;
    trunk* prev;
    trunk(trunk* prev, string str) : prev(prev), str(str) {}
};

Node* searchSibling(Node*& node);

void rotateLeft(Node*& root, Node* node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->Parent = node;
    }
    rightChild->Parent = node->Parent;
    if (node->Parent == nullptr) {
        root = rightChild;
        rightChild->color = BLACK;
    }
    else if (node == node->Parent->left) {
        node->Parent->left = rightChild;
    }
    else {
        node->Parent->right = rightChild;
    }
    rightChild->left = node;
    node->Parent = rightChild;
}



void rotateRight(Node*& root, Node* node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->Parent = node;
    }
    leftChild->Parent = node->Parent;
    if (node->Parent == nullptr) {
        root = leftChild;
    }
    else if (node == node->Parent->right) {
        node->Parent->right = leftChild;
    }
    else {
        node->Parent->left = leftChild;
    }
    leftChild->right = node;
    node->Parent = leftChild;
}


void fixTreeInsert(Node*& root, Node* node) {
    while (node->Parent != nullptr && node->Parent->color == RED) {
        if (node->Parent == node->Parent->Parent->left) {
            Node* uncle = node->Parent->Parent->right;
            if (uncle != nullptr && uncle->color == RED) {
                node->Parent->color = BLACK;
                uncle->color = BLACK;
                node->Parent->Parent->color = RED;
                node = node->Parent->Parent;
            }
            else {
                if (node == node->Parent->right) {
                    node = node->Parent;
                    rotateLeft(root, node);
                }
                node->Parent->color = BLACK;
                node->Parent->Parent->color = RED;
                rotateRight(root, node->Parent->Parent);
            }
        }
        else {
            Node* y = node->Parent->Parent->left;
            if (y != nullptr && y->color == RED) {
                node->Parent->color = BLACK;
                y->color = BLACK;
                node->Parent->Parent->color = RED;
                node = node->Parent->Parent;
            }
            else {
                if (node == node->Parent->left) {
                    node = node->Parent;
                    rotateRight(root, node);
                }
                node->Parent->color = BLACK;
                node->Parent->Parent->color = RED;
                rotateLeft(root, node->Parent->Parent);
            }
        }
    }
    root->color = BLACK;
}

void insert(Node*& root, int key) {
    Node* newNode = new Node;
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->color = RED;

    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr) {
        parent = current;
        if (newNode->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    newNode->Parent = parent;
    if (parent == nullptr) {
        root = newNode;
    }
    else if (newNode->key < parent->key) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }
    fixTreeInsert(root, newNode);
}

Node* getMin(Node*& node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node* getMax(Node*& node) {
    while (node->right) {
        node = node->right;
    }
    return node;
}

Node* searchSibling(Node*& node)
{
    if (node && node->Parent)
    {
        if (node == node->Parent->left)
            return node->Parent->right;
        else
            return node->Parent->left;
    }
    return NULL;
}

void deleteTree(Node*& root)
{
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void preOrderTraversal(Node*& root)
{
    if (root)
    {
        cout << root->key << " ";
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

void inOrderTraversal(Node* root)
{
    if (root)
    {
        inOrderTraversal(root->left);
        cout << root->key << " ";
        inOrderTraversal(root->right);
    }
}


void showTrunkConsole(trunk* p, int& count) {
    if (p != nullptr) {
        showTrunkConsole(p->prev, count);
        count++;
        cout << p->str;
    }
}

void printTreeConsole(Node*& tree, trunk* prev, bool isRight) {
    if (tree != nullptr) {
        string prevStr = "    ";
        trunk* temp = new trunk(prev, prevStr);
        if (tree->right != nullptr) {
            printTreeConsole(tree->right, temp, 1);
        }

        if (!prev) {
            temp->str = "-->";
        }
        else if (isRight) {
            temp->str = ".-->";
            prevStr = "   |";
        }
        else {
            temp->str = "`-->";
            prev->str = prevStr;
        }

        int count = 0;
        showTrunkConsole(temp, count);
        if (tree->color == RED) {
            cout << "(КР)";
        }
        else {
            cout << "(Ч)";
        }
        cout << tree->key << endl;
        if (prev) {
            prev->str = prevStr;
        }
        temp->str = "   |";
        if (tree->left != nullptr) {
            printTreeConsole(tree->left, temp, 0);
        }
        delete temp;
    }
    else {
        return;
    }
}

Node* createRandomRBTree(int size) {
    srand(time(nullptr));
    Node* root = nullptr;
    if (size == 0) {
        cout << "Дерево пустое!";
        return root;
    }
    else {
        for (int i = 0; i < size; i++) {
            int key = rand() % 199 - 99;
            insert(root, key);
        }
    }
    return root;
}

Node* createArrayRBTree(int size, int*& array) {
    Node* root = nullptr;
    if (size == 0) {
        cout << "Дерево пустое!";
        return root;
    }
    else {
        for (int i = 0; i < size; i++) {
            int key = array[i];
            insert(root, key);
        }
    }
    return root;
}

Node* createArray() {
    string inputData;
    string str;
    cout << "Введите узлы: \n";
    cin.ignore();
    getline(cin, inputData);
    int size = 1;
    for (int i = 0; i < inputData.size(); i++) {
        if (inputData[i] == ' ') {
            size++;
        }
    }

    int* array = new int[size];
    str = "";
    int localIndex = 0;
    for (int i = 0; i < inputData.size(); i++) {
        if (inputData[i] != ' ') {
            str.push_back(inputData[i]);
        }
        else {
            array[localIndex] = std::stoi(str);
            localIndex++;
            str = "";
        }
    }
    if (!str.empty()) {
        array[localIndex] = std::stoi(str);
    }
    Node* tree = createArrayRBTree(size, array);
    return tree;
}

Node* getKey(Node*& root, int key)
{
    if (root)
    {
        if (key == root->key) {
            cout << "Элемент найден!\n";
            return root;
        }
        if (key < root->key)
            return getKey(root->left, key);
        else
            return getKey(root->right, key);
    }
    else {
        cout << "Элемент не найден!\n";
        return nullptr;
    }
}

Node* findNode(Node*& root, int data)
{
    if (!root || root->key == data) {
        return root;
    }
    if (root->key < data)
        return findNode(root->right, data);
    else
        return findNode(root->left, data);
}


Node* rotateToLeft(Node* node) {
    Node* right = node->right;
    Node* rightLeft = right->left;
    right->left = node;
    node->right = rightLeft;
    return right;
}

Node* rotateToRight(Node* node) {
    Node* left = node->left;
    Node* leftRight = left->right;
    left->right = node;
    node->left = leftRight;
    return left;
}

bool balanceRemoveLeft(Node** root) {
    Node* node = *root;
    Node* left = node->left;
    Node* right = node->right;
    Node* rightLeft = right->left;
    Node* rightRight = right->right;

    if (left and left->color == RED) { // если была убрана красная вершина
        left->color = BLACK;
        return false;
    }
    if (right and right->color == RED) { // если удалённая вершина имеет красного брата
        node->color = RED;
        right->color = BLACK;
        node = *root = rotateToLeft(node);
        if (balanceRemoveLeft(&node->left)) {
            node->left->color = BLACK;
        }
        return false;
    }

    if ((!rightLeft or !rightLeft->color) and (!rightRight or !rightRight->color)) { // если удалённая вершина имеет чёрного брата (2)
        right->color = RED;
        return true;
    }

    if (rightLeft and rightLeft->color == RED) { // если удалённая вершина имеет чёрного брата, у которого красный левый (4)
        right->color = RED;
        rightLeft->color = BLACK;
        right = node->right = rotateToRight(right);
        rightRight = right->right;
    }

    if (rightRight and rightRight->color == RED) { // если удалённая вершина имеет чёрного брата, у которого красный правый 
        right->color = node->color;
        rightRight->color = node->color = BLACK;
        *root = rotateToLeft(node);
    }

    return false;
}

bool balanceRemoveRight(Node** root) {
    Node* node = *root;
    Node* left = node->left;
    Node* right = node->right;
    Node* leftLeft = left->left;
    Node* leftRight = left->right;

    if (right and right->color) {
        right->color = BLACK;
        return false;
    }

    if (left and left->color) {
        node->color = RED;
        left->color = BLACK;
        node = *root = rotateToRight(node);
        if (balanceRemoveRight(&node->right)) {
            node->right->color = BLACK;
        }
        return false;
    }

    if ((!leftRight or leftRight->color == BLACK) and (!leftLeft or leftLeft->color == BLACK)) {
        left->color = RED;
        return true;
    }

    if (leftRight and leftRight->color == RED) {
        left->color = RED;
        leftRight->color = BLACK;
        left = node->left = rotateToLeft(left);
        leftLeft = left->left;
    }

    if (leftLeft and leftLeft->color) {
        left->color = node->color;
        leftLeft->color = node->color = BLACK;
        *root = rotateToRight(node);
    }

    return false;
}

bool getColorMinRight(Node** rightDeleteNode, Node** deleteNode) {
    Node* node = *rightDeleteNode;
    if (node->left) {
        if (getColorMinRight(&node->left, deleteNode)) {
            return balanceRemoveLeft(rightDeleteNode);
        }
    }
    else {
        *rightDeleteNode = node->right;
        *deleteNode = node;
        return !node->color;
    }
    return false;
}

bool remove22(int value, Node** root) {
    Node* tempNode, * node = *root;
    if (node) {
        if (value > node->key) {
            if (remove22(value, &node->right)) {
                return balanceRemoveRight(root);
            }
        }
        else if (value < node->key) {
            if (remove22(value, &node->left)) {
                return balanceRemoveLeft(root);
            }
        }
        else {
            bool color; // false
            if (!node->right) { // rightMin
                *root = node->left; // нет right, берем оставшийся
                color = !node->color;
                delete node;
                return color;
            }
            else {
                color = getColorMinRight(&node->right, root); // случай с 2 детьми 
                tempNode = *root;
                tempNode->color = node->color;
                tempNode->left = node->left;
                tempNode->right = node->right;
                delete node;
                if (color) { // по-умолчанию красный - балансируем
                    color = balanceRemoveRight(root);
                }
                return color;
            }
        }
        return false;
    }
    else {
        return false;
    }
}


void menu(Node*& tree, trunk*& prev, bool isRight, Node* delNode);

void menuActions(Node*& tree, trunk*& prev, bool isRight, Node* delNode)
{
    srand(time(NULL));
    int picker;
    {
        cout << "Что делать?\n"
            << "1. Вставка\n"
            << "2. Удаление\n"
            << "3. Получение\n"
            << "4. Повторить\n"
            << "0. Выход\n";
    }
    cin >> picker;

    switch (picker)
    {
    case 1: {
        cout << "\nВведите число для вставки: ";
        int keyValueToAdd;
        cin >> keyValueToAdd;

        auto startInsertTime = steady_clock::now();
        insert(tree, keyValueToAdd);
        auto endInsertTime = steady_clock::now();

        cout << "\n\n";
        printTreeConsole(tree, prev, isRight);
        cout << "\n\tВремя вставки: " << duration_cast<nanoseconds>(endInsertTime - startInsertTime).count() << " наносекунд\n";


        system("Pause");

        menuActions(tree, prev, isRight, delNode);
        break;
    }
    case 2: {
        cout << "Введите число для удаления: ";
        int keyValueToDelete22;
        cin >> keyValueToDelete22;

        auto startDeleteTime = steady_clock::now();
        remove22(keyValueToDelete22, &tree);
        auto endDeleteTime = steady_clock::now();

        printTreeConsole(tree, prev, isRight);
        cout << "\n\tВремя удаления: " << duration_cast<nanoseconds>(endDeleteTime - startDeleteTime).count() << " наносекунд\n";
        system("Pause");
        menuActions(tree, prev, isRight, delNode);
        break;
    }
    case 3: {
        cout << "Введите число для нахождения: ";
        int keyValueToGet;
        cin >> keyValueToGet;

        auto startFindTime = steady_clock::now();
        getKey(tree, keyValueToGet);
        auto endFindTime = steady_clock::now();

        printTreeConsole(tree, prev, isRight);
        cout << "\n\tВремя нахождения: " << duration_cast<nanoseconds>(endFindTime - startFindTime).count() << " наносекунд\n";
        system("Pause");
        menuActions(tree, prev, isRight, delNode);
        break;
    }
    case 4:
        system("Pause");
        system("cls");
        menu(tree, prev, isRight, delNode);
        break;
    case 0:
        deleteTree(tree);
        system("cls");
        break;
    default:
        cout << "Так нельзя!\n";
        system("Pause");
        system("cls");
        menuActions(tree, prev, isRight, delNode);
        break;
    }

}

void menu(Node*& tree, trunk*& prev, bool isRight, Node* delNode)
{
    int mainMenu;
    {
        cout << "Что будем делать?\n"
            << "1. Создание дерева по размеру\n"
            << "2. Создание дерева по элементам\n";
    }
    cin >> mainMenu;

    switch (mainMenu)
    {
    case 1: {
        system("cls");
        int size;
        cout << "Введите размер дерева: ";
        cin >> size;

        auto startRandomCreatTime = steady_clock::now();
        tree = createRandomRBTree(size);
        auto endRandomCreatTime = steady_clock::now();

        cout << "\nСимметричный обход:\n";
        inOrderTraversal(tree);
        cout << "\nПрямой обход:\n";
        preOrderTraversal(tree);

        cout << "\n\n";
        cout << "Красно-чёрное дерево:\n\n";
        printTreeConsole(tree, prev, isRight);

        cout << "\n\n";
        cout << "\n\tВремя создания: " << duration_cast<nanoseconds>(endRandomCreatTime - startRandomCreatTime).count() << " наносекунд\n";
        system("Pause");
        menuActions(tree, prev, isRight, delNode);
        break;
    }
    case 2: {
        system("cls");

        tree = createArray();

        cout << "\nПрямой обход:\n";
        preOrderTraversal(tree);
        cout << "\nКрасно-чёрное дерево:\n\n";
        printTreeConsole(tree, prev, isRight);
        cout << "\n\n";

        system("Pause");
        menuActions(tree, prev, isRight, delNode);;
        break;
    }
    default:
        cout << "Так нельзя!\n";
        system("Pause");
        system("cls");
        menu(tree, prev, isRight, delNode);
        break;
    }

}

int main()
{
    Node* tree = nullptr;
    trunk* prev = nullptr;
    Node* delNode = tree;
    bool isRight = false;

    menu(tree, prev, isRight, delNode);


    return 0;
}
