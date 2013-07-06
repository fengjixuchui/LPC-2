// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: client/test.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "client/test.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Void_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Void_reflection_ = NULL;
const ::google::protobuf::Descriptor* PID_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PID_reflection_ = NULL;
const ::google::protobuf::Descriptor* Text_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Text_reflection_ = NULL;
const ::google::protobuf::Descriptor* Time_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Time_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_client_2ftest_2eproto() {
  protobuf_AddDesc_client_2ftest_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "client/test.proto");
  GOOGLE_CHECK(file != NULL);
  Void_descriptor_ = file->message_type(0);
  static const int Void_offsets_[1] = {
  };
  Void_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Void_descriptor_,
      Void::default_instance_,
      Void_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Void, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Void, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Void));
  PID_descriptor_ = file->message_type(1);
  static const int PID_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PID, value_),
  };
  PID_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PID_descriptor_,
      PID::default_instance_,
      PID_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PID, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PID, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PID));
  Text_descriptor_ = file->message_type(2);
  static const int Text_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Text, value_),
  };
  Text_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Text_descriptor_,
      Text::default_instance_,
      Text_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Text, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Text, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Text));
  Time_descriptor_ = file->message_type(3);
  static const int Time_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Time, value_),
  };
  Time_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Time_descriptor_,
      Time::default_instance_,
      Time_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Time, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Time, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Time));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_client_2ftest_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Void_descriptor_, &Void::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PID_descriptor_, &PID::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Text_descriptor_, &Text::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Time_descriptor_, &Time::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_client_2ftest_2eproto() {
  delete Void::default_instance_;
  delete Void_reflection_;
  delete PID::default_instance_;
  delete PID_reflection_;
  delete Text::default_instance_;
  delete Text_reflection_;
  delete Time::default_instance_;
  delete Time_reflection_;
}

void protobuf_AddDesc_client_2ftest_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_LPC_2fbase_5ftype_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021client/test.proto\032\023LPC/base_type.proto"
    "\"\006\n\004Void\"\024\n\003PID\022\r\n\005value\030\001 \001(\005\"\025\n\004Text\022\r"
    "\n\005value\030\001 \001(\t\"\025\n\004Time\022\r\n\005value\030\001 \001(\0052F\n\004"
    "Test\022\033\n\006GetPID\022\005.Void\032\004.PID\"\004\200\265\030\001\022!\n\007Get"
    "Time\022\005.Text\032\005.Time\"\010\200\265\030\002\210\265\030\001", 188);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "client/test.proto", &protobuf_RegisterTypes);
  Void::default_instance_ = new Void();
  PID::default_instance_ = new PID();
  Text::default_instance_ = new Text();
  Time::default_instance_ = new Time();
  Void::default_instance_->InitAsDefaultInstance();
  PID::default_instance_->InitAsDefaultInstance();
  Text::default_instance_->InitAsDefaultInstance();
  Time::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_client_2ftest_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_client_2ftest_2eproto {
  StaticDescriptorInitializer_client_2ftest_2eproto() {
    protobuf_AddDesc_client_2ftest_2eproto();
  }
} static_descriptor_initializer_client_2ftest_2eproto_;

// ===================================================================

#ifndef _MSC_VER
#endif  // !_MSC_VER

Void::Void()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Void::InitAsDefaultInstance() {
}

Void::Void(const Void& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Void::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Void::~Void() {
  SharedDtor();
}

void Void::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Void::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Void::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Void_descriptor_;
}

const Void& Void::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2ftest_2eproto();
  return *default_instance_;
}

Void* Void::default_instance_ = NULL;

Void* Void::New() const {
  return new Void;
}

void Void::Clear() {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Void::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      return true;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, mutable_unknown_fields()));
  }
  return true;
#undef DO_
}

void Void::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Void::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Void::ByteSize() const {
  int total_size = 0;

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Void::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Void* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Void*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Void::MergeFrom(const Void& from) {
  GOOGLE_CHECK_NE(&from, this);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Void::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Void::CopyFrom(const Void& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Void::IsInitialized() const {

  return true;
}

void Void::Swap(Void* other) {
  if (other != this) {
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Void::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Void_descriptor_;
  metadata.reflection = Void_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int PID::kValueFieldNumber;
#endif  // !_MSC_VER

PID::PID()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void PID::InitAsDefaultInstance() {
}

PID::PID(const PID& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void PID::SharedCtor() {
  _cached_size_ = 0;
  value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PID::~PID() {
  SharedDtor();
}

void PID::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PID::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PID::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PID_descriptor_;
}

const PID& PID::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2ftest_2eproto();
  return *default_instance_;
}

PID* PID::default_instance_ = NULL;

PID* PID::New() const {
  return new PID;
}

void PID::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    value_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PID::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 value = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &value_)));
          set_has_value();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void PID::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 value = 1;
  if (has_value()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->value(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* PID::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 value = 1;
  if (has_value()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->value(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int PID::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 value = 1;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->value());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PID::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PID* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PID*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PID::MergeFrom(const PID& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_value()) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PID::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PID::CopyFrom(const PID& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PID::IsInitialized() const {

  return true;
}

void PID::Swap(PID* other) {
  if (other != this) {
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PID::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PID_descriptor_;
  metadata.reflection = PID_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Text::kValueFieldNumber;
#endif  // !_MSC_VER

Text::Text()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Text::InitAsDefaultInstance() {
}

Text::Text(const Text& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Text::SharedCtor() {
  _cached_size_ = 0;
  value_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Text::~Text() {
  SharedDtor();
}

void Text::SharedDtor() {
  if (value_ != &::google::protobuf::internal::kEmptyString) {
    delete value_;
  }
  if (this != default_instance_) {
  }
}

void Text::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Text::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Text_descriptor_;
}

const Text& Text::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2ftest_2eproto();
  return *default_instance_;
}

Text* Text::default_instance_ = NULL;

Text* Text::New() const {
  return new Text;
}

void Text::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_value()) {
      if (value_ != &::google::protobuf::internal::kEmptyString) {
        value_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Text::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string value = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_value()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->value().data(), this->value().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Text::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string value = 1;
  if (has_value()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->value(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Text::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string value = 1;
  if (has_value()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->value(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Text::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string value = 1;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->value());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Text::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Text* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Text*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Text::MergeFrom(const Text& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_value()) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Text::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Text::CopyFrom(const Text& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Text::IsInitialized() const {

  return true;
}

void Text::Swap(Text* other) {
  if (other != this) {
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Text::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Text_descriptor_;
  metadata.reflection = Text_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Time::kValueFieldNumber;
#endif  // !_MSC_VER

Time::Time()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Time::InitAsDefaultInstance() {
}

Time::Time(const Time& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Time::SharedCtor() {
  _cached_size_ = 0;
  value_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Time::~Time() {
  SharedDtor();
}

void Time::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Time::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Time::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Time_descriptor_;
}

const Time& Time::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_client_2ftest_2eproto();
  return *default_instance_;
}

Time* Time::default_instance_ = NULL;

Time* Time::New() const {
  return new Time;
}

void Time::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    value_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Time::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 value = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &value_)));
          set_has_value();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Time::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 value = 1;
  if (has_value()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->value(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Time::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 value = 1;
  if (has_value()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->value(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Time::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 value = 1;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->value());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Time::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Time* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Time*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Time::MergeFrom(const Time& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_value()) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Time::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Time::CopyFrom(const Time& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Time::IsInitialized() const {

  return true;
}

void Time::Swap(Time* other) {
  if (other != this) {
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Time::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Time_descriptor_;
  metadata.reflection = Time_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
