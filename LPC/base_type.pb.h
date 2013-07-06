// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: LPC/base_type.proto

#ifndef PROTOBUF_LPC_2fbase_5ftype_2eproto__INCLUDED
#define PROTOBUF_LPC_2fbase_5ftype_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "google/protobuf/descriptor.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_LPC_2fbase_5ftype_2eproto();
void protobuf_AssignDesc_LPC_2fbase_5ftype_2eproto();
void protobuf_ShutdownFile_LPC_2fbase_5ftype_2eproto();

class LpcMessage;

// ===================================================================

class LpcMessage : public ::google::protobuf::Message {
 public:
  LpcMessage();
  virtual ~LpcMessage();

  LpcMessage(const LpcMessage& from);

  inline LpcMessage& operator=(const LpcMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LpcMessage& default_instance();

  void Swap(LpcMessage* other);

  // implements Message ----------------------------------------------

  LpcMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LpcMessage& from);
  void MergeFrom(const LpcMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 dispatch_id = 1;
  inline bool has_dispatch_id() const;
  inline void clear_dispatch_id();
  static const int kDispatchIdFieldNumber = 1;
  inline ::google::protobuf::int32 dispatch_id() const;
  inline void set_dispatch_id(::google::protobuf::int32 value);

  // optional int32 request_id = 2;
  inline bool has_request_id() const;
  inline void clear_request_id();
  static const int kRequestIdFieldNumber = 2;
  inline ::google::protobuf::int32 request_id() const;
  inline void set_request_id(::google::protobuf::int32 value);

  // optional bytes content = 3;
  inline bool has_content() const;
  inline void clear_content();
  static const int kContentFieldNumber = 3;
  inline const ::std::string& content() const;
  inline void set_content(const ::std::string& value);
  inline void set_content(const char* value);
  inline void set_content(const void* value, size_t size);
  inline ::std::string* mutable_content();
  inline ::std::string* release_content();
  inline void set_allocated_content(::std::string* content);

  // @@protoc_insertion_point(class_scope:LpcMessage)
 private:
  inline void set_has_dispatch_id();
  inline void clear_has_dispatch_id();
  inline void set_has_request_id();
  inline void clear_has_request_id();
  inline void set_has_content();
  inline void clear_has_content();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 dispatch_id_;
  ::google::protobuf::int32 request_id_;
  ::std::string* content_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_LPC_2fbase_5ftype_2eproto();
  friend void protobuf_AssignDesc_LPC_2fbase_5ftype_2eproto();
  friend void protobuf_ShutdownFile_LPC_2fbase_5ftype_2eproto();

  void InitAsDefaultInstance();
  static LpcMessage* default_instance_;
};
// ===================================================================

static const int kMethodIdFieldNumber = 50000;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< ::google::protobuf::int32 >, 5, false >
  method_id;
static const int kPushServiceFieldNumber = 50001;
extern ::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::MethodOptions,
    ::google::protobuf::internal::PrimitiveTypeTraits< bool >, 8, false >
  push_service;

// ===================================================================

// LpcMessage

// required int32 dispatch_id = 1;
inline bool LpcMessage::has_dispatch_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LpcMessage::set_has_dispatch_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LpcMessage::clear_has_dispatch_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LpcMessage::clear_dispatch_id() {
  dispatch_id_ = 0;
  clear_has_dispatch_id();
}
inline ::google::protobuf::int32 LpcMessage::dispatch_id() const {
  return dispatch_id_;
}
inline void LpcMessage::set_dispatch_id(::google::protobuf::int32 value) {
  set_has_dispatch_id();
  dispatch_id_ = value;
}

// optional int32 request_id = 2;
inline bool LpcMessage::has_request_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LpcMessage::set_has_request_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LpcMessage::clear_has_request_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LpcMessage::clear_request_id() {
  request_id_ = 0;
  clear_has_request_id();
}
inline ::google::protobuf::int32 LpcMessage::request_id() const {
  return request_id_;
}
inline void LpcMessage::set_request_id(::google::protobuf::int32 value) {
  set_has_request_id();
  request_id_ = value;
}

// optional bytes content = 3;
inline bool LpcMessage::has_content() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LpcMessage::set_has_content() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LpcMessage::clear_has_content() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LpcMessage::clear_content() {
  if (content_ != &::google::protobuf::internal::kEmptyString) {
    content_->clear();
  }
  clear_has_content();
}
inline const ::std::string& LpcMessage::content() const {
  return *content_;
}
inline void LpcMessage::set_content(const ::std::string& value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::kEmptyString) {
    content_ = new ::std::string;
  }
  content_->assign(value);
}
inline void LpcMessage::set_content(const char* value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::kEmptyString) {
    content_ = new ::std::string;
  }
  content_->assign(value);
}
inline void LpcMessage::set_content(const void* value, size_t size) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::kEmptyString) {
    content_ = new ::std::string;
  }
  content_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LpcMessage::mutable_content() {
  set_has_content();
  if (content_ == &::google::protobuf::internal::kEmptyString) {
    content_ = new ::std::string;
  }
  return content_;
}
inline ::std::string* LpcMessage::release_content() {
  clear_has_content();
  if (content_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = content_;
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void LpcMessage::set_allocated_content(::std::string* content) {
  if (content_ != &::google::protobuf::internal::kEmptyString) {
    delete content_;
  }
  if (content) {
    set_has_content();
    content_ = content;
  } else {
    clear_has_content();
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_LPC_2fbase_5ftype_2eproto__INCLUDED
