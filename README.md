LPC
===

LPC is a windows local procedure call library based on protobuf.

USAGE
---

Write a .proto file containing the LPC method definitions.

    // test.proto
    import "LPC/base_type.proto";

  	message Void {}
  	message PID {
  		optional int32 value = 1;
  	}
  
  	message Text {
  		optional string value = 1;
  	}
  	message Time {
  		optional int32 value = 1;
  	}
  
  	service Test {
  		rpc GetPID(Void) returns(PID) { 
  			option (method_id) = 1;
  		}
  		
  		rpc GetTime(Text) returns(Time) { 
  			option (method_id) = 2;
  			option (push_service) = true;
  		}
  	}
  
Use `--server_out` and `--client_out` with the protoc.exe to compile the .proto file into C++ source files.  
`test.pb.h` and `test.pb.cc` for the client. Another `test.pb.h`, `test.pb.cc` and `test_impl.pb.cc` for the server.
Implement methods in `test_impl.pb.cc`

    // test_impl.pb.cc
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
    
For my implementation, the server and the client must know the process id of each other.

    // server.cpp
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

and

    // client.cpp
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
