
#pragma once

#include "group.hpp"

#define CFG_T(t) t
#define CFG_CONCAT_PRIMITIVE(x, y) x##y
#define CFG_CONCAT(x, y) CFG_CONCAT_PRIMITIVE(x, y)

#define CFG_INIT_META(Meta, Type, InName, ...)                                 \
  (*ptr_)[#InName] =                                                           \
      confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(             \
          ++id, #Type, __VA_ARGS__);

#define CFG_INIT_Check(...) CFG_T(CFG_INIT_META(Check, __VA_ARGS__))
#define CFG_INIT_Input(...) CFG_T(CFG_INIT_META(Input, __VA_ARGS__))
#define CFG_INIT_String(...) CFG_T(CFG_INIT_META(String, __VA_ARGS__))
#define CFG_INIT_Select(...) CFG_T(CFG_INIT_META(Select, __VA_ARGS__))
#define CFG_INIT_Range(...) CFG_T(CFG_INIT_META(Range, __VA_ARGS__))
#define CFG_INIT_Array(...) CFG_T(CFG_INIT_META(Array, __VA_ARGS__))
#define CFG_INIT_Enums(Type, ...) CFG_T(CFG_INIT_META(Select, Int, __VA_ARGS__))

#define PMG_PARAM_INIT_Group(Type, InName, Value, ...)                         \
  (*ptr_)[#InName] = confgen::item<confgen::meta_t::Group, Type>(              \
      ++id, "Group", Type{}, __VA_ARGS__);

#define PMG_PARAM_INIT_Refer(Type, InName, ...)                                \
  (*ptr_)[#InName] =                                                           \
      confgen::item<confgen::meta_t::Refer, Type>(++id, "Str", __VA_ARGS__);

/// getter and setter for common items.
#define CFG_IMPL_META(Meta, Type, InName, ...)                                 \
  bool set_##InName(const confgen::data_t::Type &value) {                      \
    return confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(        \
               root_, get_ptr(#InName))                                        \
        .set(value);                                                           \
  }                                                                            \
  confgen::data_t::Type get_##InName(const confgen::data_t::Type &fallback =   \
                                         confgen::data_t::Type{}) const {      \
    return confgen::item<confgen::meta_t::Meta, confgen::data_t::Type>(        \
               root_, get_ptr(#InName))                                        \
        .get(fallback);                                                        \
  }

#define CFG_IMPL_Check(...) CFG_T(CFG_IMPL_META(Check, __VA_ARGS__))
#define CFG_IMPL_Input(...) CFG_T(CFG_IMPL_META(Input, __VA_ARGS__))
#define CFG_IMPL_String(...) CFG_T(CFG_IMPL_META(String, __VA_ARGS__))
#define CFG_IMPL_Select(...) CFG_T(CFG_IMPL_META(Select, __VA_ARGS__))
#define CFG_IMPL_Range(...) CFG_T(CFG_IMPL_META(Range, __VA_ARGS__))
#define CFG_IMPL_Array(...) CFG_T(CFG_IMPL_META(Array, __VA_ARGS__))
#define CFG_IMPL_Enums(Type, ...) CFG_T(CFG_IMPL_META(Select, Int, __VA_ARGS__))

#define CFG_IMPL_Group(Type, InName, ...)                                      \
  bool set_##InName(const Type &_val) { return set(#InName, _val); }           \
  Type get_##InName(const Type &fallback = Type{}) const {                     \
    return get(#InName, fallback);                                             \
  }

#define CFG_IMPL_Refer(...) CFG_T(CFG_IMPL_META(__VA_ARGS__))
#define CFG_IMPL_Enums(Type, InName, ...)                                      \
  bool set_##InName(const Type &_val) {                                        \
    return set<confgen::data_t::Int>(#InName,                                  \
                                     static_cast<confgen::data_t::Int>(_val)); \
  }                                                                            \
  Type get_##InName(const Type &fallback = Type{}) const {                     \
    return static_cast<Type>(get<confgen::data_t::Int>(#InName, fallback));    \
  }

#define CFG_INIT_2(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_1(__VA_ARGS__))
#define CFG_IMPL_2(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_1(__VA_ARGS__))
#define CFG_INIT_3(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_2(__VA_ARGS__))
#define CFG_IMPL_3(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_2(__VA_ARGS__))
#define CFG_INIT_4(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_3(__VA_ARGS__))
#define CFG_IMPL_4(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_3(__VA_ARGS__))
#define CFG_INIT_5(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_4(__VA_ARGS__))
#define CFG_IMPL_5(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_4(__VA_ARGS__))
#define CFG_INIT_6(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_5(__VA_ARGS__))
#define CFG_IMPL_6(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_5(__VA_ARGS__))
#define CFG_INIT_7(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_6(__VA_ARGS__))
#define CFG_IMPL_7(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_6(__VA_ARGS__))
#define CFG_INIT_8(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_7(__VA_ARGS__))
#define CFG_IMPL_8(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_7(__VA_ARGS__))
#define CFG_INIT_9(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_8(__VA_ARGS__))
#define CFG_IMPL_9(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_8(__VA_ARGS__))
#define CFG_INIT_10(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_9(__VA_ARGS__))
#define CFG_IMPL_10(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_9(__VA_ARGS__))
#define CFG_INIT_11(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_10(__VA_ARGS__))
#define CFG_IMPL_11(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_10(__VA_ARGS__))
#define CFG_INIT_12(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_11(__VA_ARGS__))
#define CFG_IMPL_12(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_11(__VA_ARGS__))
#define CFG_INIT_13(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_12(__VA_ARGS__))
#define CFG_IMPL_13(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_12(__VA_ARGS__))
#define CFG_INIT_14(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_13(__VA_ARGS__))
#define CFG_IMPL_14(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_13(__VA_ARGS__))
#define CFG_INIT_15(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_14(__VA_ARGS__))
#define CFG_IMPL_15(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_14(__VA_ARGS__))
#define CFG_INIT_16(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_15(__VA_ARGS__))
#define CFG_IMPL_16(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_15(__VA_ARGS__))
#define CFG_INIT_17(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_16(__VA_ARGS__))
#define CFG_IMPL_17(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_16(__VA_ARGS__))
#define CFG_INIT_18(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_17(__VA_ARGS__))
#define CFG_IMPL_18(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_17(__VA_ARGS__))
#define CFG_INIT_19(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_18(__VA_ARGS__))
#define CFG_IMPL_19(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_18(__VA_ARGS__))
#define CFG_INIT_20(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_19(__VA_ARGS__))
#define CFG_IMPL_20(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_19(__VA_ARGS__))
#define CFG_INIT_21(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_20(__VA_ARGS__))
#define CFG_IMPL_21(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_20(__VA_ARGS__))
#define CFG_INIT_22(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_21(__VA_ARGS__))
#define CFG_IMPL_22(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_21(__VA_ARGS__))
#define CFG_INIT_23(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_22(__VA_ARGS__))
#define CFG_IMPL_23(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_22(__VA_ARGS__))
#define CFG_INIT_24(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_23(__VA_ARGS__))
#define CFG_IMPL_24(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_23(__VA_ARGS__))
#define CFG_INIT_25(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_24(__VA_ARGS__))
#define CFG_IMPL_25(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_24(__VA_ARGS__))
#define CFG_INIT_26(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_25(__VA_ARGS__))
#define CFG_IMPL_26(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_25(__VA_ARGS__))
#define CFG_INIT_27(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_26(__VA_ARGS__))
#define CFG_IMPL_27(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_26(__VA_ARGS__))
#define CFG_INIT_28(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_27(__VA_ARGS__))
#define CFG_IMPL_28(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_27(__VA_ARGS__))
#define CFG_INIT_29(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_28(__VA_ARGS__))
#define CFG_IMPL_29(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_28(__VA_ARGS__))
#define CFG_INIT_30(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_29(__VA_ARGS__))
#define CFG_IMPL_30(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_29(__VA_ARGS__))
#define CFG_INIT_31(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_30(__VA_ARGS__))
#define CFG_IMPL_31(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_30(__VA_ARGS__))
#define CFG_INIT_32(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_31(__VA_ARGS__))
#define CFG_IMPL_32(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_31(__VA_ARGS__))
#define CFG_INIT_33(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_32(__VA_ARGS__))
#define CFG_IMPL_33(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_32(__VA_ARGS__))
#define CFG_INIT_34(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_33(__VA_ARGS__))
#define CFG_IMPL_34(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_33(__VA_ARGS__))
#define CFG_INIT_35(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_34(__VA_ARGS__))
#define CFG_IMPL_35(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_34(__VA_ARGS__))
#define CFG_INIT_36(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_35(__VA_ARGS__))
#define CFG_IMPL_36(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_35(__VA_ARGS__))
#define CFG_INIT_37(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_36(__VA_ARGS__))
#define CFG_IMPL_37(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_36(__VA_ARGS__))
#define CFG_INIT_38(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_37(__VA_ARGS__))
#define CFG_IMPL_38(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_37(__VA_ARGS__))
#define CFG_INIT_39(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_38(__VA_ARGS__))
#define CFG_IMPL_39(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_38(__VA_ARGS__))
#define CFG_INIT_40(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_39(__VA_ARGS__))
#define CFG_IMPL_40(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_39(__VA_ARGS__))
#define CFG_INIT_41(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_40(__VA_ARGS__))
#define CFG_IMPL_41(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_40(__VA_ARGS__))
#define CFG_INIT_42(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_41(__VA_ARGS__))
#define CFG_IMPL_42(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_41(__VA_ARGS__))
#define CFG_INIT_43(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_42(__VA_ARGS__))
#define CFG_IMPL_43(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_42(__VA_ARGS__))
#define CFG_INIT_44(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_43(__VA_ARGS__))
#define CFG_IMPL_44(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_43(__VA_ARGS__))
#define CFG_INIT_45(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_44(__VA_ARGS__))
#define CFG_IMPL_45(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_44(__VA_ARGS__))
#define CFG_INIT_46(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_45(__VA_ARGS__))
#define CFG_IMPL_46(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_45(__VA_ARGS__))
#define CFG_INIT_47(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_46(__VA_ARGS__))
#define CFG_IMPL_47(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_46(__VA_ARGS__))
#define CFG_INIT_48(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_47(__VA_ARGS__))
#define CFG_IMPL_48(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_47(__VA_ARGS__))
#define CFG_INIT_49(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_48(__VA_ARGS__))
#define CFG_IMPL_49(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_48(__VA_ARGS__))
#define CFG_INIT_50(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_49(__VA_ARGS__))
#define CFG_IMPL_50(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_49(__VA_ARGS__))
#define CFG_INIT_51(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_50(__VA_ARGS__))
#define CFG_IMPL_51(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_50(__VA_ARGS__))
#define CFG_INIT_52(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_51(__VA_ARGS__))
#define CFG_IMPL_52(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_51(__VA_ARGS__))
#define CFG_INIT_53(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_52(__VA_ARGS__))
#define CFG_IMPL_53(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_52(__VA_ARGS__))
#define CFG_INIT_54(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_53(__VA_ARGS__))
#define CFG_IMPL_54(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_53(__VA_ARGS__))
#define CFG_INIT_55(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_54(__VA_ARGS__))
#define CFG_IMPL_55(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_54(__VA_ARGS__))
#define CFG_INIT_56(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_55(__VA_ARGS__))
#define CFG_IMPL_56(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_55(__VA_ARGS__))
#define CFG_INIT_57(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_56(__VA_ARGS__))
#define CFG_IMPL_57(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_56(__VA_ARGS__))
#define CFG_INIT_58(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_57(__VA_ARGS__))
#define CFG_IMPL_58(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_57(__VA_ARGS__))
#define CFG_INIT_59(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_58(__VA_ARGS__))
#define CFG_IMPL_59(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_58(__VA_ARGS__))
#define CFG_INIT_60(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_59(__VA_ARGS__))
#define CFG_IMPL_60(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_59(__VA_ARGS__))
#define CFG_INIT_61(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_60(__VA_ARGS__))
#define CFG_IMPL_61(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_60(__VA_ARGS__))
#define CFG_INIT_62(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_61(__VA_ARGS__))
#define CFG_IMPL_62(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_61(__VA_ARGS__))
#define CFG_INIT_63(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_62(__VA_ARGS__))
#define CFG_IMPL_63(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_62(__VA_ARGS__))
#define CFG_INIT_64(P, ...) CFG_INIT_1(P) CFG_T(CFG_INIT_63(__VA_ARGS__))
#define CFG_IMPL_64(P, ...) CFG_IMPL_1(P) CFG_T(CFG_IMPL_63(__VA_ARGS__))