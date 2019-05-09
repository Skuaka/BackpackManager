//
// Created by 苏嘉凯 on 2019-04-29.
//

#ifndef GAMEBACKPACK_BACKPACK_H
#define GAMEBACKPACK_BACKPACK_H

#include "StorageRoom.h"

#include <mutex>
#include <shared_mutex>
// shared_mutex in C++17, instead of shared_timed_mutex in C++14

const static unsigned BP_CAPACITY = 60 ;

class Backpack : public StorageRoom{
public:
    static Backpack& GetInstance();

    int AddItem(const Item &item, uint16_t num = 1);
    int AddItem(Item &&item, uint16_t num = 1);
    int AddItem(int pos, const Item &item, uint16_t num = 1);
    int AddItem(int pos, Item &&item, uint16_t num = 1);

    int DelItem(int pos, uint16_t num = 0);

    Item GetItem(int pos) const;
    uint16_t GetItemNum(int pos) const;
    std::pair<Item, uint16_t> GetItemInfo(int pos) const;

//      todo: 整理背包，同一种类的物品靠在一起并根据id排序。
//      用优先队列，需要的时候再写
//    void SortBackpack();

private:
    Backpack();

private:
    static std::mutex __m_instance_mutex;
    static Backpack* __m_pInstance;
};

#endif //GAMEBACKPACK_BACKPACK_H
