English | [中文](README.zh-cn.md)

# Conf-gen

Conf-gen is a configuration file and parameter interface generator designed for C++, and it is currently under development. 

# Requirements
- This project is developed based on C++20, so it requires C++20.
- [nlohmann::json](https://github.com/nlohmann/json/) is the only third-party dependent library.

# Supported Compilers
- Tested on Vistul Studio 2019
- Tested on Clang-12
- Tested on GCC-9

# A sample usage

We can declare a group of parameters like below:
```cpp
#include "conf-gen/generator.hpp"

// clang-format off
CONF_GROUP(BoxSize,
            Input(Int, width, 10, Show, "width", {10, 100, 10}, "The box width, should be multiple of 10 in [10, 100]"),
            Input(Int, height, .value = 50, .ctrl = {10, 200, 20}, .comment = "box's height"));

```

Then, you can use the parameter type you declared (in this example, it's `BoxSize`) to instantiate a parameter manager, and set the corresponding parameters through the `getter` and `setter` interfaces: 
```cpp

  BoxSize box_param;

  if(!box_param.set_height(30)) {
      std::cerr << "set height failed!" << std::endl;
  }

  int height = box_param.get_height();
  int width = box_param.get_width();

  // assert(height == 30);
  // assert(width == 50);
```

And you can serialize the parameters like below with stream model in c++: 
```cpp
    std::ostringstream oss;

    oss << box_param;

    // or you can puts the param into std::out
    // std::cout << box_param;
```

# About supported GUI element types, control information and data types 

// TODO