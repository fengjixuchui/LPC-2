#include <google/protobuf/compiler/command_line_interface.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include "server_generator.h"
#include "client_generator.h"

int main(int argc, char *argv[]) {

	google::protobuf::compiler::CommandLineInterface cli;
	cli.AllowPlugins("protoc-");

	// Proto2 C++
	google::protobuf::compiler::cpp::CppGenerator cpp_generator;
	cli.RegisterGenerator("--cpp_out", "--cpp_opt", &cpp_generator,
		"Generate C++ header and source.");

	// C++ server
	google::protobuf::compiler::cpp::ServerGenerator server_generator;
	cli.RegisterGenerator("--server_out", "--server_opt", &server_generator,
		"Generate C++ server header and source.");

	// C++ client
	google::protobuf::compiler::cpp::ClientGenerator client_generator;
	cli.RegisterGenerator("--client_out", "--client_opt", &client_generator,
		"Generate C++ client header and source.");

	return cli.Run(argc, argv);
}