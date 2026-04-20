#include <iostream>
 using namespace std;

 int main() {
     string* n;
     n = new string[3];
     n[0] = "Hola";
     n[1] = "Maria";
     n[2] = "Joh";
     for (int i = 0; i < 3; i++) {
         cout << n[i] << endl;
     }
     return 0;
 }