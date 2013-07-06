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

	// sync normal method
	PID ret_pid = test_client.GetPID();
	cout << "returned pid = " << ret_pid.value() << endl;

	// async normal method
	test_client.GetPID([] (const PID &ret_pid) {
		cout << "returned pid = " << ret_pid.value() << endl;
	});

	// sync push method
	Time time = test_client.GetTime("test message");
	cout << "current time  = " << clock() << endl;
	cout << "returned time = " << time.value() << endl;

	// async push method
	test_client.GetTime("another test message", [] (const Time &time) {
		static int count = 0;
		cout << "current time  = " << clock() << endl;
		cout << "returned time = " << time.value() << endl;
		if (++count == 10)
			return false;	// return false to stop pushing
		return true;
	});

	system("pause");
	return 0;
}