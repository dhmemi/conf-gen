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
#include <memory>
#include <stdexcept>
#include <vector>

#include "items.hpp"

namespace confgen {

class group : public detail::value_base {
public:
  using detail::value_base::value_base;

  template <typename Dtype>
  bool set(const std::string &name, const Dtype &val);

  template <typename Dtype>
  Dtype get(const std::string &name, const Dtype &fallback = {}) const;

  CFG_NO_DISCARD json *find_ptr(const std::string &name) const {
    return &(*ptr_).at(name);
  }

  detail::item_base at(std::string &name);

  CFG_NO_DISCARD CFG_INLINE bool have_enable_item() const {
    return ptr_->contains("enable");
  }

  CFG_NO_DISCARD CFG_INLINE size_t size() const { return ptr_->size(); }

  CFG_NO_DISCARD std::vector<std::string> keys() const {
    std::vector<std::string> all_keys;
    for (const auto &i : ptr_->items()) {
      all_keys.push_back(i.key());
    }
    return all_keys;
  }

  CFG_NO_DISCARD bool save_to(const std::string &path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
      return false;
    }
    file << *ptr_;
    return true;
  }

  CFG_NO_DISCARD bool read_from(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
      return false;
    }
    file >> *ptr_;
    return true;
  }
};

} // namespace confgen