#include <iostream>
#include "regex.h"

using namespace std;

int main(int argc, char *argv[]) {
    Regex re;
    re.Compile("(a|b)*a|bcd");
    cout << re.Match("ab") << endl;
    cout << re.Match("bcd") << endl;

    return 0;
}
