#include <iostream>
#include "regex.h"

using namespace std;

int main(int argc, char *argv[]) {
    Regex re;
    re.Compile("abc");
    cout << re.Match("ab") << endl;

    return 0;
}
