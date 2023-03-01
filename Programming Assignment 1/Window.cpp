#include "Window.h"

Window::Window() {
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab() {
    Node<Tab> *activeTabNode =  tabs.getNodeAtIndex(activeTab);
    if (activeTabNode)
        return activeTabNode -> data;
    return Tab();
}

bool Window::isEmpty() const {
    return tabs.isEmpty();
}

void Window::newTab(const Tab &tab) {
    tabs.insertAtIndex(tab, activeTab+1);
    activeTab++;
}

void Window::closeTab() {
    tabs.removeNodeAtIndex(activeTab);
    if (tabs.isEmpty())
        activeTab = -1;
    else
        if (activeTab == tabs.getSize())
            activeTab--;       
}

void Window::moveActiveTabTo(int index) {
    if (index >= tabs.getSize())
        index = tabs.getSize()-1;
    tabs.moveToIndex(activeTab, index);
    if (index >= 0)       
        activeTab = index;
}

void Window::changeActiveTabTo(int index) {
    if (0 <= index && index < tabs.getSize())
        activeTab = index;
}

void Window::addTab(Node<Tab> &tab) {
    tabs.append(tab.data);
    if (tabs.getSize() == 1)
        activeTab = 0;
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}
