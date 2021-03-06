/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef rpc_TYPES_H
#define rpc_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>


namespace vagt { namespace transport { namespace rpc {

class XRpcControlField;

typedef struct _XRpcControlField__isset {
  _XRpcControlField__isset() : control1(false), control2(false), control3(false), control4(false), control5(false), control6(false) {}
  bool control1 :1;
  bool control2 :1;
  bool control3 :1;
  bool control4 :1;
  bool control5 :1;
  bool control6 :1;
} _XRpcControlField__isset;

class XRpcControlField : public virtual ::apache::thrift::TBase {
 public:

  XRpcControlField(const XRpcControlField&);
  XRpcControlField& operator=(const XRpcControlField&);
  XRpcControlField() : control1(0), control2(0), control3(0), control4(), control5(), control6() {
  }

  virtual ~XRpcControlField() throw();
  int32_t control1;
  int32_t control2;
  int32_t control3;
  std::string control4;
  std::string control5;
  std::string control6;

  _XRpcControlField__isset __isset;

  void __set_control1(const int32_t val);

  void __set_control2(const int32_t val);

  void __set_control3(const int32_t val);

  void __set_control4(const std::string& val);

  void __set_control5(const std::string& val);

  void __set_control6(const std::string& val);

  bool operator == (const XRpcControlField & rhs) const
  {
    if (__isset.control1 != rhs.__isset.control1)
      return false;
    else if (__isset.control1 && !(control1 == rhs.control1))
      return false;
    if (__isset.control2 != rhs.__isset.control2)
      return false;
    else if (__isset.control2 && !(control2 == rhs.control2))
      return false;
    if (__isset.control3 != rhs.__isset.control3)
      return false;
    else if (__isset.control3 && !(control3 == rhs.control3))
      return false;
    if (__isset.control4 != rhs.__isset.control4)
      return false;
    else if (__isset.control4 && !(control4 == rhs.control4))
      return false;
    if (__isset.control5 != rhs.__isset.control5)
      return false;
    else if (__isset.control5 && !(control5 == rhs.control5))
      return false;
    if (__isset.control6 != rhs.__isset.control6)
      return false;
    else if (__isset.control6 && !(control6 == rhs.control6))
      return false;
    return true;
  }
  bool operator != (const XRpcControlField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const XRpcControlField & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(XRpcControlField &a, XRpcControlField &b);

std::ostream& operator<<(std::ostream& out, const XRpcControlField& obj);

}}} // namespace

#endif
