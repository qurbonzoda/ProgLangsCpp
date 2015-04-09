#include <iostream>
#include "settings.h"

using namespace std;

int main() {
    settings *A = new settings("D:\\ClionProjects\\SettingsLib\\input.txt");

    cout << A->get("name2", "there is no name2") << endl;
    A->set("name2", "val2");
    cout << A->get("name2", "output must be val2") << endl;
    A->reload();
    A->set("name1", "value1");
    A->reset();
    cout << A->get("name1", "empty") << endl;
    return 0;
}
