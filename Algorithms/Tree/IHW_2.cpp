#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
 
using namespace std;
 
struct StudentGroup {
    char grade;
    int smthAndYear;
 
    StudentGroup(string name) : grade(name[0]), smthAndYear(stoi(name.substr(1, 4))) {}

    bool operator < (const StudentGroup& other){
        return grade < other.grade || (grade == other.grade && smthAndYear < other.smthAndYear);
    }

    bool operator > (const StudentGroup& other){
        return grade > other.grade || (grade == other.grade && smthAndYear > other.smthAndYear);
    }

    bool operator == (const StudentGroup& other){
        return grade == other.grade && smthAndYear == other.smthAndYear;
    }
};
 
struct NodeList {
    int data;
    NodeList* prev;
    NodeList* next;
};
 
struct NodeTree {
    StudentGroup key;
    NodeTree* left;
    NodeTree* right;
    NodeList* list;
 
    NodeTree(StudentGroup value) : key(value), left(nullptr), right(nullptr), list(nullptr) {}
};
 
NodeList* createNode(int value) {
    NodeList* newNode = new NodeList;
    newNode->data = value;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

bool findNodeList(NodeList* head, int value){
    NodeList* current = head;
    do{
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }while(current != head);
    return false;
}
 
void deleteChosenElement(NodeList*& head, int value) {
    NodeList* current = head;

    if(current == nullptr)
        return;
    
    do{
        if(current->next == current && current->data == value){
            delete head;
            head = nullptr;
            current = nullptr;
            return;
        }
        else if(current->data == value){
            bool isHead = false;
            if(current == head)
                isHead = true;
            NodeList* delEl = current;
            current = current->next;
            delEl->prev->next = current;
            current->prev = delEl->prev;

            if (isHead) {
                head = current;
            }

            delete delEl;
            delEl = nullptr;
        }
        else{
            current = current->next;
        }
    }while(current != head);
}
 
 
 
void appendToBegining(NodeList*& head, int value) { 
    NodeList* newNode = createNode(value);
    if (!head) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        NodeList* tail = head->prev;
        newNode->next = head;
        newNode->prev = tail;
        head->prev = newNode;
        tail->next = newNode;
        head = newNode;
    }
}
 
NodeTree* insert(NodeTree*& root, StudentGroup value, int lineNumber) {
    if (root == nullptr) {
        NodeTree* newNode = new NodeTree(value);
        appendToBegining(newNode->list, lineNumber);
        return newNode;
    }
 
    if ((value.grade < root->key.grade) ||
        (value.grade == root->key.grade && value.smthAndYear < root->key.smthAndYear)) {
        root->left = insert(root->left, value, lineNumber);
    } else if ((value.grade > root->key.grade) ||
        (value.grade == root->key.grade && value.smthAndYear > root->key.smthAndYear)) {
        root->right = insert(root->right, value, lineNumber);
    } else {
        appendToBegining(root->list, lineNumber);
    }
 
    return root;
}
 
// Для удаления 
void deleteList(NodeList*& head) {
    if (head != nullptr) {
        NodeList* current = head;
        NodeList* next;
        do {
            NodeList* temp = current;
            current = current->next;
            delete temp;
        } while (current != head);
        head = nullptr;
    }
}
 
void clearNode(NodeList*& head) {
    NodeList* current = head;
 
    if (!head) {
        std::cout << "Список пуст. Чистить нечего." << endl;
        return;
    }
 
    do {
        NodeList* temp = current;
        current = current->next;
        delete temp;
    } while (current != head);
}

void delMinRight(NodeTree*& root, NodeTree*& q){
    if(root->left != nullptr){
        delMinRight(root->left, q);
        } else{
            q->key = root->key;
            q->list = root->list;
            root = root->right;
    }
}

void remove(NodeTree*& root, StudentGroup value, int lineNumber) {
    if (root == nullptr) {
        return;
    }
 
    if (value < root->key) {
        remove(root->left, value, lineNumber);
    } else if (value > root->key) {
        remove(root->right, value, lineNumber);
    } 
    
    if(findNodeList(root->list, lineNumber)){
        // Если только один элемент в списке
        if(root->list->next != root->list){
            deleteChosenElement(root->list, lineNumber);
        }
        else{
            // Если более одного, удаляет номер строки
            if(root->left == nullptr){
                root = root->right;
            }
            else if(root->right == nullptr){
                root = root->left;
            }
            else{
                NodeTree* q = root;
                delMinRight(q->right, q);
            }
        }
    }
}
 
void printNodeList(NodeList* head) {
    if (head) {
        NodeList* current = head;
 
        do {
            cout << current->data << " ";
            current = current->next;
        } while (current != head);
 
    } else {
        cout << "Список пуст.";
        return;
    }
}
 
void printAllNodeLists(NodeTree* root) {
    if (root != nullptr) {
        cout << root->key.grade << setw(4) << setfill('0') << root->key.smthAndYear << ": ";
        printNodeList(root->list);
        cout << endl;
 
        printAllNodeLists(root->left);
        printAllNodeLists(root->right);
    }
}
 
void symmetricTraversalToFile(NodeTree* root, ofstream& outFile) {
    if (root != nullptr) {
        symmetricTraversalToFile(root->left, outFile);
 
        NodeList* current = root->list;
        do {
            outFile << root->key.grade << setw(4) << setfill('0') << root->key.smthAndYear << endl;
            current = current->next;
        } while (current != root->list);
 
        outFile << setw(0) << setfill(' ');
 
        symmetricTraversalToFile(root->right, outFile);
    }
}
 
NodeList* searchKey(NodeTree* root, StudentGroup value) {
    if (root == nullptr || (root->key.grade == value.grade && root->key.smthAndYear == value.smthAndYear)) {
        if (root != nullptr) {
            if (root->list != nullptr) {
                cout << "Ключ найден. " << root->key.grade << setw(4) << setfill('0') << root->key.smthAndYear << ": ";
                printNodeList(root->list);
                cout << endl;
            } else {
                cout << "Ключ найден. " << root->key.grade << setw(4) << setfill('0') << root->key.smthAndYear << ": Список пуст" << endl;
            }
            return root->list;
        } else {
            cout << "Ключ не найден." << endl;
            return nullptr;
        }
    }
 
    if (value.grade < root->key.grade || (value.grade == root->key.grade && value.smthAndYear < root->key.smthAndYear)) {
        return searchKey(root->left, value);
    } else {
        return searchKey(root->right, value);
    }
}
 
void draw(NodeTree* root, int h) {
    if (root != nullptr) {
        draw(root->right, h + 4);
 
        for (int i = 1; i <= h; i++) {
            cout << " ";
        }
        cout << root->key.grade << setw(4) << setfill('0') << root->key.smthAndYear;
 
        cout << " (";
        printNodeList(root->list);
        cout << ")" << endl;
 
        draw(root->left, h + 4);
    }
}
 
 
void deleteListForKey(NodeTree* root, StudentGroup key) {
    NodeTree* node = root;
    while (node != nullptr) {
        if (key.grade < node->key.grade || (key.grade == node->key.grade && key.smthAndYear < node->key.smthAndYear)) {
            node = node->left;
        } else if (key.grade > node->key.grade || (key.grade == node->key.grade && key.smthAndYear > node->key.smthAndYear)) {
            node = node->right;
        } else {
            NodeList* current = node->list;
            NodeList* next;
            do {
                NodeList* temp = current;
                current = current->next;
                delete temp;
            } while (current != node->list);
            node->list = nullptr;
            return;
        }
    }
    
 
    cout << "Ключ не найден в дереве." << endl;
}
 
void deleteTree(NodeTree*& root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
 
        if (root->list) {
            deleteList(root->list);
        }
 
        delete root;
        root = nullptr;
    }
}
 
 
int main() {
    NodeTree* root = nullptr;

    ifstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/DataIn.txt");
    string input;
    int lineNumber = 1;
    while (inFile >> input) {
        root = insert(root, StudentGroup(input), lineNumber);
        lineNumber++;
    }
 
    inFile.close();
 
    ofstream outFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/DataOut.txt");
    
    symmetricTraversalToFile(root, outFile);
    outFile << endl;
 
    outFile.close();

    draw(root, 0);
    cout << endl;
 
    StudentGroup keyForDelete("B9007");
    remove(root, keyForDelete, 5);
    remove(root, keyForDelete, 12);
    remove(root, keyForDelete, 19); 

    draw(root, 0);
 
    return 0;
}
