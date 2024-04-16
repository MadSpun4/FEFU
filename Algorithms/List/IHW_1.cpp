#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* prev;
    Node* next;
};

Node* createNode(int value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

void appendToBegining(Node*& head, int value) {
    Node* newNode = createNode(value);
    if (!head) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        Node* tail = head->prev;
        newNode->next = head;
        newNode->prev = tail;
        head->prev = newNode;
        tail->next = newNode;
        head = newNode;
    }
}

void deleteAllChosenElements(Node*& head, int value) {   // сделать для одного элемента
    bool isHeadMoved = false;
    Node* current = head;
    if (!head) {
        cout << "Список пуст.\nНевозможно удалить элементы." << endl;
        return;
    }
    
    do {
        if (current->data == value) {
            isHeadMoved = false;
            Node* temp = current;
            current = current->next;
            current->prev = temp->prev;
            current->prev->next = current;
            delete temp;
            if (head == temp) {
                head = current;
                isHeadMoved = true;
            }
        } else {
            current = current->next;
        }
    } while ((current != head) or (isHeadMoved));

}

void display1(Node* head) {
    if (!head) {
        cout << "Список пуст." << endl;
        return;
    }

    Node* current = head;
    do {
        cout << current->data << " ";
        current = current->next;
    } while (current != head);
    cout << endl;
}

void display2(Node* head) {
    if (!head) {
        cout << "Список пуст." << endl;
        return;
    }

    Node* current = head->prev;
    do {
        cout << current->data << " ";
        current = current->prev;
    } while (current != head->prev);
    cout << endl;
}

void clearNode(Node*& head) {
    Node* current = head;

    if (!head) {
        cout << "Список пуст. Чистить нечего." << endl;
        return;
    }

    do {
        Node* temp = current;
        current = current->next;
        delete temp;
    } while (current != head);
}

bool containsElement(Node* head, int value) {
    if (!head) {
        return false;
    }

    Node* current = head;
    do {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    } while (current != head);

    return false;
}

Node* differenceList(Node* head1, Node* head2) {
    Node* head = nullptr;

    if (!head1) {
        return head;
    }

    Node* current1 = head1;
    do {
        if (!containsElement(head2, current1->data)) {
            appendToBegining(head, current1->data);
        }
        current1 = current1->next;
    } while (current1 != head1);

    return head;
}

bool searchElement (Node* head, int value) {
    if (!head) {
        cout << "Список пуст." << endl;
        return false;
    }

    Node* current = head;
    do {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    } while (current != head);
    cout << endl;
}


int main() {
    Node* head = nullptr;
    appendToBegining(head, 1);
    appendToBegining(head, 4);
    appendToBegining(head, 2);
    appendToBegining(head, 5);

    display1(head);
    display2(head);

    Node* head2 = nullptr;
    appendToBegining(head2, 4);
    appendToBegining(head2, 4);
    appendToBegining(head2, 2);
    appendToBegining(head2, 1);

    Node* head3 = differenceList(head, head2);
    display1(head3);
    cout << containsElement(head3, 5) << endl;


    clearNode(head);
    clearNode(head2);
    clearNode(head3);

    return 0;
}
