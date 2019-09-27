#include <iostream>
#include <fstream>
using namespace std;

int main () {
	char str[20];
	char def[20];
 
	fstream dict;
	dict.open ("dictionary.dat", ios :: app | ios :: out | ios :: binary);

	for (int i = 0; i < 2; i++){
		cin >> str >> def;
		dict << str << " " << def << endl;
	}
	cin >> str >> def;
	dict << str << " " << def;

	dict.close ();

	return 0;
}
