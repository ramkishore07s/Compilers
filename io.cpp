#include <cstdio>
#include <ctype.h>
#include <stdint.h>

extern "C" void outputInt(int32_t val) { printf("%d\n", val); }
extern "C" void outputChar(int32_t val) { printf("%d\n", (char)val); }
extern "C" int inputInt() { int32_t a; scanf("%d", &a); return a; }
extern "C" int32_t inputChar() {
    int c;
    do
        c = getchar();
    while (isspace(c));
    return c;
}

//extern "C" int8_t* readStr(int8_t* word) {scanf("%s", word); printf("%s", word); return word; }