//
// Created by 苏嘉凯 on 2019-04-29.
//

#ifndef GAMEBACKPACK_ITEMBAR_H
#define GAMEBACKPACK_ITEMBAR_H

#include "StorageRoom.h"

#include <mutex>
#include <limits>

const static unsigned BAR_CAPACITY = 8;

class ItemBar : public StorageRoom{
public:
    static ItemBar& GetInstance();

    // 物品栏需要的特有添加方法，淘汰最久未使用的物品
    // 返回淘汰的Item和num，
    // 如果不需要淘汰则返回 num 为 0，id 为 0。
    // 如果出错，返回 num 为 0，id 为错误码 < 0。
    std::pair<Item, uint16_t> SqueezeItem(const Item &item, uint16_t num = 1);
    std::pair<Item, uint16_t> SqueezeItem(Item &&item, uint16_t num = 1);

    int AddItem(const Item &item, uint16_t num = 1);
    int AddItem(Item &&item, uint16_t num = 1);
    int AddItem(int pos, const Item &item, uint16_t num = 1);
    int AddItem(int pos, Item &&item, uint16_t num = 1);

    int DelItem(int pos, uint16_t num = 0);

    Item GetItem(int pos) const;
    uint16_t GetItemNum(int pos) const;
    std::pair<Item, uint16_t> GetItemInfo(int pos) const;

    void ExchangeItems(int pos1, int pos2);

    // 使用并且数量减1
    int UseItem(int pos);

private:
    ItemBar();
    void __ToFirst(int pos);

private:
    static std::mutex __m_instance_mutex;
    static ItemBar* __m_pInstance;

    std::vector<int> __m_order_vec;
    // 最近使用时间排序
    // 新添加的物品当做最新使用
};


#endif //GAMEBACKPACK_ITEMBAR_H
