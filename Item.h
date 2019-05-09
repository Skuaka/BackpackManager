//
// Created by 苏嘉凯 on 2019-04-30.
//

#ifndef GAMEBACKPACK_ITEM_H
#define GAMEBACKPACK_ITEM_H

#include <string>
#include <functional>

class Item {
public:
    // 物品性质
    struct Property{
        enum Kind : char {
            weapon = 0,
            equipment,
            clothes,

            coin = 64,
            medicine,
        };

        Kind m_kind;
        int16_t m_max;
        /* 非消耗品：m_max <= 0
         * 消耗品：m_max >= 1
         * 假设最大叠加数量为 9999，所以只需要 int16
         * 假设同一种消耗品只能占一个格子
         */

        Property() = default;
        explicit Property(Kind k, int16_t max = -1):
            m_kind(k),
            m_max(max)
        {}
    };
    using Kind = Property::Kind;

public:
    Item();
    Item(int id, const std::string & name, Property pro, std::function<void(void)> f);
    Item(int id, std::string && name, Property pro, std::function<void(void)> f);
    Item(const Item & other);
    Item(Item && other) noexcept;
    Item& operator=(const Item & item) = default;
    Item& operator=(Item && other) noexcept;

    // 使用一次该物品
    void Use();

public:
    int m_id;
    Property m_property;
    std::string m_name;
private:
    std::function<void()> __m_fConsume;
};

#endif //GAMEBACKPACK_ITEM_H
