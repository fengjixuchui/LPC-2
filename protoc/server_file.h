#ifndef GOOGLE_PROTOBUF_COMPILER_SERVER_FILE_H__
#define GOOGLE_PROTOBUF_COMPILER_SERVER_FILE_H__

#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/cpp/cpp_field.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>

namespace google {
	namespace protobuf {
		class FileDescriptor;        // descriptor.h
		namespace io {
			class Printer;             // printer.h
		}
	}

	namespace protobuf {
		namespace compiler {
			namespace cpp {

				class EnumGenerator;           // enum.h
				class MessageGenerator;        // message.h
				class ServerServiceGenerator;        // service.h
				class ExtensionGenerator;      // extension.h

				class ServerFileGenerator {
				public:
					// See generator.cc for the meaning of dllexport_decl.
					explicit ServerFileGenerator(const FileDescriptor* file,
						const Options& options);
					~ServerFileGenerator();

					void GenerateHeader(io::Printer* printer);
					void GenerateMessageImpl(io::Printer* printer);
					void GenerateServiceImpl(io::Printer* printer);

				private:
					// Generate the BuildDescriptors() procedure, which builds all descriptors
					// for types defined in the file.
					void GenerateBuildDescriptors(io::Printer* printer);

					void GenerateNamespaceOpeners(io::Printer* printer);
					void GenerateNamespaceClosers(io::Printer* printer);

					const FileDescriptor* file_;

					scoped_array<scoped_ptr<MessageGenerator> > message_generators_;
					scoped_array<scoped_ptr<EnumGenerator> > enum_generators_;
					scoped_array<scoped_ptr<ServerServiceGenerator> > service_generators_;
					scoped_array<scoped_ptr<ExtensionGenerator> > extension_generators_;

					// E.g. if the package is foo.bar, package_parts_ is {"foo", "bar"}.
					vector<string> package_parts_;

					const Options options_;

					GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ServerFileGenerator);
				};

			}  // namespace cpp
		}  // namespace compiler
	}  // namespace protobuf

}  // namespace google
#endif  // GOOGLE_PROTOBUF_COMPILER_CPP_FILE_H__