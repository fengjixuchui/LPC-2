#include "client_service.h"
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>
#include "../LPC/base_type.pb.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

ClientServiceGenerator::ClientServiceGenerator(const ServiceDescriptor* descriptor,
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

ClientServiceGenerator::~ClientServiceGenerator() {}

void ClientServiceGenerator::Generate(io::Printer* printer) {
	printer->Print(vars_, 
		"class $classname$ {\n");
	printer->Indent();

	printer->Print(
		"StubImpl impl_;\n"
		"typedef StubImpl::IDType IDType;\n"
		"\n");

	for (int i = 0; i < descriptor_->method_count(); i++) {
		GenerateMethodID(printer, descriptor_->method(i));	
	}
	printer->Print("\n");

	printer->Outdent();
	printer->Print(vars_,
		"public:\n");
	printer->Indent();

	printer->Print(vars_,
		"$classname$(InterProcessChannel *ipc) : impl_(ipc) {}\n"
		"\n");

	const MethodDescriptor *method;
	for (int i = 0; i < descriptor_->method_count(); i++) {
		method = descriptor_->method(i);
		if (method->options().HasExtension(push_service) 
			&& method->options().GetExtension(push_service) == true)
			GeneratePushMethod(printer, method);
		else
			GenerateMethod(printer, method);	
	}

	printer->Outdent();
	printer->Print(vars_, 
		"};\n"
		"\n");
}

void ClientServiceGenerator::GenerateMethodID( io::Printer* printer, 
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

void ClientServiceGenerator::GenerateMethod( io::Printer* printer, 
											const MethodDescriptor *method ) {
	const Descriptor *param_type = method->input_type();
	const Descriptor *return_type = method->output_type();

	map<string, string> vars;
	vars["return_type"] = return_type->name();
	vars["method_name"] = method->name();
	vars["param_type"] = param_type->name();

	struct Field {
		string name;
		const char *type_name;
	};
	vector<Field> param_fields;
	for (int i = 0; i < param_type->field_count(); i++) {
		Field field = {param_type->field(i)->name(), param_type->field(i)->cpp_type_name()};
		param_fields.push_back(field);
	}

	// 展开参数的同步调用
	printer->Print(vars, 
		"$return_type$ $method_name$(");
	if (param_fields.empty()) { 
		printer->Print("void");
	} else { 
		for (unsigned int i = 0; i < param_fields.size(); i++) {
			printer->Print(
				" $type$ $name$",
				"type", param_fields[i].type_name,
				"name", param_fields[i].name);
			if (i != param_fields.size()-1)
				printer->Print(",");
		}
	}
	printer->Print(") {\n");

	printer->Indent();
	printer->Print(vars,
		"$param_type$ params;\n");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			"params.set_$name$($name$);\n",
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		"return $method_name$(params);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 封装参数的同步调用
	printer->Print(vars, 
		"$return_type$ $method_name$(const $param_type$ &params) {\n");
	printer->Indent();
	printer->Print(vars,
		"return impl_.CallSync<$param_type$, $return_type$>(k$method_name$, params);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 展开参数的异步调用
	printer->Print(vars, 
		"void $method_name$(");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			" $type$ $name$,",
			"type", param_fields[i].type_name,
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		" ::std::function<void(const $return_type$&)> cb) {\n");

	printer->Indent();
	printer->Print(vars,
		"$param_type$ params;\n");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			"params.set_$name$($name$);\n",
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		"$method_name$(params, cb);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 封装参数的异步调用
	printer->Print(vars,
		"void $method_name$(const $param_type$ &params, ::std::function<void(const $return_type$&)> cb) {\n");
	printer->Indent();
	printer->Print(vars,
		"impl_.CallAsync<$param_type$, $return_type$>(k$method_name$, params, cb);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");
	
}

void ClientServiceGenerator::GeneratePushMethod( io::Printer* printer, 
												const MethodDescriptor *method ) {
	const Descriptor *param_type = method->input_type();
	const Descriptor *return_type = method->output_type();

	map<string, string> vars;
	vars["return_type"] = return_type->name();
	vars["method_name"] = method->name();
	vars["param_type"] = param_type->name();

	struct Field {
		string name;
		const char *type_name;
	};
	vector<Field> param_fields;
	for (int i = 0; i < param_type->field_count(); i++) {
		Field field = {param_type->field(i)->name(), 
			param_type->field(i)->cpp_type() != 9 ? 
				param_type->field(i)->cpp_type_name() :
				"::std::string"
		};
		param_fields.push_back(field);
	}

	// 展开参数的同步调用
	printer->Print(vars, 
		"$return_type$ $method_name$(");
	if (param_fields.empty()) { 
		printer->Print("void");
	} else { 
		for (unsigned int i = 0; i < param_fields.size(); i++) {
			printer->Print(
				" $type$ $name$",
				"type", param_fields[i].type_name,
				"name", param_fields[i].name);
			if (i != param_fields.size()-1)
				printer->Print(",");
		}
	}
	printer->Print(") {\n");

	printer->Indent();
	printer->Print(vars,
		"$param_type$ params;\n");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			"params.set_$name$($name$);\n",
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		"	return $method_name$(params);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 封装参数的同步调用
	printer->Print(vars, 
		"$return_type$ $method_name$(const $param_type$ &params) {\n");
	printer->Indent();
	printer->Print(vars,
		"return impl_.CallPushSync<$param_type$, $return_type$>(k$method_name$, params);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");

	// 展开参数的异步调用
	printer->Print(vars, 
		"void $method_name$(");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			" $type$ $name$,",
			"type", param_fields[i].type_name,
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		" ::std::function<bool(const $return_type$&)> cb) {\n");

	printer->Indent();

	printer->Print(vars,
		"$param_type$ params;\n");
	for (unsigned int i = 0; i < param_fields.size(); i++) {
		printer->Print(
			"params.set_$name$($name$);\n",
			"name", param_fields[i].name);
	}
	printer->Print(vars,
		"$method_name$(params, cb);\n");

	printer->Outdent();

	printer->Print(
		"}\n"
		"\n");
	
	// 封装参数的异步调用
	printer->Print(vars,
		"void $method_name$(const $param_type$ &params, ::std::function<bool(const $return_type$&)> cb) {\n");
	printer->Indent();
	printer->Print(vars,
		"impl_.CallPushAsync<$param_type$, $return_type$>(k$method_name$, params, cb);\n");
	printer->Outdent();
	printer->Print(
		"}\n"
		"\n");
}

// ===================================================================

void ClientServiceGenerator::GenerateDescriptorInitializer(
	io::Printer* printer, int index) {
		map<string, string> vars;
		vars["classname"] = descriptor_->name();
		vars["index"] = SimpleItoa(index);

		printer->Print(vars,
			"$classname$_descriptor_ = file->service($index$);\n");
}

// ===================================================================

void ClientServiceGenerator::GenerateImplementation(io::Printer* printer) {
	printer->Print(vars_,
		"$classname$::~$classname$() {}\n"
		"\n"
		"const ::google::protobuf::ServiceDescriptor* $classname$::descriptor() {\n"
		"  protobuf_AssignDescriptorsOnce();\n"
		"  return $classname$_descriptor_;\n"
		"}\n"
		"\n"
		"const ::google::protobuf::ServiceDescriptor* $classname$::GetDescriptor() {\n"
		"  protobuf_AssignDescriptorsOnce();\n"
		"  return $classname$_descriptor_;\n"
		"}\n"
		"\n");

	// Generate methods of the interface.
	GenerateNotImplementedMethods(printer);
	GenerateCallMethod(printer);
	GenerateGetPrototype(REQUEST, printer);
	GenerateGetPrototype(RESPONSE, printer);

	// Generate stub implementation.
	printer->Print(vars_,
		"$classname$_Stub::$classname$_Stub(::google::protobuf::RpcChannel* channel)\n"
		"  : channel_(channel), owns_channel_(false) {}\n"
		"$classname$_Stub::$classname$_Stub(\n"
		"    ::google::protobuf::RpcChannel* channel,\n"
		"    ::google::protobuf::Service::ChannelOwnership ownership)\n"
		"  : channel_(channel),\n"
		"    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}\n"
		"$classname$_Stub::~$classname$_Stub() {\n"
		"  if (owns_channel_) delete channel_;\n"
		"}\n"
		"\n");

	GenerateStubMethods(printer);
}

void ClientServiceGenerator::GenerateNotImplementedMethods(io::Printer* printer) {
	for (int i = 0; i < descriptor_->method_count(); i++) {
		const MethodDescriptor* method = descriptor_->method(i);
		map<string, string> sub_vars;
		sub_vars["classname"] = descriptor_->name();
		sub_vars["name"] = method->name();
		sub_vars["index"] = SimpleItoa(i);
		sub_vars["input_type"] = ClassName(method->input_type(), true);
		sub_vars["output_type"] = ClassName(method->output_type(), true);

		printer->Print(sub_vars,
			"void $classname$::$name$(::google::protobuf::RpcController* controller,\n"
			"                         const $input_type$*,\n"
			"                         $output_type$*,\n"
			"                         ::google::protobuf::Closure* done) {\n"
			"  controller->SetFailed(\"Method $name$() not implemented.\");\n"
			"  done->Run();\n"
			"}\n"
			"\n");
	}
}

void ClientServiceGenerator::GenerateCallMethod(io::Printer* printer) {
	printer->Print(vars_,
		"void $classname$::CallMethod(const ::google::protobuf::MethodDescriptor* method,\n"
		"                             ::google::protobuf::RpcController* controller,\n"
		"                             const ::google::protobuf::Message* request,\n"
		"                             ::google::protobuf::Message* response,\n"
		"                             ::google::protobuf::Closure* done) {\n"
		"  GOOGLE_DCHECK_EQ(method->service(), $classname$_descriptor_);\n"
		"  switch(method->index()) {\n");

	for (int i = 0; i < descriptor_->method_count(); i++) {
		const MethodDescriptor* method = descriptor_->method(i);
		map<string, string> sub_vars;
		sub_vars["name"] = method->name();
		sub_vars["index"] = SimpleItoa(i);
		sub_vars["input_type"] = ClassName(method->input_type(), true);
		sub_vars["output_type"] = ClassName(method->output_type(), true);

		// Note:  down_cast does not work here because it only works on pointers,
		//   not references.
		printer->Print(sub_vars,
			"    case $index$:\n"
			"      $name$(controller,\n"
			"             ::google::protobuf::down_cast<const $input_type$*>(request),\n"
			"             ::google::protobuf::down_cast< $output_type$*>(response),\n"
			"             done);\n"
			"      break;\n");
	}

	printer->Print(vars_,
		"    default:\n"
		"      GOOGLE_LOG(FATAL) << \"Bad method index; this should never happen.\";\n"
		"      break;\n"
		"  }\n"
		"}\n"
		"\n");
}

void ClientServiceGenerator::GenerateGetPrototype(RequestOrResponse which,
	io::Printer* printer) {
		if (which == REQUEST) {
			printer->Print(vars_,
				"const ::google::protobuf::Message& $classname$::GetRequestPrototype(\n");
		} else {
			printer->Print(vars_,
				"const ::google::protobuf::Message& $classname$::GetResponsePrototype(\n");
		}

		printer->Print(vars_,
			"    const ::google::protobuf::MethodDescriptor* method) const {\n"
			"  GOOGLE_DCHECK_EQ(method->service(), descriptor());\n"
			"  switch(method->index()) {\n");

		for (int i = 0; i < descriptor_->method_count(); i++) {
			const MethodDescriptor* method = descriptor_->method(i);
			const Descriptor* type =
				(which == REQUEST) ? method->input_type() : method->output_type();

			map<string, string> sub_vars;
			sub_vars["index"] = SimpleItoa(i);
			sub_vars["type"] = ClassName(type, true);

			printer->Print(sub_vars,
				"    case $index$:\n"
				"      return $type$::default_instance();\n");
		}

		printer->Print(vars_,
			"    default:\n"
			"      GOOGLE_LOG(FATAL) << \"Bad method index; this should never happen.\";\n"
			"      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);\n"
			"  }\n"
			"}\n"
			"\n");
}

void ClientServiceGenerator::GenerateStubMethods(io::Printer* printer) {
	for (int i = 0; i < descriptor_->method_count(); i++) {
		const MethodDescriptor* method = descriptor_->method(i);
		map<string, string> sub_vars;
		sub_vars["classname"] = descriptor_->name();
		sub_vars["name"] = method->name();
		sub_vars["index"] = SimpleItoa(i);
		sub_vars["input_type"] = ClassName(method->input_type(), true);
		sub_vars["output_type"] = ClassName(method->output_type(), true);

		printer->Print(sub_vars,
			"void $classname$_Stub::$name$(::google::protobuf::RpcController* controller,\n"
			"                              const $input_type$* request,\n"
			"                              $output_type$* response,\n"
			"                              ::google::protobuf::Closure* done) {\n"
			"  channel_->CallMethod(descriptor()->method($index$),\n"
			"                       controller, request, response, done);\n"
			"}\n");
	}
}

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
