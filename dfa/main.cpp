#include <iostream>
#include "regex.h"

using namespace std;

int main(int argc, char *argv[]) {
    Regex re;
    //re.Compile("(a|b)*a|bcd");
    re.Compile("(a|b)*abb");
    //re.PrintTree();

    cout << re.Match("aabb") << endl;
    cout << re.Match("abb") << endl;
    cout << re.Match("b") << endl;
    return 0;
    cout << re.Match("ab") << endl;
    cout << re.Match("bcd") << endl;

    return 0;
}
