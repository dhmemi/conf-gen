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

#include "group.hpp"

#define CFG_T(t) t
#define CFG_CONCAT_PRIMITIVE(x, y) x##y
#define CFG_CONCAT(x, y) CFG_CONCAT_PRIMITIVE(x, y)

#define CFG_GEN_REFER_TYPE(Type, cls_name)                                     \
  struct cls_name : confgen::data_t {                                          \
  private:                                                                     \
    static auto helper() -> decltype(Type());                                  \
                                                                               \
  public:                                                                      \
    using type = decltype(helper());                                           \
  }

#define CFG_INIT_META(Meta, Type, InName, ...)                                 \
  (*this->ptr_)[#InName] =                                                     \
      confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(             \
          {++id, confgen::meta_t::Meta, #Type},                                \
          {__VA_ARGS__},                                                       \
          std::string{#InName});

#define CFG_INIT_Check(...) CFG_T(CFG_INIT_META(Check, __VA_ARGS__))
#define CFG_INIT_Input(...) CFG_T(CFG_INIT_META(Input, __VA_ARGS__))
#define CFG_INIT_String(...) CFG_T(CFG_INIT_META(String, __VA_ARGS__))
#define CFG_INIT_Select(...) CFG_T(CFG_INIT_META(Select, __VA_ARGS__))
#define CFG_INIT_Range(...) CFG_T(CFG_INIT_META(Range, __VA_ARGS__))
#define CFG_INIT_Array(...) CFG_T(CFG_INIT_META(Array, __VA_ARGS__))
#define CFG_INIT_Enums(Type, ...) CFG_T(CFG_INIT_META(Select, Int, __VA_ARGS__))

#define CFG_INIT_Group(Type, InName, Value, ...)                               \
  (*this->ptr_)[#InName] = confgen::item<confgen::meta_t::Group, Type>(        \
      {++id, confgen::meta_t::Group, "Group"},                                 \
      {Type{}, __VA_ARGS__},                                                   \
      std::string{#InName});

#define CFG_INIT_Refer(Type, InName, ...)                                      \
  CFG_GEN_REFER_TYPE(Type, cfg_refer_t_##InName);                              \
  (*this->ptr_)[#InName] =                                                     \
      confgen::item<confgen::meta_t::Refer, cfg_refer_t_##InName::type>(       \
          {++id, confgen::meta_t::Refer, "Str"},                               \
          {confgen::meta_t::__VA_ARGS__},                                      \
          std::string{#InName});

/// getter and setter for common items.
#define CFG_OPER_META(Meta, Type, InName, ...)                                 \
  bool set_##InName(const confgen::data_t::Type &value) {                      \
    return confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(        \
               root_, find_ptr(#InName))                                       \
        .set(value);                                                           \
  }                                                                            \
  CFG_NO_DISCARD confgen::data_t::Type get_##InName(                           \
      const confgen::data_t::Type &fallback = confgen::data_t::Type{}) const { \
    return confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(        \
               root_, find_ptr(#InName))                                       \
        .get(fallback);                                                        \
  }

#define CFG_OPER_Check(...) CFG_T(CFG_OPER_META(Check, __VA_ARGS__))
#define CFG_OPER_Input(...) CFG_T(CFG_OPER_META(Input, __VA_ARGS__))
#define CFG_OPER_String(...) CFG_T(CFG_OPER_META(String, __VA_ARGS__))
#define CFG_OPER_Select(...) CFG_T(CFG_OPER_META(Select, __VA_ARGS__))
#define CFG_OPER_Range(...) CFG_T(CFG_OPER_META(Range, __VA_ARGS__))
#define CFG_OPER_Array(...) CFG_T(CFG_OPER_META(Array, __VA_ARGS__))
#define CFG_OPER_Enums(Type, ...) CFG_T(CFG_OPER_META(Select, Int, __VA_ARGS__))

#define CFG_OPER_Group(Type, InName, ...)                                      \
  bool set_##InName(const Type &value) {                                       \
    return confgen::item<confgen::meta_t::Group, Type>(root_,                  \
                                                       find_ptr(#InName))      \
        .set(value);                                                           \
  }                                                                            \
  CFG_NO_DISCARD Type get_##InName(const Type &fallback = Type{}) const {      \
    return confgen::item<confgen::meta_t::Group, Type>(root_,                  \
                                                       find_ptr(#InName))      \
        .get(fallback);                                                        \
  }

#define CFG_OPER_Refer(Type, InName, Value, ...)                               \
private:                                                                       \
  CFG_GEN_REFER_TYPE(Type, cfg_refer_t_##InName);                              \
                                                                               \
public:                                                                        \
  bool set_##InName(const cfg_refer_t_##InName::type &value) {                 \
    return confgen::item<confgen::meta_t::Refer, cfg_refer_t_##InName::type>(  \
               root_, find_ptr(#InName, true))                                 \
        .set<confgen::meta_t::Value>(#InName, value);                          \
  }                                                                            \
  CFG_NO_DISCARD auto get_##InName(                                            \
      const cfg_refer_t_##InName::type &fallback =                             \
          cfg_refer_t_##InName::type{}) const {                                \
    return confgen::item<confgen::meta_t::Refer, cfg_refer_t_##InName::type>(  \
               root_, find_ptr(#InName, true))                                 \
        .get<confgen::meta_t::Value>(#InName, fallback);                       \
  }

// clang-format off
#define CFG_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, \
        _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28,  \
        _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42,  \
        _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56,  \
        _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N     

#define CFG_N(...) CFG_T(CFG_ARG_N(__VA_ARGS__))
#define CFG_COUNT(...)                                                         \
  CFG_N(__VA_ARGS__ , 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,  \
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34,    \
        33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,    \
        16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
// clang-format on

#define CFG_INIT_CONCAT(N, ...) CFG_T(CFG_CONCAT(CFG_INIT_, N)(__VA_ARGS__))
#define CFG_OPER_CONCAT(N, ...) CFG_T(CFG_CONCAT(CFG_OPER_, N)(__VA_ARGS__))

#define CFG_INIT_ALL(N, ...) CFG_INIT_CONCAT(N, __VA_ARGS__)
#define CFG_OPER_ALL(N, ...) CFG_OPER_CONCAT(N, __VA_ARGS__)

#define CONF_GROUP(Confgen, ...)                                               \
  class Confgen : public confgen::group {                                      \
  public:                                                                      \
    Confgen() : confgen::group() {                                             \
      this->parse_with_check_ = true;                                          \
      using confgen::permission_t::Show;                                       \
      using confgen::permission_t::Advan;                                      \
      using confgen::permission_t::Hide;                                       \
      using confgen::permission_t::Fixed;                                      \
      int id = -1;                                                             \
      CFG_INIT_ALL(CFG_COUNT(__VA_ARGS__), __VA_ARGS__)                        \
    }                                                                          \
    CFG_OPER_ALL(CFG_COUNT(__VA_ARGS__), __VA_ARGS__)                          \
    explicit Confgen(const confgen::group &cf_group)                           \
        : confgen::group(cf_group) {}                                          \
    explicit Confgen(const std::shared_ptr<confgen::json> &j,                  \
                     confgen::json *p = nullptr,                               \
                     bool parse_check = true)                                  \
        : confgen::group(j, p) {                                               \
      this->parse_with_check_ = parse_check;                                   \
    }                                                                          \
  }

#define CFG_INIT_1(P, ...) CFG_CONCAT(CFG_INIT_, P)
#define CFG_OPER_1(P, ...) CFG_CONCAT(CFG_OPER_, P)

// macro loop
#define CFG_INIT_2(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_1(__VA_ARGS__))
#define CFG_OPER_2(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_1(__VA_ARGS__))
#define CFG_INIT_3(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_2(__VA_ARGS__))
#define CFG_OPER_3(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_2(__VA_ARGS__))
#define CFG_INIT_4(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_3(__VA_ARGS__))
#define CFG_OPER_4(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_3(__VA_ARGS__))
#define CFG_INIT_5(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_4(__VA_ARGS__))
#define CFG_OPER_5(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_4(__VA_ARGS__))
#define CFG_INIT_6(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_5(__VA_ARGS__))
#define CFG_OPER_6(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_5(__VA_ARGS__))
#define CFG_INIT_7(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_6(__VA_ARGS__))
#define CFG_OPER_7(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_6(__VA_ARGS__))
#define CFG_INIT_8(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_7(__VA_ARGS__))
#define CFG_OPER_8(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_7(__VA_ARGS__))
#define CFG_INIT_9(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_8(__VA_ARGS__))
#define CFG_OPER_9(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_8(__VA_ARGS__))
#define CFG_INIT_10(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_9(__VA_ARGS__))
#define CFG_OPER_10(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_9(__VA_ARGS__))
#define CFG_INIT_11(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_10(__VA_ARGS__))
#define CFG_OPER_11(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_10(__VA_ARGS__))
#define CFG_INIT_12(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_11(__VA_ARGS__))
#define CFG_OPER_12(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_11(__VA_ARGS__))
#define CFG_INIT_13(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_12(__VA_ARGS__))
#define CFG_OPER_13(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_12(__VA_ARGS__))
#define CFG_INIT_14(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_13(__VA_ARGS__))
#define CFG_OPER_14(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_13(__VA_ARGS__))
#define CFG_INIT_15(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_14(__VA_ARGS__))
#define CFG_OPER_15(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_14(__VA_ARGS__))
#define CFG_INIT_16(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_15(__VA_ARGS__))
#define CFG_OPER_16(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_15(__VA_ARGS__))
#define CFG_INIT_17(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_16(__VA_ARGS__))
#define CFG_OPER_17(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_16(__VA_ARGS__))
#define CFG_INIT_18(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_17(__VA_ARGS__))
#define CFG_OPER_18(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_17(__VA_ARGS__))
#define CFG_INIT_19(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_18(__VA_ARGS__))
#define CFG_OPER_19(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_18(__VA_ARGS__))
#define CFG_INIT_20(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_19(__VA_ARGS__))
#define CFG_OPER_20(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_19(__VA_ARGS__))
#define CFG_INIT_21(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_20(__VA_ARGS__))
#define CFG_OPER_21(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_20(__VA_ARGS__))
#define CFG_INIT_22(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_21(__VA_ARGS__))
#define CFG_OPER_22(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_21(__VA_ARGS__))
#define CFG_INIT_23(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_22(__VA_ARGS__))
#define CFG_OPER_23(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_22(__VA_ARGS__))
#define CFG_INIT_24(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_23(__VA_ARGS__))
#define CFG_OPER_24(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_23(__VA_ARGS__))
#define CFG_INIT_25(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_24(__VA_ARGS__))
#define CFG_OPER_25(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_24(__VA_ARGS__))
#define CFG_INIT_26(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_25(__VA_ARGS__))
#define CFG_OPER_26(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_25(__VA_ARGS__))
#define CFG_INIT_27(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_26(__VA_ARGS__))
#define CFG_OPER_27(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_26(__VA_ARGS__))
#define CFG_INIT_28(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_27(__VA_ARGS__))
#define CFG_OPER_28(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_27(__VA_ARGS__))
#define CFG_INIT_29(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_28(__VA_ARGS__))
#define CFG_OPER_29(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_28(__VA_ARGS__))
#define CFG_INIT_30(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_29(__VA_ARGS__))
#define CFG_OPER_30(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_29(__VA_ARGS__))
#define CFG_INIT_31(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_30(__VA_ARGS__))
#define CFG_OPER_31(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_30(__VA_ARGS__))
#define CFG_INIT_32(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_31(__VA_ARGS__))
#define CFG_OPER_32(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_31(__VA_ARGS__))
#define CFG_INIT_33(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_32(__VA_ARGS__))
#define CFG_OPER_33(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_32(__VA_ARGS__))
#define CFG_INIT_34(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_33(__VA_ARGS__))
#define CFG_OPER_34(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_33(__VA_ARGS__))
#define CFG_INIT_35(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_34(__VA_ARGS__))
#define CFG_OPER_35(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_34(__VA_ARGS__))
#define CFG_INIT_36(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_35(__VA_ARGS__))
#define CFG_OPER_36(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_35(__VA_ARGS__))
#define CFG_INIT_37(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_36(__VA_ARGS__))
#define CFG_OPER_37(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_36(__VA_ARGS__))
#define CFG_INIT_38(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_37(__VA_ARGS__))
#define CFG_OPER_38(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_37(__VA_ARGS__))
#define CFG_INIT_39(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_38(__VA_ARGS__))
#define CFG_OPER_39(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_38(__VA_ARGS__))
#define CFG_INIT_40(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_39(__VA_ARGS__))
#define CFG_OPER_40(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_39(__VA_ARGS__))
#define CFG_INIT_41(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_40(__VA_ARGS__))
#define CFG_OPER_41(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_40(__VA_ARGS__))
#define CFG_INIT_42(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_41(__VA_ARGS__))
#define CFG_OPER_42(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_41(__VA_ARGS__))
#define CFG_INIT_43(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_42(__VA_ARGS__))
#define CFG_OPER_43(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_42(__VA_ARGS__))
#define CFG_INIT_44(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_43(__VA_ARGS__))
#define CFG_OPER_44(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_43(__VA_ARGS__))
#define CFG_INIT_45(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_44(__VA_ARGS__))
#define CFG_OPER_45(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_44(__VA_ARGS__))
#define CFG_INIT_46(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_45(__VA_ARGS__))
#define CFG_OPER_46(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_45(__VA_ARGS__))
#define CFG_INIT_47(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_46(__VA_ARGS__))
#define CFG_OPER_47(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_46(__VA_ARGS__))
#define CFG_INIT_48(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_47(__VA_ARGS__))
#define CFG_OPER_48(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_47(__VA_ARGS__))
#define CFG_INIT_49(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_48(__VA_ARGS__))
#define CFG_OPER_49(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_48(__VA_ARGS__))
#define CFG_INIT_50(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_49(__VA_ARGS__))
#define CFG_OPER_50(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_49(__VA_ARGS__))
#define CFG_INIT_51(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_50(__VA_ARGS__))
#define CFG_OPER_51(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_50(__VA_ARGS__))
#define CFG_INIT_52(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_51(__VA_ARGS__))
#define CFG_OPER_52(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_51(__VA_ARGS__))
#define CFG_INIT_53(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_52(__VA_ARGS__))
#define CFG_OPER_53(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_52(__VA_ARGS__))
#define CFG_INIT_54(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_53(__VA_ARGS__))
#define CFG_OPER_54(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_53(__VA_ARGS__))
#define CFG_INIT_55(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_54(__VA_ARGS__))
#define CFG_OPER_55(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_54(__VA_ARGS__))
#define CFG_INIT_56(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_55(__VA_ARGS__))
#define CFG_OPER_56(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_55(__VA_ARGS__))
#define CFG_INIT_57(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_56(__VA_ARGS__))
#define CFG_OPER_57(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_56(__VA_ARGS__))
#define CFG_INIT_58(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_57(__VA_ARGS__))
#define CFG_OPER_58(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_57(__VA_ARGS__))
#define CFG_INIT_59(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_58(__VA_ARGS__))
#define CFG_OPER_59(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_58(__VA_ARGS__))
#define CFG_INIT_60(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_59(__VA_ARGS__))
#define CFG_OPER_60(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_59(__VA_ARGS__))
#define CFG_INIT_61(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_60(__VA_ARGS__))
#define CFG_OPER_61(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_60(__VA_ARGS__))
#define CFG_INIT_62(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_61(__VA_ARGS__))
#define CFG_OPER_62(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_61(__VA_ARGS__))
#define CFG_INIT_63(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_62(__VA_ARGS__))
#define CFG_OPER_63(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_62(__VA_ARGS__))
#define CFG_INIT_64(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_63(__VA_ARGS__))
#define CFG_OPER_64(P, ...) CFG_OPER_1(P) CFG_T(CFG_OPER_63(__VA_ARGS__))