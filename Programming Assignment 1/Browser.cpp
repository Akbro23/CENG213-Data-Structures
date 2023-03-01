#include "Browser.h"

Browser::Browser() {
    windows.append(Window());
}

void Browser::newWindow() {
    windows.prepend(Window());
}

void Browser::closeWindow() {
    if (!windows.isEmpty()){
        Node<Window> *firstWindow = windows.getFirstNode();
        windows.removeNode(firstWindow);
    }
}

void Browser::switchToWindow(int index) {
    if (0 <= index && index < windows.getSize())
        windows.moveToIndex(index, 0);
}

Window &Browser::getWindow(int index) {
    return windows.getNodeAtIndex(index) -> data;
}

void Browser::moveTab(Window &from, Window &to) {
    Tab fromTab = from.getActiveTab();
    from.closeTab();
    to.newTab(fromTab);
}

void Browser::mergeWindows(Window &window1, Window &window2) {
    window2.changeActiveTabTo(0);
    while (!window2.isEmpty()){       
        moveTab(window2, window1);
    }
}

void Browser::mergeAllWindows() {
    Node<Window> *window1 = windows.getFirstNode();
    if (window1){
        Node<Window> *window2 = window1 -> next;
        while (window2 != window1){
            mergeWindows(window1 -> data, window2 -> data);
            window2 = window2 -> next;
        }
    }

}

void Browser::closeAllWindows() {
    windows.removeAllNodes();
}

void Browser::closeEmptyWindows() {
    Node<Window> *curNode = windows.getFirstNode();
    while (!windows.isEmpty() && curNode -> data.isEmpty()){
        Node<Window> *nextNode = curNode -> next;
        windows.removeNode(curNode);
        curNode = nextNode;
    }
    if (!windows.isEmpty()){
        do {
            Node<Window> *nextNode = curNode -> next;
            if (curNode -> data.isEmpty())
                windows.removeNode(curNode);
            curNode = nextNode;
        }while (curNode != windows.getFirstNode());
    }
}

void Browser::print() {
    Node<Window> *head = windows.getFirstNode();
    if(head == NULL) {
        std::cout << "The browser is empty" << std::endl;
    } else {
        (head -> data).print();
        head = head -> next;
        while(head != windows.getFirstNode()) {
            (head -> data).print();
            head = head -> next;
        }
    }
}
