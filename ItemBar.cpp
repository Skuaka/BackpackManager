//
// Created by 苏嘉凯 on 2019-04-29.
//

#include "ItemBar.h"

std::mutex ItemBar::__m_instance_mutex;
ItemBar* ItemBar::__m_pInstance;

ItemBar &ItemBar::GetInstance() {
    if(!__m_pInstance)
    {
        std::unique_lock<std::mutex> lk(__m_instance_mutex);
        if (!__m_pInstance) {
            __m_pInstance = new ItemBar;
        }
    }
    return *__m_pInstance;
}

ItemBar::ItemBar() :
    StorageRoom(BAR_CAPACITY)
{
    __m_order_vec.resize(_m_capacity);
    for(int i=0; i<_m_capacity; ++i){
        __m_order_vec[i] = i;
    }
}

std::pair<Item, uint16_t>
ItemBar::SqueezeItem(const Item &item, uint16_t num) {
    return SqueezeItem(std::move(Item(item)), num);
}

// todo: 移动未优化
std::pair<Item, uint16_t>
ItemBar::SqueezeItem(Item &&item, uint16_t num) {
    //  为了偷懒和方便调试，直接调用父类函数，但是
    //  因为下面还需要用到item，所以不能移动item。
    int ret = StorageRoom::_AddItem(item, num);

    // 物品栏满了，需要置换
    if( ret == E_ROOM_FULL){
        int i_to_chg = __m_order_vec[_m_capacity-1];
        std::pair<Item, uint16_t> ret_info(std::move(GetItemInfo(i_to_chg)));

        _m_cell_vec[i_to_chg].m_item = std::move(item);
        _m_cell_vec[i_to_chg].m_num = num;

        for(int i=_m_capacity-1; i>=1; --i){
            __m_order_vec[i] = __m_order_vec[i-1];
        }
        __m_order_vec[0] = i_to_chg;

        return std::move(ret_info);
    }

    return std::move(
            std::pair<Item, uint16_t >(
                    Item(ret, std::string{}, Item::Property{}, nullptr),
                    0 ));
}

int ItemBar::AddItem(const Item &item, uint16_t num) {
    return AddItem(std::move(Item(item)), num);
}

int ItemBar::AddItem(Item &&item, uint16_t num) {
    int pos = StorageRoom::_AddItem(std::move(item), num);

    if(pos >= 0) {
        __ToFirst(pos);
    }

    return pos;
}

int
ItemBar::AddItem(int pos, const Item &item, uint16_t num) {
    return AddItem(pos, std::move(Item(item)), num);
}

int
ItemBar::AddItem(int pos, Item &&item, uint16_t num) {
    int ret = StorageRoom::_AddItem(pos, std::move(item), num);

    if(ret >= 0) {
        __ToFirst(pos);
    }

    return ret;
}

int
ItemBar::DelItem(int pos, uint16_t num) {
    return StorageRoom::_DelItem(pos, num);
}

Item
ItemBar::GetItem(int pos) const {
    return StorageRoom::_GetItem(pos);
}

uint16_t
ItemBar::GetItemNum(int pos) const {
    return StorageRoom::_GetItemNum(pos);
}

std::pair<Item, uint16_t>
ItemBar::GetItemInfo(int pos) const {
    return std::move(StorageRoom::_GetItemInfo(pos));
}

int
ItemBar::UseItem(int pos) {
    int ret = StorageRoom::_DelItem(pos, 1);

    if(ret == 0) {
        _m_cell_vec[pos].m_item.Use();

        if(_m_cell_vec[pos].m_num > 0) {
            __ToFirst(pos);
        }
    }

    return ret;
}

void
ItemBar::__ToFirst(int pos) {
    for (int i = _m_capacity - 1; i >= 0; --i) {
        if (__m_order_vec[i] == pos) {
            for (int j = i; j >= 1; --j) {
                __m_order_vec[j] = __m_order_vec[j - 1];
            }
            __m_order_vec[0] = pos;
            break;
        }
    }
}

void
ItemBar::ExchangeItems(int pos1, int pos2)
{
    if(pos1 < 0 || pos1 > BAR_CAPACITY || pos2 < 0 || pos2 > BAR_CAPACITY)
        throw std::domain_error("illegal position");

    if(pos1 == pos2)
        throw std::invalid_argument("The two positions must be different");

    auto t = GetItemInfo(pos1);

    _m_cell_vec[pos1].m_num = _m_cell_vec[pos2].m_num;
    _m_cell_vec[pos1].m_item = std::move(_m_cell_vec[pos2].m_item);

    _m_cell_vec[pos2].m_num = t.second;
    _m_cell_vec[pos2].m_item = std::move(t.first);
}
