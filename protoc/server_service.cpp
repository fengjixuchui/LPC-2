#include "server_service.h"
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>
#include "../LPC/base_type.pb.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

ServerServiceGenerator::ServerServiceGenerator(const ServiceDescriptor* descriptor,
	const Options& options)
	: descriptor_(descriptor) {
		vars_["classname"] = descriptor_->name();
		vars_["full_name"] = descriptor_->full_name();
		if (options.dllexport_decl.empty()) {
			vars_["dllexport"] = "";
		} else {
			vars_["dllexport"] = options.dllexport_decl + " ";
		}
}

ServerServiceGenerator::~ServerServiceGenerator() {}

void ServerServiceGenerator::GenerateDeclarations(io::Printer* printer) {
	printer->Print(vars_,
		"class $classname$ {\n");
	printer->Indent();

	printer->Print(
		"ServiceImpl impl_;\n"
		"typedef ServiceImpl::IDType IDType;\n"
		"\n");

	for (int i = 0; i < descriptor_->method_count(); i++) {
		GenerateMethodID(printer, descriptor_->method(i));	
	}

	printer->Outdent();
	printer->Print("\npublic:\n");
	printer->Indent();

	printer->Print(vars_,
		"$classname$(InterProcessChannel *open_for);\n"
		"~$classname$();\n"
		"\n");

	printer->Outdent();
	printer->Print("private:\n");
	printer->Indent();

	const MethodDescriptor *method;
	for (int i = 0; i < descriptor_->method_count(); i++) {
		method = descriptor_->method(i);
		if (method->options().HasExtension(push_service) 
			&& method->options().GetExtension(push_service) == true)
			GeneratePushMethodDeclaration(printer, method);
		else
			GenerateMethodDeclaration(printer, method);	
	}

	printer->Outdent();
	printer->Print(vars_, 
		"};\n"
		"\n");
}

void ServerServiceGenerator::GenerateMethodID( io::Printer* printer, 
											  const MethodDescriptor *method ) {
	MethodOptions options = method->options();
	if (!options.HasExtension(method_id) || options.GetExtension(method_id) == 0)
		throw std::invalid_argument("每个方法都需要一个大于0的method_id选项");

	char id_buf[32];
	printer->Print(
		"static const IDType k$method_name$ = $method_id$;\n",
		"method_name", method->name(),
		"method_id", _itoa(options.GetExtension(method_id), id_buf, 10));
}

void ServerServiceGenerator::GenerateMethodDeclaration( io::Printer* printer, 
											const MethodDescriptor *method ) {
	printer->Print(
		"static void $method_name$(const $param_type$&, const InterProcessReturn<$return_type$>&);\n\n", 
		"method_name", method->name(),
		"param_type", method->input_type()->name(),
		"return_type", method->output_type()->name());
}

void ServerServiceGenerator::GeneratePushMethodDeclaration( io::Printer* printer, 
												const MethodDescriptor *method ) {
	printer->Print(
		"::std::unordered_map<IDType, void*> $method_name$_stop_signals_;\n",
		"method_name", method->name());
	printer->Print(
		"static void $method_name$(const $param_type$&, void *&, const InterProcessPush<$return_type$>&);\n", 
		"method_name", method->name(),
		"param_type", method->input_type()->name(),
		"return_type", method->output_type()->name());
	printer->Print(
		"static void Stop$method_name$(void *stop_signal);\n\n",
		"method_name", method->name());
}

// ===================================================================

void ServerServiceGenerator::GenerateDescriptorInitializer(
	io::Printer* printer, int index) {
		map<string, string> vars;
		vars["classname"] = descriptor_->name();
		vars["index"] = SimpleItoa(index);

		printer->Print(vars,
			"$classname$_descriptor_ = file->service($index$);\n");
}

// ===================================================================

void ServerServiceGenerator::GenerateImplementation(io::Printer* printer) {
	const MethodDescriptor *method;

	// 构造函数
	printer->Print(vars_,
		"$classname$::$classname$(InterProcessChannel *open_for) :\n"
		"impl_(open_for) {\n");
	printer->Indent();

	for (int i = 0; i < descriptor_->method_count(); i++) {
		method = descriptor_->method(i);
		if (method->options().HasExtension(push_service) 
			&& method->options().GetExtension(push_service) == true)
			GeneratePushMethodRegisterCode(printer, method);
		else
			GenerateMethodRegisterCode(printer, method);	
	}

	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 析构函数
	printer->Print(vars_,
		"$classname$::~$classname$() {\n");
	printer->Indent();

	for (int i = 0; i < descriptor_->method_count(); i++) {
		method = descriptor_->method(i);
		if (method->options().HasExtension(push_service) 
			&& method->options().GetExtension(push_service) == true)
			GeneratePushMethodRemoveCode(printer, method);
		else
			GenerateMethodRemoveCode(printer, method);	
	}

	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 待实现的方法
	for (int i = 0; i < descriptor_->method_count(); i++) {
		method = descriptor_->method(i);
		if (method->options().HasExtension(push_service) 
			&& method->options().GetExtension(push_service) == true)
			GenerateEmptyPushMethod(printer, method);
		else
			GenerateEmptyMethod(printer, method);	
	}
}

void ServerServiceGenerator::GenerateMethodRegisterCode( io::Printer* printer, 
														const MethodDescriptor *method ) {
	printer->Print(
		"impl_.RegisterService<$param_type$, $return_type$>(k$method_name$, $method_name$);\n"
		"\n",
		"method_name", method->name(),
		"param_type", method->input_type()->name(),
		"return_type", method->output_type()->name());
}

void ServerServiceGenerator::GeneratePushMethodRegisterCode( io::Printer* printer, 
															const MethodDescriptor *method ) {
	printer->Print(
		"impl_.RegisterPushService<$param_type$, $return_type$>(k$method_name$, "
		"$method_name$, Stop$method_name$, $method_name$_stop_signals_);\n"
		"\n",
		"method_name", method->name(),
		"param_type", method->input_type()->name(),
		"return_type", method->output_type()->name());
}

void ServerServiceGenerator::GenerateMethodRemoveCode( io::Printer* printer, 
													  const MethodDescriptor *method ) {
	printer->Print(
		"impl_.RemoveService(k$method_name$);\n"
		"\n",
		"method_name", method->name());
}

void ServerServiceGenerator::GeneratePushMethodRemoveCode( io::Printer* printer, 
														  const MethodDescriptor *method ) {
	printer->Print(
		"impl_.RemoveService(k$method_name$);\n"
		"for (::std::pair<int, void*> pair : $method_name$_stop_signals_)\n",
		"method_name", method->name());
	printer->Indent();
	printer->Print(
		"impl_.RemoveService(pair.first);\n");
	printer->Outdent();
	printer->Print(
		"\n");
}

void ServerServiceGenerator::GenerateEmptyMethod( io::Printer* printer, 
												 const MethodDescriptor *method ) {
	map<string, string> vars = vars_;
	vars["method_name"] = method->name();
	vars["param_type"] = method->input_type()->name();
	vars["return_type"] = method->output_type()->name();

	printer->Print(vars, 
		"void $classname$::$method_name$(const $param_type$ &params, const "
		"InterProcessReturn<$return_type$> &r) {\n"
		"\n"
		"}\n"
		"\n\n");
}

void ServerServiceGenerator::GenerateEmptyPushMethod( io::Printer* printer, 
													 const MethodDescriptor *method ) {
	map<string, string> vars = vars_;
	vars["method_name"] = method->name();
	vars["param_type"] = method->input_type()->name();
	vars["return_type"] = method->output_type()->name();

	printer->Print(vars,
		"void $classname$::$method_name$(const $param_type$ &params, "
		"void *&stop_signal, const InterProcessPush<$return_type$> &r) {\n"
		"\n"
		"}\n"
		"\n"
		"void $classname$::Stop$method_name$(void *stop_signal) {\n"
		"\n"
		"}\n"
		"\n\n");
}



}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
