// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_C2S_ABBY_H_
#define FLATBUFFERS_GENERATED_C2S_ABBY_H_

#include "flatbuffers/flatbuffers.h"

namespace abby {

struct ClientA;
struct ClientABuilder;
struct ClientAT;

struct ClientB;
struct ClientBBuilder;
struct ClientBT;

struct ClientC;
struct ClientCBuilder;
struct ClientCT;

struct ClientData;
struct ClientDataBuilder;
struct ClientDataT;

enum ClientType : int8_t {
  ClientType_ClientA = 0,
  ClientType_ClientB = 1,
  ClientType_ClientC = 2,
  ClientType_MIN = ClientType_ClientA,
  ClientType_MAX = ClientType_ClientC
};

inline const ClientType (&EnumValuesClientType())[3] {
  static const ClientType values[] = {
    ClientType_ClientA,
    ClientType_ClientB,
    ClientType_ClientC
  };
  return values;
}

inline const char * const *EnumNamesClientType() {
  static const char * const names[4] = {
    "ClientA",
    "ClientB",
    "ClientC",
    nullptr
  };
  return names;
}

inline const char *EnumNameClientType(ClientType e) {
  if (flatbuffers::IsOutRange(e, ClientType_ClientA, ClientType_ClientC)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesClientType()[index];
}

enum ClientField : uint8_t {
  ClientField_NONE = 0,
  ClientField_ClientA = 1,
  ClientField_ClientB = 2,
  ClientField_ClientC = 3,
  ClientField_MIN = ClientField_NONE,
  ClientField_MAX = ClientField_ClientC
};

inline const ClientField (&EnumValuesClientField())[4] {
  static const ClientField values[] = {
    ClientField_NONE,
    ClientField_ClientA,
    ClientField_ClientB,
    ClientField_ClientC
  };
  return values;
}

inline const char * const *EnumNamesClientField() {
  static const char * const names[5] = {
    "NONE",
    "ClientA",
    "ClientB",
    "ClientC",
    nullptr
  };
  return names;
}

inline const char *EnumNameClientField(ClientField e) {
  if (flatbuffers::IsOutRange(e, ClientField_NONE, ClientField_ClientC)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesClientField()[index];
}

template<typename T> struct ClientFieldTraits {
  static const ClientField enum_value = ClientField_NONE;
};

template<> struct ClientFieldTraits<abby::ClientA> {
  static const ClientField enum_value = ClientField_ClientA;
};

template<> struct ClientFieldTraits<abby::ClientB> {
  static const ClientField enum_value = ClientField_ClientB;
};

template<> struct ClientFieldTraits<abby::ClientC> {
  static const ClientField enum_value = ClientField_ClientC;
};

struct ClientFieldUnion {
  ClientField type;
  void *value;

  ClientFieldUnion() : type(ClientField_NONE), value(nullptr) {}
  ClientFieldUnion(ClientFieldUnion&& u) FLATBUFFERS_NOEXCEPT :
    type(ClientField_NONE), value(nullptr)
    { std::swap(type, u.type); std::swap(value, u.value); }
  ClientFieldUnion(const ClientFieldUnion &);
  ClientFieldUnion &operator=(const ClientFieldUnion &u)
    { ClientFieldUnion t(u); std::swap(type, t.type); std::swap(value, t.value); return *this; }
  ClientFieldUnion &operator=(ClientFieldUnion &&u) FLATBUFFERS_NOEXCEPT
    { std::swap(type, u.type); std::swap(value, u.value); return *this; }
  ~ClientFieldUnion() { Reset(); }

  void Reset();

#ifndef FLATBUFFERS_CPP98_STL
  template <typename T>
  void Set(T&& val) {
    using RT = typename std::remove_reference<T>::type;
    Reset();
    type = ClientFieldTraits<typename RT::TableType>::enum_value;
    if (type != ClientField_NONE) {
      value = new RT(std::forward<T>(val));
    }
  }
#endif  // FLATBUFFERS_CPP98_STL

  static void *UnPack(const void *obj, ClientField type, const flatbuffers::resolver_function_t *resolver);
  flatbuffers::Offset<void> Pack(flatbuffers::FlatBufferBuilder &_fbb, const flatbuffers::rehasher_function_t *_rehasher = nullptr) const;

  abby::ClientAT *AsClientA() {
    return type == ClientField_ClientA ?
      reinterpret_cast<abby::ClientAT *>(value) : nullptr;
  }
  const abby::ClientAT *AsClientA() const {
    return type == ClientField_ClientA ?
      reinterpret_cast<const abby::ClientAT *>(value) : nullptr;
  }
  abby::ClientBT *AsClientB() {
    return type == ClientField_ClientB ?
      reinterpret_cast<abby::ClientBT *>(value) : nullptr;
  }
  const abby::ClientBT *AsClientB() const {
    return type == ClientField_ClientB ?
      reinterpret_cast<const abby::ClientBT *>(value) : nullptr;
  }
  abby::ClientCT *AsClientC() {
    return type == ClientField_ClientC ?
      reinterpret_cast<abby::ClientCT *>(value) : nullptr;
  }
  const abby::ClientCT *AsClientC() const {
    return type == ClientField_ClientC ?
      reinterpret_cast<const abby::ClientCT *>(value) : nullptr;
  }
};

bool VerifyClientField(flatbuffers::Verifier &verifier, const void *obj, ClientField type);
bool VerifyClientFieldVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct ClientAT : public flatbuffers::NativeTable {
  typedef ClientA TableType;
  double price = 0.0;
  std::string name{};
};

struct ClientA FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientAT NativeTableType;
  typedef ClientABuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PRICE = 4,
    VT_NAME = 6
  };
  double price() const {
    return GetField<double>(VT_PRICE, 0.0);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<double>(verifier, VT_PRICE) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           verifier.EndTable();
  }
  ClientAT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ClientAT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ClientA> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientAT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ClientABuilder {
  typedef ClientA Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_price(double price) {
    fbb_.AddElement<double>(ClientA::VT_PRICE, price, 0.0);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(ClientA::VT_NAME, name);
  }
  explicit ClientABuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientA> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientA>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientA> CreateClientA(
    flatbuffers::FlatBufferBuilder &_fbb,
    double price = 0.0,
    flatbuffers::Offset<flatbuffers::String> name = 0) {
  ClientABuilder builder_(_fbb);
  builder_.add_price(price);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<ClientA> CreateClientADirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    double price = 0.0,
    const char *name = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return abby::CreateClientA(
      _fbb,
      price,
      name__);
}

flatbuffers::Offset<ClientA> CreateClientA(flatbuffers::FlatBufferBuilder &_fbb, const ClientAT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct ClientBT : public flatbuffers::NativeTable {
  typedef ClientB TableType;
  std::string name{};
  int32_t age = 0;
};

struct ClientB FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientBT NativeTableType;
  typedef ClientBBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_AGE = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int32_t age() const {
    return GetField<int32_t>(VT_AGE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int32_t>(verifier, VT_AGE) &&
           verifier.EndTable();
  }
  ClientBT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ClientBT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ClientB> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientBT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ClientBBuilder {
  typedef ClientB Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(ClientB::VT_NAME, name);
  }
  void add_age(int32_t age) {
    fbb_.AddElement<int32_t>(ClientB::VT_AGE, age, 0);
  }
  explicit ClientBBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientB> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientB>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientB> CreateClientB(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t age = 0) {
  ClientBBuilder builder_(_fbb);
  builder_.add_age(age);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<ClientB> CreateClientBDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t age = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return abby::CreateClientB(
      _fbb,
      name__,
      age);
}

flatbuffers::Offset<ClientB> CreateClientB(flatbuffers::FlatBufferBuilder &_fbb, const ClientBT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct ClientCT : public flatbuffers::NativeTable {
  typedef ClientC TableType;
  std::string name{};
  int32_t weight = 0;
};

struct ClientC FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientCT NativeTableType;
  typedef ClientCBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_WEIGHT = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int32_t weight() const {
    return GetField<int32_t>(VT_WEIGHT, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int32_t>(verifier, VT_WEIGHT) &&
           verifier.EndTable();
  }
  ClientCT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ClientCT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ClientC> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientCT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ClientCBuilder {
  typedef ClientC Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(ClientC::VT_NAME, name);
  }
  void add_weight(int32_t weight) {
    fbb_.AddElement<int32_t>(ClientC::VT_WEIGHT, weight, 0);
  }
  explicit ClientCBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientC> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientC>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientC> CreateClientC(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t weight = 0) {
  ClientCBuilder builder_(_fbb);
  builder_.add_weight(weight);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<ClientC> CreateClientCDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t weight = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return abby::CreateClientC(
      _fbb,
      name__,
      weight);
}

flatbuffers::Offset<ClientC> CreateClientC(flatbuffers::FlatBufferBuilder &_fbb, const ClientCT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct ClientDataT : public flatbuffers::NativeTable {
  typedef ClientData TableType;
  abby::ClientType type = abby::ClientType_ClientA;
  abby::ClientFieldUnion message{};
};

struct ClientData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ClientDataT NativeTableType;
  typedef ClientDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE = 4,
    VT_MESSAGE_TYPE = 6,
    VT_MESSAGE = 8
  };
  abby::ClientType type() const {
    return static_cast<abby::ClientType>(GetField<int8_t>(VT_TYPE, 0));
  }
  abby::ClientField message_type() const {
    return static_cast<abby::ClientField>(GetField<uint8_t>(VT_MESSAGE_TYPE, 0));
  }
  const void *message() const {
    return GetPointer<const void *>(VT_MESSAGE);
  }
  template<typename T> const T *message_as() const;
  const abby::ClientA *message_as_ClientA() const {
    return message_type() == abby::ClientField_ClientA ? static_cast<const abby::ClientA *>(message()) : nullptr;
  }
  const abby::ClientB *message_as_ClientB() const {
    return message_type() == abby::ClientField_ClientB ? static_cast<const abby::ClientB *>(message()) : nullptr;
  }
  const abby::ClientC *message_as_ClientC() const {
    return message_type() == abby::ClientField_ClientC ? static_cast<const abby::ClientC *>(message()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_MESSAGE_TYPE) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           VerifyClientField(verifier, message(), message_type()) &&
           verifier.EndTable();
  }
  ClientDataT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ClientDataT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<ClientData> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientDataT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

template<> inline const abby::ClientA *ClientData::message_as<abby::ClientA>() const {
  return message_as_ClientA();
}

template<> inline const abby::ClientB *ClientData::message_as<abby::ClientB>() const {
  return message_as_ClientB();
}

template<> inline const abby::ClientC *ClientData::message_as<abby::ClientC>() const {
  return message_as_ClientC();
}

struct ClientDataBuilder {
  typedef ClientData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_type(abby::ClientType type) {
    fbb_.AddElement<int8_t>(ClientData::VT_TYPE, static_cast<int8_t>(type), 0);
  }
  void add_message_type(abby::ClientField message_type) {
    fbb_.AddElement<uint8_t>(ClientData::VT_MESSAGE_TYPE, static_cast<uint8_t>(message_type), 0);
  }
  void add_message(flatbuffers::Offset<void> message) {
    fbb_.AddOffset(ClientData::VT_MESSAGE, message);
  }
  explicit ClientDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<ClientData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ClientData>(end);
    return o;
  }
};

inline flatbuffers::Offset<ClientData> CreateClientData(
    flatbuffers::FlatBufferBuilder &_fbb,
    abby::ClientType type = abby::ClientType_ClientA,
    abby::ClientField message_type = abby::ClientField_NONE,
    flatbuffers::Offset<void> message = 0) {
  ClientDataBuilder builder_(_fbb);
  builder_.add_message(message);
  builder_.add_message_type(message_type);
  builder_.add_type(type);
  return builder_.Finish();
}

flatbuffers::Offset<ClientData> CreateClientData(flatbuffers::FlatBufferBuilder &_fbb, const ClientDataT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline ClientAT *ClientA::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<ClientAT>(new ClientAT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ClientA::UnPackTo(ClientAT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = price(); _o->price = _e; }
  { auto _e = name(); if (_e) _o->name = _e->str(); }
}

inline flatbuffers::Offset<ClientA> ClientA::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientAT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateClientA(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ClientA> CreateClientA(flatbuffers::FlatBufferBuilder &_fbb, const ClientAT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ClientAT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _price = _o->price;
  auto _name = _o->name.empty() ? 0 : _fbb.CreateString(_o->name);
  return abby::CreateClientA(
      _fbb,
      _price,
      _name);
}

inline ClientBT *ClientB::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<ClientBT>(new ClientBT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ClientB::UnPackTo(ClientBT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = name(); if (_e) _o->name = _e->str(); }
  { auto _e = age(); _o->age = _e; }
}

inline flatbuffers::Offset<ClientB> ClientB::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientBT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateClientB(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ClientB> CreateClientB(flatbuffers::FlatBufferBuilder &_fbb, const ClientBT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ClientBT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _name = _o->name.empty() ? 0 : _fbb.CreateString(_o->name);
  auto _age = _o->age;
  return abby::CreateClientB(
      _fbb,
      _name,
      _age);
}

inline ClientCT *ClientC::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<ClientCT>(new ClientCT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ClientC::UnPackTo(ClientCT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = name(); if (_e) _o->name = _e->str(); }
  { auto _e = weight(); _o->weight = _e; }
}

inline flatbuffers::Offset<ClientC> ClientC::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientCT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateClientC(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ClientC> CreateClientC(flatbuffers::FlatBufferBuilder &_fbb, const ClientCT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ClientCT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _name = _o->name.empty() ? 0 : _fbb.CreateString(_o->name);
  auto _weight = _o->weight;
  return abby::CreateClientC(
      _fbb,
      _name,
      _weight);
}

inline ClientDataT *ClientData::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = std::unique_ptr<ClientDataT>(new ClientDataT());
  UnPackTo(_o.get(), _resolver);
  return _o.release();
}

inline void ClientData::UnPackTo(ClientDataT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = type(); _o->type = _e; }
  { auto _e = message_type(); _o->message.type = _e; }
  { auto _e = message(); if (_e) _o->message.value = abby::ClientFieldUnion::UnPack(_e, message_type(), _resolver); }
}

inline flatbuffers::Offset<ClientData> ClientData::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ClientDataT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateClientData(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<ClientData> CreateClientData(flatbuffers::FlatBufferBuilder &_fbb, const ClientDataT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ClientDataT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _type = _o->type;
  auto _message_type = _o->message.type;
  auto _message = _o->message.Pack(_fbb);
  return abby::CreateClientData(
      _fbb,
      _type,
      _message_type,
      _message);
}

inline bool VerifyClientField(flatbuffers::Verifier &verifier, const void *obj, ClientField type) {
  switch (type) {
    case ClientField_NONE: {
      return true;
    }
    case ClientField_ClientA: {
      auto ptr = reinterpret_cast<const abby::ClientA *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ClientField_ClientB: {
      auto ptr = reinterpret_cast<const abby::ClientB *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ClientField_ClientC: {
      auto ptr = reinterpret_cast<const abby::ClientC *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyClientFieldVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyClientField(
        verifier,  values->Get(i), types->GetEnum<ClientField>(i))) {
      return false;
    }
  }
  return true;
}

inline void *ClientFieldUnion::UnPack(const void *obj, ClientField type, const flatbuffers::resolver_function_t *resolver) {
  switch (type) {
    case ClientField_ClientA: {
      auto ptr = reinterpret_cast<const abby::ClientA *>(obj);
      return ptr->UnPack(resolver);
    }
    case ClientField_ClientB: {
      auto ptr = reinterpret_cast<const abby::ClientB *>(obj);
      return ptr->UnPack(resolver);
    }
    case ClientField_ClientC: {
      auto ptr = reinterpret_cast<const abby::ClientC *>(obj);
      return ptr->UnPack(resolver);
    }
    default: return nullptr;
  }
}

inline flatbuffers::Offset<void> ClientFieldUnion::Pack(flatbuffers::FlatBufferBuilder &_fbb, const flatbuffers::rehasher_function_t *_rehasher) const {
  switch (type) {
    case ClientField_ClientA: {
      auto ptr = reinterpret_cast<const abby::ClientAT *>(value);
      return CreateClientA(_fbb, ptr, _rehasher).Union();
    }
    case ClientField_ClientB: {
      auto ptr = reinterpret_cast<const abby::ClientBT *>(value);
      return CreateClientB(_fbb, ptr, _rehasher).Union();
    }
    case ClientField_ClientC: {
      auto ptr = reinterpret_cast<const abby::ClientCT *>(value);
      return CreateClientC(_fbb, ptr, _rehasher).Union();
    }
    default: return 0;
  }
}

inline ClientFieldUnion::ClientFieldUnion(const ClientFieldUnion &u) : type(u.type), value(nullptr) {
  switch (type) {
    case ClientField_ClientA: {
      value = new abby::ClientAT(*reinterpret_cast<abby::ClientAT *>(u.value));
      break;
    }
    case ClientField_ClientB: {
      value = new abby::ClientBT(*reinterpret_cast<abby::ClientBT *>(u.value));
      break;
    }
    case ClientField_ClientC: {
      value = new abby::ClientCT(*reinterpret_cast<abby::ClientCT *>(u.value));
      break;
    }
    default:
      break;
  }
}

inline void ClientFieldUnion::Reset() {
  switch (type) {
    case ClientField_ClientA: {
      auto ptr = reinterpret_cast<abby::ClientAT *>(value);
      delete ptr;
      break;
    }
    case ClientField_ClientB: {
      auto ptr = reinterpret_cast<abby::ClientBT *>(value);
      delete ptr;
      break;
    }
    case ClientField_ClientC: {
      auto ptr = reinterpret_cast<abby::ClientCT *>(value);
      delete ptr;
      break;
    }
    default: break;
  }
  value = nullptr;
  type = ClientField_NONE;
}

inline const abby::ClientData *GetClientData(const void *buf) {
  return flatbuffers::GetRoot<abby::ClientData>(buf);
}

inline const abby::ClientData *GetSizePrefixedClientData(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<abby::ClientData>(buf);
}

inline bool VerifyClientDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<abby::ClientData>(nullptr);
}

inline bool VerifySizePrefixedClientDataBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<abby::ClientData>(nullptr);
}

inline void FinishClientDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<abby::ClientData> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedClientDataBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<abby::ClientData> root) {
  fbb.FinishSizePrefixed(root);
}

inline std::unique_ptr<abby::ClientDataT> UnPackClientData(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<abby::ClientDataT>(GetClientData(buf)->UnPack(res));
}

inline std::unique_ptr<abby::ClientDataT> UnPackSizePrefixedClientData(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<abby::ClientDataT>(GetSizePrefixedClientData(buf)->UnPack(res));
}

}  // namespace abby

#endif  // FLATBUFFERS_GENERATED_C2S_ABBY_H_
