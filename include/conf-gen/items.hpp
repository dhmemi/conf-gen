#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "def.hpp"
#include "nlohmann/json.hpp"

namespace confgen {
using nlohmann::json;

using Bool = bool;
using Int = int;
using F64 = double;
using Str = std::string;
using VecBool = std::vector<bool>;
using VecInt = std::vector<int>;
using VecF64 = std::vector<double>;
using VecStr = std::vector<std::string>;

enum class permission { Show, Advan, Hide, Fixed };

namespace const_keys {
const std::string name = "name";
const std::string value = "value";
const std::string data_type = "dtype";
const std::string index = "index";
const std::string comment = "comment";
const std::string premission = "premission";
const std::string meta_type = "mtype";
} // namespace const_keys

enum meta_type {
  Check = 10,  // bool
  Input = 11,  // int, double
  String = 13, // string
  Select = 14, // int, double, string
  Range = 17,  // vec<[int, double]>
  Array = 19,  // vec<[bool, int, double, string]>
  Enums = -14, // it exists just for code color, it will be replaced with Select
               // in parser.
  Refer = 1,   // all
  Group = 2
};

namespace detail {

// template <typename Test, template <typename...> class Ref>
// struct is_specialization : std::false_type {};
// template <template <typename...> class Ref, typename... Args>
// struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

class item_base {
public:
  item_base() : root_(std::make_shared<json>()) { ptr_ = root_.get(); }

  item_base(int index,
            const std::string &data_type,
            const meta_type &meta_type,
            const permission &premission,
            const std::string &name,
            const std::string &comment)
      : item_base() {
    json &item = *ptr_;
    item[const_keys::name] = name;
    item[const_keys::data_type] = data_type;
    item[const_keys::meta_type] = meta_type;
    item[const_keys::index] = index;
    item[const_keys::premission] = premission;
    item[const_keys::comment] = comment;
  }

  item_base(const std::shared_ptr<json> &root, json *ptr)
      : root_(std::move(root)), ptr_(ptr) {}

  CFG_NO_DISCARD CFG_INLINE const json &get_json() const { return *ptr_; }

  template <typename Dtype>
  Dtype get(const Dtype &fallback = {}) const {
    json &item = *ptr_;
    if (item[const_keys::value].is_null()) {
      return fallback;
    }
    return item[const_keys::value].get<Dtype>();
  }

protected:
  std::shared_ptr<json> root_;
  json *ptr_;
};

} // namespace detail

template <meta_type MetaType, typename DataType>
class item : public detail::item_base {
public:
  item() = delete;
  using detail::item_base::item_base;

  item(int index,
       const std::string &data_type,
       const DataType &value,
       const permission &premission,
       const std::string &name,
       const std::string &comment)
      : detail::item_base(
            index, data_type, MetaType, premission, name, comment) {
    set(value);
  }

  CFG_NO_DISCARD CFG_INLINE bool set(const DataType &val) {
    if (check(val)) {
      (*ptr_)[const_keys::value] = val;
    }
  }

  CFG_NO_DISCARD CFG_INLINE DataType get(const DataType &fallback = {}) const {
    detail::item_base::get(fallback);
  }

  CFG_NO_DISCARD CFG_INLINE bool check(const DataType & /*val*/) {
    return false;
  }
};

} // namespace confgen