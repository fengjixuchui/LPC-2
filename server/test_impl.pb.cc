#include "server/test.pb.h"
#include <windows.h>
#include <time.h>
#include <iostream>
#include <future>
using namespace std;


Test::Test(InterProcessChannel *open_for) :
impl_(open_for) {
  impl_.RegisterService<Void, PID>(kGetPID, GetPID);

  impl_.RegisterPushService<Text, Time>(kGetTime, GetTime, StopGetTime, GetTime_stop_signals_);

}

Test::~Test() {
  impl_.RemoveService(kGetPID);

  impl_.RemoveService(kGetTime);
  for (::std::pair<int, void*> pair : GetTime_stop_signals_)
	impl_.RemoveService(pair.first);

}

void Test::GetPID(const Void &params, const InterProcessReturn<PID> &r) {
	PID pid;
	pid.set_value(GetCurrentProcessId());
	r.Return(pid);
}


void Test::GetTime(const Text &params, void *&stop_signal, const InterProcessPush<Time> &r) {
	cout << params.value() << endl;
	HANDLE stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	stop_signal = stop_event;
	async(launch::async, [r, stop_event] () {
		Time time;
		while (WaitForSingleObject(stop_event, 1000) != WAIT_OBJECT_0) {
			time.set_value(clock());
			r.Push(time);
		}
		CloseHandle(stop_event);
	});
}

void Test::StopGetTime(void *stop_signal) {
	SetEvent(reinterpret_cast<HANDLE>(stop_signal));
}



// ===================================================================

