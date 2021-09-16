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

#include <fstream>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "items.hpp"

namespace confgen {

class group : public detail::value_base {
protected:
  CFG_NO_DISCARD CFG_INLINE json *find_ptr(const std::string &name,
                                           bool is_refer = false) const {
    if (is_refer) {
      return ptr_;
    }
    return &(*ptr_).at(name);
  }

public:
  using detail::value_base::value_base;

  template <typename Type = detail::item_base>
  CFG_NO_DISCARD CFG_INLINE Type at(const std::string &name) const {
    return detail::item_base(root_, &(ptr_->at(name))).as<Type>();
  }

  template <typename Type = detail::value_base>
  CFG_NO_DISCARD CFG_INLINE Type value_at(const std::string &name) const {
    return detail::value_base(root_,
                              &(ptr_->at(name).at(detail::key_ns::k_value)))
        .as<Type>();
  }

  CFG_NO_DISCARD CFG_INLINE bool have_enable_item() const {
    return ptr_->contains("enable");
  }

  CFG_NO_DISCARD CFG_INLINE size_t size() const { return ptr_->size(); }

  CFG_NO_DISCARD CFG_INLINE std::vector<std::string> keys() const {
    std::vector<std::string> all_keys;
    for (const auto &i : ptr_->items()) {
      all_keys.push_back(i.key());
    }
    return all_keys;
  }

  group &with_check() {
    this->parse_with_check_ = true;
    return *this;
  };

  group &with_no_check() {
    this->parse_with_check_ = false;
    return *this;
  }
};

} // namespace confgen