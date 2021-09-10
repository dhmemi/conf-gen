#pragma once

#include <functional>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "def.hpp"
#include "nlohmann/json.hpp"
#include <type_traits>

using json = nlohmann::json;
namespace confgen {

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
const std::string ctrl_info = "ctrls";
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

struct item_infos {
  const int index;
  const std::string &data_type;
  const meta_type &meta_type;
  const permission &premission;
  const std::string &name;
  const std::string &comment;

  CFG_INLINE CFG_NO_DISCARD json to_json() const {
    json item;
    item[const_keys::name] = name;
    item[const_keys::data_type] = data_type;
    item[const_keys::meta_type] = meta_type;
    item[const_keys::index] = index;
    item[const_keys::premission] = premission;
    item[const_keys::comment] = comment;
    return item;
  }
};

// template <typename Test, template <typename...> class Ref>
// struct is_specialization : std::false_type {};
// template <template <typename...> class Ref, typename... Args>
// struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

class item_base {
public:
  item_base() : root_(std::make_shared<json>()) { ptr_ = root_.get(); }

  explicit item_base(const item_infos &infos) : item_base() {
    *ptr_ = infos.to_json();
  }

  item_base(const std::shared_ptr<json> &root, json *ptr)
      : root_(std::move(root)), ptr_(ptr) {}

  CFG_NO_DISCARD CFG_INLINE const json &get_json() const { return *ptr_; }

protected:
  std::shared_ptr<json> root_;
  json *ptr_;
};

template <typename Dtype>
class item_with_gsetter : public item_base {
public:
  using item_base::item_base;

  Dtype get(const Dtype &fallback = {}) {
    if (is_store_value_valid()) {
      return (*ptr_).at(const_keys::value).get<Dtype>();
    }
    return fallback;
  }

  CFG_NO_DISCARD bool set(const Dtype &value) {
    if (is_new_value_valid(value)) {
      (*ptr_)[const_keys::value] = value;
      return true;
    }
    return false;
  }

  CFG_NO_DISCARD virtual bool is_new_value_valid(const Dtype &) const {
    return true;
  }

  CFG_NO_DISCARD virtual bool is_store_value_valid() const {
    return (*ptr_).contains(const_keys::value);
  }
};

} // namespace detail

template <meta_type MetaType, typename DataType>
class item;

template <>
class item<Check, bool> : public detail::item_with_gsetter<bool> {
public:
  item(bool value, const detail::item_infos &infos)
      : detail::item_with_gsetter<bool>(infos) {
    assert(set(value));
  }
  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<bool>::is_store_value_valid() &&
           (*ptr_).at(const_keys::value).is_boolean();
  }
};

template <typename Dtype>
class item<Input, Dtype> : public detail::item_with_gsetter<Dtype> {
public:
  static_assert(std::is_same_v<Dtype, Int> || std::is_same_v<Dtype, F64>,
                "Input type only support data type `Int` or `F64`");
  struct ctrl {
    Dtype min, max, step;

    CFG_INLINE bool valid(const Dtype &value) const {
      return (min <= value && value <= max && [&value, this] {
        double n = double(value - min) / step;
        return fabs(round(n) - n) < 1e-6;
      }());
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl, min, max, step)
  };

  item(Dtype value, const detail::item_infos &infos, const ctrl &ctrl_data)
      : detail::item_with_gsetter<Dtype>(infos) {
    (*this->ptr_)[const_keys::ctrl_info] = ctrl_data;
    assert(set(value));
  }

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<bool>::is_store_value_valid() &&
           (*this->ptr_).at(const_keys::value).is_number();
  }

  CFG_NO_DISCARD CFG_INLINE bool
  is_new_value_valid(const Dtype &value) const override {
    const json &ctrls = (*this->ptr_).at(const_keys::ctrl_info);
    return ctrls.get<ctrl>().valid(value);
  }
};

template <>
class item<String, Str> : public detail::item_with_gsetter<Str> {
public:
  struct ctrl {
    Str regex;

    CFG_NO_DISCARD CFG_INLINE bool valid(const Str &value) const {
      return std::regex_match(value, std::regex(regex));
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl, regex)
  };

  item(const Str &value, const detail::item_infos &infos, const Str &ctrl_data)
      : detail::item_with_gsetter<Str>(infos) {
    (*this->ptr_)[const_keys::ctrl_info] = ctrl_data;
    assert(set(value));
  }

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<Str>::is_store_value_valid() &&
           (*this->ptr_).at(const_keys::value).is_string();
  }

  CFG_NO_DISCARD CFG_INLINE bool
  is_new_value_valid(const Str &value) const override {
    const json &ctrls = (*this->ptr_).at(const_keys::ctrl_info);
    return ctrls.get<ctrl>().valid(value);
  }
};

template <typename Dtype>
class item<Select, Dtype> : public detail::item_with_gsetter<Dtype> {
public:
  static_assert(std::is_same_v<Dtype, Int> || std::is_same_v<Dtype, F64> ||
                    std::is_same_v<Dtype, Str>,
                "Select type only support data type `Int`, `F64` and `Str`");

  struct ctrl_item {
    Str name;
    Dtype value;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl_item, name, value)
  };

  struct ctrl : public std::vector<ctrl_item> {
    CFG_NO_DISCARD bool valid() const {
      return true; // TODO()
    }
  };

  item(Dtype value,
       const detail::item_infos &infos,
       const std::vector<std::pair<Str, Dtype>> &select_items)
      : detail::item_with_gsetter<Dtype>(infos) {
    (*this->ptr_)[const_keys::ctrl_info] = select_items;
    assert(set(value));
  }

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    if (detail::item_with_gsetter<bool>::is_store_value_valid()) {
      return false;
    }
    const json &val = (*this->ptr_).at(const_keys::value);
    return val.is_number() || val.is_string();
  }

  CFG_NO_DISCARD CFG_INLINE bool
  is_new_value_valid(const Dtype &value) const override {
    const Json::Value &j_sel = (*this->head_)[N::ctrl_info]["selects"];
    std::vector<DType> values(j_sel.size());
    for (int i = 0, sz = j_sel.size(); i < sz; ++i) {
      values[i] = json2cpp<DType>(j_sel[i]["value"]);
    }
    return std::find(values.begin(), values.end(), val) != values.end();
  }
};

} // namespace confgen