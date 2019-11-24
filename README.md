# Building a Compiler

## Building binary:
```
git clone https://github.com/IIIT-CSG/language-design-and-documentation-ramkishore07s.git
cd language-design-and-documentation/src/
make
```

## Running the compiler:
```
./parser < test.code
```

## Features:

* Variable types:
  * Int
  * Uint
  * Char
  * Bool

* Operations support:
  * Add (+)
  * Sub (-)
  * Mul (*)
  * Div (/)
  * Or (|)
  * And (&)
  * Not (!)

* Arrays: Multi dimensional arrays

* Functions

* Function parameters:
  * Pass by value for primitive types (int , uint , char , bool)
  * Pass by reference for array types
  
* Branching:
  * For loop
  * While looop
  * If
  * Else
  * Break
  * Continue
  
## Sample Programs:

### Sample 1:
```
int pow(int i, bool xd[10]) {
    int c[10][10];
    i = 5;
    c[i][i] = i;
    outputInt(c[5]);
    for (i=0; i<10; i=i+1;) {
        c[i][i] = i * i;
        outputInt(c[i][i]);
    }
    while (i < 20) {
        i = i + 1;
        c[i-10][i-10] = i;
        if (i > 15) {
        continue;
        }
        outputInt(c[i-10][i-10]);

    }

    bool d[10][10];
    d[3][5] = true;
    outputBool(d[3][5] + 10);
    return 10;
}

int main() {
        int c;
        outputInt(c);
    int d;
    bool e[10];
    outputBool(e[3][5]);
    outputInt(d);
    outputBool(e[3][5]);
    pow(3, e);

    return 0;
}
```

### Sample 2:
```
int pow(int i[10]) {
    int j[10];
    int c;
    c = i[5];
    i[5] = 5;
        j[5] = 2;
    return 10;
}
int main() {
    int c[10][10];
    int j;
    j = 5;
        c[5][5] = 2;
            outputInt(c[5][5]);
    pow(c[5]);
    outputInt(c[5][5]);
    return 0;
}
```
