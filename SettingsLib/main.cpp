#include <iostream>
#include "settings.h"

using namespace std;

int main() {
    settings *A = new settings("D:\\ClionProjects\\SettingsLib\\input.txt");

    A->set("name2", "val2");
    cout << A->get("name2", "output must be val2") << endl;
    A->reload();
    A->set("name1", "1023");

    cout << A->get("name1") << endl;
    settings::param p = A->operator[]("name1") = "asa";
    cout << string(p) << endl;
    return 0;
}
