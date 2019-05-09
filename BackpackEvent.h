//
// Created by 苏嘉凯 on 2019-05-03.
//

#ifndef GAMEBACKPACK_BACKPACKEVENT_H
#define GAMEBACKPACK_BACKPACKEVENT_H

#include "Backpack.h"
#include "ItemBar.h"

#include <iostream>

#include <unordered_map>
#include <map>
#include <shared_mutex>
// shared_mutex in C++17, instead of shared_timed_mutex in C++14

class BackpackEvent {
public:
    static BackpackEvent& GetInstance();

    // 1.从背包将道具到身上，如果满，淘汰最久没使用的道具来替换.
    int FromBpToBar(int pos);

    // 2.拾取物品，优先放到身上，如果没有空位，放背包
    int PickItem(const Item & item, uint16_t num = 1);
    int PickItem(Item&& item, uint16_t num = 1);

    // 3.道具使用后打印使用信息，消费类道具扣减数量直至为0，非消费类道具使用后打印使用信息即可
    int UseItem(int pos);

    // 5.组合道具，A,B,C三个道具可以组合为一个D道具，但必须放在身上进行组合，当三个道具全部放到身上后，自动合并为D一个道具
    void SetCombine3(const Item& item1, const Item& item2, const Item& item3, const Item& item4);

    // 其他Api
    // 显示背包所有格子
    void ShowBackpack();
    // 显示物品栏所有格子
    void ShowItemBar();
    // 交换物品栏两个物品的位置
    void ExchangeItemBar(int pos1, int pos2);
    // 丢弃背包内的物品，数量为0表示全部丢弃
    int DropBackpack(int pos, uint16_t num = 0);
    // 丢弃物品栏的物品，数量为0表示全部丢弃
    int DropItemBar(int pos, uint16_t num = 0);

private:
    BackpackEvent();

    void __TestCombine(int id1);

private:
    static std::mutex __m_instance_mutex;
    static BackpackEvent* __m_pInstance;

    Backpack& __m_bp;
    ItemBar& __m_bar;
    std::shared_mutex __m_main_sMutex;

    // 存储组合道具的id，三级索引，一个时间和空间的折中方案:
    // 每次存储三个一级索引在哈希表，二级索引和三级索引为红黑树，二级索引的id小于三级索引的id
    // 即：假设 id1 < id2 < id3
    //    __m_comb_map[id1][id2][id3] = item4;
    //    __m_comb_map[id2][id1][id3] = item4;
    //    __m_comb_map[id3][id2][id3] = item4;
    std::unordered_map<int, std::map<int, std::map<int, Item>>> __m_comb_map;
    std::shared_mutex __m_comb_sMutex;
};


#endif //GAMEBACKPACK_BACKPACKEVENT_H
