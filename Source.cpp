#include<iostream>	
#include<string>
#include<fstream>
#include<cstring>
#include<vector>


using namespace std;
 
enum state {
	a,
	b,
	c,
	d,
	e
};

int main() {

	//int low, high, slice$;

	vector<char> buffer;
	vector<string> ids;
	vector<char> ops;
	state go = a;
	fstream file;
	
	file.open("code.txt");

	char i;
	char *bb, *cc;
	string aps = " ";
	bb = cc = &buffer[0];
	while (!file.eof()) {
		file >> i;
		buffer.push_back(i);
	}
	

	int a = 0;
	while (a<buffer.size()) {
		if (isalpha(*cc)) {
			go = b;
		}if (isdigit(*cc)) {
			go = c;
			while (isdigit(*cc)) {
				go = c; 
				cc++;
			}
		}if (*cc == ' ' || *cc == ',' || *cc == ';') {
			ops.push_back(*cc);
			cc--;
			while (bb != cc) {
				aps += *bb;
			}
			cout << "ID = " << aps;
		}
		a++;
	}
 

	 



	cout << endl;
	system("Pause");
	return 0;
}


 