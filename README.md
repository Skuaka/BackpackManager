# BackpackManager

这是某游戏公司曾经的一道笔试题目，实现背包管理系统。我也参与了这次笔试，放上代码~

#### 类说明

| class         | instruction                                                  |
| ------------- | ------------------------------------------------------------ |
| Item          | 表示一个物品的所有性质，是背包和物品栏管理的基本数据结构。   |
| StorageRoom   | 存储室，是物品栏和背包栏的基类，定义了基本的增删查以及errno。 |
| Backpack      | 背包栏，单例模式，继承于StorageRoom，没有新方法。            |
| ItemBar       | 物品栏，单例模式，继承于StorageRoom，添加了一些物品栏特有的方法。 |
| BackpackEvent | 是整个背包系统的外部接口Api，内部使用了Backpack和ItemBar，操作都上锁了。内部还有一个存储组合道具的三级索引map。 |

#### 其他文件

`ItemExample.h` 定义了一些物品，方便测试。

`main.c` 测试代码

#### c++标准

因为用到了shared_mutex，所以需要c++17支持。

#### run

```
make
./game
make clean
```

