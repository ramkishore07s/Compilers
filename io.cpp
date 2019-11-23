#include <iostream>
#include <limits>
#include <cstdio>
#include <ctype.h>
#include <stdint.h>

using namespace std;

extern "C" void outputInt(int32_t val) { printf("%d\n", val); }
extern "C" void outputUint(int32_t val) { printf("%u\n", val); }
extern "C" void outputChar(int8_t val) { printf("%c\n", (char)val); }
extern "C" void outputBool(int8_t val) {
    if (val > 0) {
        cout << "true\n";
    } else {
        cout << "false\n";
    }
}

//extern "C" int inputInt() { int32_t a; cin.ignore(); (cin >> a).get(); cin.ignore(numeric_limits<streamsize>::max(),'\n'); return a; }
//extern "C" int inputInt() {int32_t a; scanf("%d", &a); return a; }
extern "C" int inputInt() {
    int32_t a;
    char line[100000];

    printf("\nEnter any integer:");
    if( fgets(line,1000000,stdin) && sscanf(line,"%d", &a)!=1 )
        a=0;

    return a;
}
extern "C" int32_t inputChar() {
    int c;
    do
        c = getchar();
    while (isspace(c));
    return c;
}

//extern "C" int8_t* readStr(int8_t* word) {scanf("%s", word); printf("%s", word); return word; }