#pragma once

#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "items.hpp"

namespace confgen {

class group : public detail::item_base {
public:
  using detail::item_base::item_base;

  template <typename Dtype>
  bool set(const std::string &name, const Dtype &val);

  template <typename Dtype>
  Dtype get(const std::string &name, const Dtype &fallback = {});

  detail::item_base at(std::string &name);

  bool have_enable_item();

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