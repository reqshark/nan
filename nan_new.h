/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT License <https://github.com/rvagg/nan/blob/master/LICENSE.md>
 ********************************************************************/

#ifndef NAN_NEW_H_
#define NAN_NEW_H_

#include <string>

namespace NanIntern {  // scnr

// TODO(agnat): Generalize
template <typename T> v8::Local<T> To(v8::Handle<v8::Integer> i);

template <>
inline
v8::Local<v8::Integer>
To<v8::Integer>(v8::Handle<v8::Integer> i) { return i->ToInteger(); }

template <>
inline
v8::Local<v8::Int32>
To<v8::Int32>(v8::Handle<v8::Integer> i)   { return i->ToInt32(); }

template <>
inline
v8::Local<v8::Uint32>
To<v8::Uint32>(v8::Handle<v8::Integer> i)  { return i->ToUint32(); }

template <typename T> struct FactoryBase { typedef v8::Local<T> return_t; };

template <typename T> struct Factory;

template <>
struct Factory<v8::Array> : public FactoryBase<v8::Array> {
  static inline return_t New();
  static inline return_t New(int length);
};

template <>
struct Factory<v8::Boolean> : public FactoryBase<v8::Boolean> {
  static inline return_t New(bool value);
};

template <>
struct Factory<v8::BooleanObject> : public FactoryBase<v8::BooleanObject> {
  static inline return_t New(bool value);
};

template <>
struct Factory<v8::Date> : public FactoryBase<v8::Date> {
  static inline return_t New(double value);
};

template <>
struct Factory<v8::External> : public FactoryBase<v8::External> {
  static inline return_t New(void *value);
};

template <>
class Factory<v8::FunctionTemplate> : public FactoryBase<v8::FunctionTemplate> {
 public:
  static inline
  return_t
  New( NanFunctionCallback callback = NULL
     , v8::Handle<v8::Value> data = v8::Handle<v8::Value>()
     , v8::Handle<v8::Signature> signature = v8::Handle<v8::Signature>());
};

template <>
struct Factory<v8::Number> : public FactoryBase<v8::Number> {
  static inline return_t New(double value);
};

template <>
struct Factory<v8::NumberObject> : public FactoryBase<v8::NumberObject> {
  static inline return_t New(double value);
};

template <typename T>
struct IntegerFactory : public FactoryBase<T> {
  typedef typename FactoryBase<T>::return_t return_t;
  static inline return_t New(int32_t value);
  static inline return_t New(uint32_t value);
};

template <>
struct Factory<v8::Integer> : public IntegerFactory<v8::Integer> {};

template <>
struct Factory<v8::Int32> : public IntegerFactory<v8::Int32> {};

template <>
struct Factory<v8::Uint32> : public FactoryBase<v8::Uint32> {
  static inline return_t New(int32_t value);
  static inline return_t New(uint32_t value);
};

template <>
struct Factory<v8::Object> : public FactoryBase<v8::Object> {
  static inline return_t New();
};

template <>
struct Factory<v8::RegExp> : public FactoryBase<v8::RegExp> {
  static inline return_t New(
      v8::Handle<v8::String> pattern, v8::RegExp::Flags flags);
};

template <>
struct Factory<v8::Script> : public FactoryBase<v8::Script> {
  static inline return_t New( v8::Local<v8::String> source);
  static inline return_t New( v8::Local<v8::String> source
                            , v8::ScriptOrigin const& origin);
};

template <>
struct Factory<v8::Signature> : public FactoryBase<v8::Signature> {
  typedef v8::Handle<v8::FunctionTemplate> FTH;
  static inline
  return_t
  New( FTH receiver = FTH(), int argc = 0, FTH argv[] = NULL );
};

template <>
struct Factory<v8::String> : public FactoryBase<v8::String> {
  static inline return_t New(const char *value, int length = -1);
  static inline return_t New(const uint16_t *value, int length = -1);
  static inline return_t New(std::string const& value);

  static inline return_t New(v8::String::ExternalStringResource * value);
  static inline return_t New(v8::String::ExternalAsciiStringResource * value);

  // TODO(agnat): Deprecate.
  static inline return_t New(const uint8_t * value, int length = -1);
};

template <>
struct Factory<v8::StringObject> : public FactoryBase<v8::StringObject> {
  static inline return_t New(v8::Handle<v8::String> value);
};

}  // end of namespace NanIntern

#if (NODE_MODULE_VERSION >= 12)

namespace NanIntern {

template <>
struct Factory<v8::UnboundScript> : public FactoryBase<v8::UnboundScript> {
  static inline return_t New( v8::Local<v8::String> source);
  static inline return_t New( v8::Local<v8::String> source
                            , v8::ScriptOrigin const& origin);
};

}  // end of namespace NanIntern

# include "nan_implementation_12_inl.h"

#else  // NODE_MODULE_VERSION >= 12

# include "nan_implementation_pre_12_inl.h"

#endif

//=== API ======================================================================

template <typename T>
typename NanIntern::Factory<T>::return_t
NanNew() {
  return NanIntern::Factory<T>::New();
}

template <typename T, typename A0>
typename NanIntern::Factory<T>::return_t
NanNew(A0 arg0) {
  return NanIntern::Factory<T>::New(arg0);
}

template <typename T, typename A0, typename A1>
typename NanIntern::Factory<T>::return_t
NanNew(A0 arg0, A1 arg1) {
  return NanIntern::Factory<T>::New(arg0, arg1);
}

template <typename T, typename A0, typename A1, typename A2>
typename NanIntern::Factory<T>::return_t
NanNew(A0 arg0, A1 arg1, A2 arg2) {
  return NanIntern::Factory<T>::New(arg0, arg1, arg2);
}

template <typename T, typename A0, typename A1, typename A2, typename A3>
typename NanIntern::Factory<T>::return_t
NanNew(A0 arg0, A1 arg1, A2 arg2, A3 arg3) {
  return NanIntern::Factory<T>::New(arg0, arg1, arg2, arg3);
}

// Convenience

template <typename T> inline v8::Local<T> NanNew(v8::Handle<T> h);
template <typename T> inline v8::Local<T> NanNew(v8::Persistent<T> const& p);

inline
NanIntern::Factory<v8::Boolean>::return_t
NanNew(bool value) {
  return NanNew<v8::Boolean>(value);
}

inline
NanIntern::Factory<v8::Int32>::return_t
NanNew(int32_t value) {
  return NanNew<v8::Int32>(value);
}

inline
NanIntern::Factory<v8::Uint32>::return_t
NanNew(uint32_t value) {
  return NanNew<v8::Uint32>(value);
}

inline
NanIntern::Factory<v8::Number>::return_t
NanNew(double value) {
  return NanNew<v8::Number>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(std::string const& value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(const char * value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(const uint8_t * value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(const uint16_t * value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(v8::String::ExternalStringResource * value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::String>::return_t
NanNew(v8::String::ExternalAsciiStringResource * value) {
  return NanNew<v8::String>(value);
}

inline
NanIntern::Factory<v8::RegExp>::return_t
NanNew(v8::Handle<v8::String> pattern, v8::RegExp::Flags flags) {
  return NanNew<v8::RegExp>(pattern, flags);
}

#endif  // NAN_NEW_H_
