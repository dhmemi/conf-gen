/**
 * Copyright dhmemi.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "def.hpp"
#include "nlohmann/json.hpp"
#include <type_traits>

namespace confgen {

using json = nlohmann::json;

struct data_t {
  using Bool = bool;
  using Int = int;
  using F64 = double;
  using Str = std::string;
  using VecBool = std::vector<bool>;
  using VecInt = std::vector<int>;
  using VecF64 = std::vector<double>;
  using VecStr = std::vector<std::string>;
}; // struct data_t

enum permission_t { Show, Advan, Hide, Fixed };

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

enum meta_t {
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

template <typename Dtype>
inline bool equal(const Dtype &a, const Dtype &b) {
  return a == b;
}

template <>
inline bool equal(const float &a, const float &b) {
  return fabs(a - b) < 1e-5;
}

template <>
inline bool equal(const double &a, const double &b) {
  return fabs(a - b) < 1e-8;
}

template <typename Dtype>
inline std::string to_string(const Dtype &a) {
  return std::to_string(a);
}

template <>
inline std::string to_string(const std::string &a) {
  return a;
}

struct item_infos {
  const int index;
  const std::string &data_type;
  const meta_t &meta_type;
  const permission_t &premission;
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

class value_base {
public:
  value_base() : root_(std::make_shared<json>()) { ptr_ = root_.get(); }
  value_base(const value_base &other) = default;
  explicit value_base(std::shared_ptr<json> root, json *ptr = nullptr)
      : root_(std::move(root)), ptr_(ptr) {
    if (nullptr == ptr_) {
      ptr_ = root_.get();
    }
  }

  template <typename Dtype>
  Dtype as() const {
    static_assert(std::is_base_of_v<value_base, Dtype>,
                  "value_base only can be convert to derived type of "
                  "confgen::detail::value_base");
    return Dtype(root_, ptr_);
  }

  CFG_NO_DISCARD const json &get_json() const { return *ptr_; }
  json &get_json() { return *ptr_; }

  friend void to_json(json &j, const value_base &t) { j = *t.ptr_; }
  friend void from_json(const json &j, value_base &t) { *t.ptr_ = j; }

protected:
  std::shared_ptr<json> root_;
  json *ptr_;
};

class item_base : public value_base {
public:
  using value_base::value_base;

#define CFG_ITEM_COMMON_INTERFACE(type, name)                                  \
  CFG_NO_DISCARD CFG_INLINE type name() const {                                \
    return ptr_->at(const_keys::name).get<type>();                             \
  }

  CFG_ITEM_COMMON_INTERFACE(int, index)
  CFG_ITEM_COMMON_INTERFACE(std::string, data_type)
  CFG_ITEM_COMMON_INTERFACE(meta_t, meta_type)
  CFG_ITEM_COMMON_INTERFACE(permission_t, premission)
  CFG_ITEM_COMMON_INTERFACE(std::string, name)
  CFG_ITEM_COMMON_INTERFACE(std::string, comment)
  CFG_ITEM_COMMON_INTERFACE(json, ctrl_info)

#undef CFG_ITEM_COMMON_INTERFACE
};

template <typename Dtype>
class item_with_gsetter : public item_base {
public:
  using item_base::item_base;
  item_with_gsetter(const Dtype &value,
                    const item_infos &infos,
                    const json &ctrl_info = {}) {
    *ptr_ = infos.to_json();
    (*ptr_)[const_keys::ctrl_info] = ctrl_info;
    assert(set(value));
  }

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

#define CFG_ITEM_CONSTRUCT(meta_type)                                          \
  item(int index,                                                              \
       permission_t permission,                                                \
       const std::string &data_type,                                           \
       const value_type &value,                                                \
       const std::string &name,                                                \
       const std::string &comment,                                             \
       const ctrl &ctrl_info)                                                  \
      : detail ::item_with_gsetter<value_type>(                                \
            value,                                                             \
            {index, data_type, meta_type, permission, name, comment},          \
            ctrl_info)

#define CFG_VALID_NEW_VALUE_FUNC()                                             \
  CFG_NO_DISCARD CFG_INLINE bool is_new_value_valid(const value_type &value)   \
      const override {                                                         \
    const json &ctrls = (*this->ptr_).at(const_keys::ctrl_info);               \
    return ctrls.get<ctrl>().valid(value);                                     \
  }

} // namespace detail

template <meta_t MetaType, typename DataType>
class item;

template <>
class item<Check, bool> : public detail::item_with_gsetter<bool> {
public:
  using value_type = bool;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;

  item(int index,
       permission_t permission,
       const std::string &data_type,
       const value_type &value,
       const std::string &name,
       const std::string &comment)
      : detail ::item_with_gsetter<value_type>(
            value, {index, data_type, Check, permission, name, comment}) {}

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<value_type>::is_store_value_valid() &&
           (*ptr_).at(const_keys::value).is_boolean();
  }
};

template <typename Dtype>
class item<Input, Dtype> : public detail::item_with_gsetter<Dtype> {
public:
  using value_type = Dtype;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;

  static_assert(std::is_same_v<value_type, data_t::Int> ||
                    std::is_same_v<value_type, data_t::F64>,
                "Input type only support data type `Int` or `F64`");
  struct ctrl {
    value_type min, max, step;

    CFG_INLINE bool valid(const value_type &value) const {
      return (min <= value && value <= max && [&value, this] {
        double n = double(value - min) / step;
        return detail::equal(round(n), n);
      }());
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl, min, max, step)
  };

  CFG_ITEM_CONSTRUCT(Input) {}
  CFG_VALID_NEW_VALUE_FUNC()

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<value_type>::is_store_value_valid() &&
           (*this->ptr_).at(const_keys::value).is_number();
  }
};

template <>
class item<String, data_t::Str>
    : public detail::item_with_gsetter<data_t::Str> {
public:
  using value_type = data_t::Str;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;
  struct ctrl {
    value_type regex;

    CFG_NO_DISCARD CFG_INLINE bool valid(const value_type &value) const {
      return std::regex_match(value, std::regex(regex));
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl, regex)
  };

  CFG_ITEM_CONSTRUCT(String) {}
  CFG_VALID_NEW_VALUE_FUNC()

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    return detail::item_with_gsetter<value_type>::is_store_value_valid() &&
           (*this->ptr_).at(const_keys::value).is_string();
  }
};

template <typename Dtype>
class item<Select, Dtype> : public detail::item_with_gsetter<Dtype> {
public:
  static_assert(std::is_same_v<Dtype, data_t::Int> ||
                    std::is_same_v<Dtype, data_t::F64> ||
                    std::is_same_v<Dtype, data_t::Str>,
                "Select type only support data type `Int`, `F64` and `Str`");

  using value_type = Dtype;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;

  struct ctrl_item {
    value_type value;
    std::string name;

    friend void to_json(nlohmann ::json &j, const ctrl_item &t) {
      j["name"] = t.name.empty() ? detail::to_string(t.value) : t.name;
      j["value"] = t.value;
    }
    friend void from_json(const nlohmann ::json &j, ctrl_item &t) {
      j.at("name").get_to(t.name);
      j.at("value").get_to(t.value);
    }
  };

  struct ctrl : public std::vector<ctrl_item> {
    using std::vector<ctrl_item>::vector;
    CFG_NO_DISCARD bool valid(const value_type &value) const {
      return std::any_of(
          this->begin(), this->end(), [value](const ctrl_item &choice) {
            return detail::equal(choice.value, value);
          });
    }
  };

  CFG_ITEM_CONSTRUCT(Select) {}
  CFG_VALID_NEW_VALUE_FUNC()

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    if (!detail::item_with_gsetter<value_type>::is_store_value_valid()) {
      return false;
    }
    const json &val = (*this->ptr_).at(const_keys::value);
    return val.is_number() || val.is_string();
  }
};

template <typename Dtype>
class item<Range, Dtype> : public detail::item_with_gsetter<Dtype> {
public:
  static_assert(std::is_same_v<Dtype, data_t::VecInt> ||
                    std::is_same_v<Dtype, data_t::VecF64>,
                "Select type only support data type `VecInt` and `VecF64`.");

  using value_type = Dtype;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;

  struct ctrl {
    typename value_type::value_type l0, r0, l1, r1; // left-0 to right-1.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ctrl, l0, r0, l1, r1)

    CFG_NO_DISCARD CFG_INLINE bool valid(const value_type &value) const {
      return value.size() == 2 && value.at(0) >= l0 && value.at(0) <= r0 &&
             value.at(1) >= l1 && value.at(1) <= r1;
    }
  };

  CFG_ITEM_CONSTRUCT(Range) {}
  CFG_VALID_NEW_VALUE_FUNC()

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    if (!detail::item_with_gsetter<value_type>::is_store_value_valid()) {
      return false;
    }
    const json &val = (*this->ptr_).at(const_keys::value);
    return val.is_array() && val.size() == 2;
  }
};

template <typename Dtype>
class item<Array, Dtype> : public detail::item_with_gsetter<Dtype> {
private:
  template <typename T>
  struct ctrl_map;

  template <>
  struct ctrl_map<data_t::VecInt> {
    using type = item<Input, data_t::Int>::ctrl;
  };

  template <>
  struct ctrl_map<data_t::VecF64> {
    using type = item<Input, data_t::F64>::ctrl;
  };

  template <>
  struct ctrl_map<data_t::VecStr> {
    using type = item<String, data_t::Str>::ctrl;
  };

public:
  static_assert(std::is_same_v<Dtype, data_t::VecInt> ||
                    std::is_same_v<Dtype, data_t::VecF64> ||
                    std::is_same_v<Dtype, data_t::VecStr>,
                "Select type only support data type `VecInt`, `VecF64` "
                "and `VecStr`.");

  using value_type = Dtype;
  using ctrl = typename ctrl_map<Dtype>::type;
  using detail::item_with_gsetter<value_type>::item_with_gsetter;

  CFG_ITEM_CONSTRUCT(Array) {}

  CFG_NO_DISCARD CFG_INLINE bool
  is_new_value_valid(const value_type &value) const override {
    const auto &ctrls =
        (*this->ptr_).at(const_keys::ctrl_info).template get<ctrl>();
    return std::all_of(
        value.begin(),
        value.end(),
        [ctrls](const typename value_type::value_type &single_val) {
          return ctrls.valid(single_val);
        });
  }

  CFG_NO_DISCARD CFG_INLINE bool is_store_value_valid() const override {
    if (!detail::item_with_gsetter<value_type>::is_store_value_valid()) {
      return false;
    }
    const json &val = (*this->ptr_).at(const_keys::value);
    return val.is_array() && val.size() == 2;
  }
};

template <typename Dtype>
class item<Group, Dtype> : public detail::item_base {
public:
  static_assert(std::is_base_of_v<detail::item_base, Dtype>,
                "Group config's data type should be derived type of "
                "confgen::detail::item_base");

  using value_type = Dtype;
  using detail::item_base::item_base;

  item(int index,
       permission_t permission,
       const std::string &data_type,
       const value_type &value,
       const std::string &name,
       const std::string &comment) {
    *this->ptr_ =
        detail::item_infos{index, data_type, Group, permission, name, comment}
            .to_json();
    (*this->ptr_)[const_keys::ctrl_info] = {};
    assert(set(value));
  }

  Dtype get(const Dtype &fallback = {}) {
    if (!is_store_value_valid()) {
      set(fallback);
    }
    return Dtype(this->root_, &(*ptr_)[const_keys::value]);
  }

  CFG_NO_DISCARD bool set(const Dtype &value) {
    if (is_new_value_valid(value)) {
      (*ptr_)[const_keys::value] = value;
      return true;
    }
    return false;
  }

  CFG_NO_DISCARD bool is_new_value_valid(const Dtype &) const { return true; }

  CFG_NO_DISCARD bool is_store_value_valid() const {
    return ptr_->contains(const_keys::value) &&
           ptr_->at(const_keys::value).is_object();
  }
};

template <typename Dtype>
class item<Refer, Dtype> : public detail::item_base {
public:
  using value_type = Dtype;
  using detail::item_base::item_base;

  item(int index,
       permission_t permission,
       const std::string &data_type,
       const std::string &ref_path,
       const std::string &name,
       const std::string &comment) {
    *this->ptr_ =
        detail::item_infos{index, data_type, Group, permission, name, comment}
            .to_json();
    (*this->ptr_)[const_keys::ctrl_info] = {};
    (*ptr_)[const_keys::value] = ref_path;
  }

  Dtype get(const Dtype &fallback = {}) {
    // TODO() solver reference
    return fallback;
  }

  CFG_NO_DISCARD bool set(const Dtype & /*value*/) {
    // TODO() solver reference
    return false;
  }

  CFG_NO_DISCARD bool is_new_value_valid(const Dtype &) const { return false; }

  CFG_NO_DISCARD bool is_store_value_valid() const { return false; }
};

} // namespace confgen