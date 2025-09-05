#include <iostream>

int main(int argc , char** argv) {

	using namespace std;

	for (;argv!=nullptr;argv++) {
		switch (*(*argv++)) {
			case '-':
				switch (*(*argv++)) {
					case '-':
						cout << "Option " << *argv << endl;
						break;

					default:
						cout << "Flag " << *argv << endl;
goto error;
						break;
				}
				break;

			default:
				cerr << "unknown command :" << *argv << endl;
goto error;
		}

	}


	return 0;

error: return 1;
}
