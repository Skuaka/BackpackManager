//
// Created by 苏嘉凯 on 2019-04-30.
//

#ifndef GAMEBACKPACK_STORAGEROOM_H
#define GAMEBACKPACK_STORAGEROOM_H

// --------- option -----------
//#define USE_INNER_LOCK 1
// 如果单独使用 StorageRoom 类可以开启锁支持
// 但背包系统会继承 StorageRoom 并添加功能，然后加整体的锁，所以这里不能开启

#include "Item.h"

#include <exception>
#include <vector>
#ifdef USE_INNER_LOCK
    #include <shared_mutex>
    // shared_mutex in C++17, instead of shared_timed_mutex in C++14
#endif

#define E_ROOM_FULL -2
#define E_CARRY_LIMIT -3
#define E_INADEQUTAE -4
#define E_NO_ITEM -5
#define E_ITEM_CONFLICT -6
#define E_CONSUMABLE_CONFLICT -7

// 背包和物品栏的基类，定义了基本的增删查接口
class StorageRoom {
public:
    StorageRoom& operator=(const StorageRoom&) = delete;
    StorageRoom& operator=(const StorageRoom&&) = delete;

protected:
    explicit StorageRoom(unsigned capacity);

    // 指定或不指定位置添加物品，默认数量为1。
    // 不指定位置就查找第一个空位或者已经存在的消耗品就叠加。
    // 添加成功返回物品的索引 pos >= 0，
    // 背包已经满了返回 E_ROOM_FULL，
    // 消耗品达到叠加上限返回 E_CARRY_LIMIT
    int _AddItem(const Item &item, uint16_t num = 1);
    int _AddItem(Item &&item, uint16_t num = 1);
    int _AddItem(int pos, const Item &item, uint16_t num = 1);
    int _AddItem(int pos, Item &&item, uint16_t num = 1);

    // 删除指定位置的物品，默认删除该位置的所有数量，即数量为0代表全部删除，
    // 成功删除返回 0，
    // 过量删除返回 E_INADEQUTAE，
    // 该位置不存在物品返回 E_NO_ITEM
    int _DelItem(int pos, uint16_t num = 0);

    // 查看物品信息，比如界面需要显示个数、
    // 鼠标停留在物品上时界面需要物品信息。
    // 如果查询时格子没有物品，返回 id为0的item
    Item _GetItem(int pos)
#ifndef USE_INNER_LOCK
    const
#endif
    ;

    uint16_t _GetItemNum(int pos) const ;

    std::pair<Item, uint16_t> _GetItemInfo(int pos)
#ifndef USE_INNER_LOCK
    const
#endif
    ;

protected:
    // 表示一个格子：物品+个数
    struct Cell{
        Item m_item;
        uint16_t m_num;
    };

#ifdef USE_INNER_LOCK
    std::shared_mutex _m_item_mutex;
#endif

    std::vector<Cell> _m_cell_vec;
    /* 格子从0开始编号，即数组下标pos;
     * 若 Cell.num == 0 表明该位置为空位;
     *
     * 因为游戏内背包容量不会很大，所以增删改
     * 查直接遍历就行，不需要要添加索引来优化。
     */

    const unsigned _m_capacity;
};


#endif //GAMEBACKPACK_STORAGEROOM_H
