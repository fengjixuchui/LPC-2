#include <windows.h>
#include <iostream>
#include "test.pb.h"
#include <time.h>
using namespace std;

int main() {
	cout << GetCurrentProcessId() << endl;
	DWORD pid;
	cin >> pid;
	InterProcessChannel *ipc = InterProcessChannel::ToProcess(pid);

	Test test_client(ipc);

	system("pause");
	return 0;
}