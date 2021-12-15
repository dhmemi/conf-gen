中文 | [English](README.md)

# Conf-gen

Conf-gen 是一个专为C++打造的配置文件和参数调用接口生成器，目前还在开发中。

# 简单的示例

我们可以像下面这样声明一组参数：
```cpp
#include "conf-gen/generator.hpp"

// clang-format off
CONF_GROUP(BoxSize,
            Input(Int, width, 10, Show, "width", {10, 100, 10}, "The box width, should be multiple of 10 in [10, 100]"),
            Input(Int, height, .value = 50, .ctrl = {10, 200, 20}, .comment = "box's height"));

```

然后，你可以使用你声明的参数类型（在这个示例中是`BoxSize`）来实例化一个参数管理器，并通过`getter`和`setter`接口来设置相应的参数：

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

如果你想将设置后的参数序列化下来，那么你可以这样做：
```cpp
    std::ostringstream oss;

    oss << box_param;

    // or you can puts the param into std::out
    // std::cout << box_param;
```

# 关于支持的界面类型、控制信息及数据类型

// TODO