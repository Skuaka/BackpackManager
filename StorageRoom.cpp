//
// Created by 苏嘉凯 on 2019-04-30.
//

#include "StorageRoom.h"

StorageRoom::StorageRoom(unsigned capacity) :
        _m_capacity(capacity)
{
    _m_cell_vec.resize(_m_capacity);
    for(int i=0; i<_m_capacity; ++i){
        _m_cell_vec[i].m_num = 0;
    }
}

int
StorageRoom::_AddItem(int pos, const Item &item, uint16_t num)
{
    return _AddItem(pos, std::move(Item(item)), num);
}

int
StorageRoom::_AddItem(int pos, Item &&item, uint16_t num)
{
    if(pos < 0 || pos > _m_capacity)
        throw std::out_of_range("illegal position");

    if(num <= 0)
        throw std::domain_error("number must > 0");

    if(item.m_property.m_max <= 0 && num != 1)
        throw std::invalid_argument("number of non-consumable must be 1");

    if(item.m_property.m_max > 0 && num > item.m_property.m_max)
        throw std::invalid_argument("number of consumable is more than the carrying limit");

    {
#ifdef USE_INNER_LOCK
        std::unique_lock<std::shared_mutex> lk(_m_item_mutex);
#endif
        if (_m_cell_vec[pos].m_num != 0 && (                    // pos位置已经有物品，并且
                _m_cell_vec[pos].m_item.m_property.m_max < 0 || // 物品是非消耗品，或
                _m_cell_vec[pos].m_item.m_id != item.m_id       // 物品是消耗品但id不同
        ))
            return E_ITEM_CONFLICT;

        // 消耗品只能叠加存放在一个格子，遍历检查
        if (item.m_property.m_max >= 0) {
            int i;
            for (i = 0; i < _m_capacity; ++i) {
                if (_m_cell_vec[i].m_num != 0 &&
                    _m_cell_vec[i].m_item.m_id == item.m_id) {
                    break;
                }
            }
            if (_m_capacity != i)
                return E_CONSUMABLE_CONFLICT;
        }

        if (_m_cell_vec[pos].m_num == 0) {
            _m_cell_vec[pos].m_item = std::move(item);
            _m_cell_vec[pos].m_num = num;
        } else {
            if (_m_cell_vec[pos].m_num + num > item.m_property.m_max)
                return E_CARRY_LIMIT;
            _m_cell_vec[pos].m_num += num;
        }
    }
    return pos;
}

int
StorageRoom::_AddItem(const Item &item, uint16_t num)
{
    return _AddItem(std::move(Item(item)), num);
}

int
StorageRoom::_AddItem(Item &&item, uint16_t num)
{
    if(num <= 0)
        throw std::domain_error("number must > 0");

    if(item.m_property.m_max <= 0 && num != 1)
        throw std::invalid_argument("number of non-consumable must be 1");

    if(item.m_property.m_max > 0 && num > item.m_property.m_max)
        throw std::invalid_argument("number of consumable is more than the carrying limit");

    int cur_pos;
    {
#ifdef USE_INNER_LOCK
        std::unique_lock<std::shared_mutex> lk(_m_item_mutex);
#endif
        if (item.m_property.m_max < 0) {
            // 如果item不是消耗品，查找第一个空位
            cur_pos = 0;
            for (; cur_pos < _m_capacity; ++cur_pos) {
                if (_m_cell_vec[cur_pos].m_num == 0) {
                    break;
                }
            }
            if (cur_pos == _m_capacity)
                return E_ROOM_FULL;

            _m_cell_vec[cur_pos].m_item = std::move(item);
            _m_cell_vec[cur_pos].m_num = 1;
        } else {
            // 如果item是消耗品，进行一次遍历，有以下情况（另外需要检查物品叠加上限）：
            // 1. cur_pos != _m_capacity ：已经存在该物品
            // 2. cur_pos == _m_capacity ：不存在该物品
            //    2.1. first_vacancy != -1 ：存在空位
            //    2.2. first_vacancy == -1 ：已经满了
            int first_vacancy = -1;
            cur_pos = 0;
            for (; cur_pos < _m_capacity; ++cur_pos) {
                if (_m_cell_vec[cur_pos].m_num == 0) {
                    if (first_vacancy == -1) {
                        first_vacancy = cur_pos;
                    }
                } else {
                    if (_m_cell_vec[cur_pos].m_item.m_id == item.m_id) {
                        break;
                    }
                }
            }

            if (cur_pos != _m_capacity) {
                if (_m_cell_vec[cur_pos].m_num + num > item.m_property.m_max)
                    return E_CARRY_LIMIT;

                _m_cell_vec[cur_pos].m_num += num;
            } else {
                if (first_vacancy == -1)
                    return E_ROOM_FULL;

                _m_cell_vec[first_vacancy].m_item = std::move(item);
                _m_cell_vec[first_vacancy].m_num = num;
                cur_pos = first_vacancy;
            }
        }
    }
    return cur_pos;
}


int
StorageRoom::_DelItem(int pos, uint16_t num) {
    if(num < 0)
        throw std::domain_error("illegal number");
    if(pos < 0 || pos > _m_capacity)
        throw std::out_of_range("illegal position");

    {
#ifdef USE_INNER_LOCK
        std::unique_lock<std::shared_mutex> lk(_m_item_mutex);
#endif
        if (_m_cell_vec[pos].m_num == 0)
            return E_NO_ITEM;

        if (_m_cell_vec[pos].m_num < num)
            return E_INADEQUTAE;

        if (num == 0)
            _m_cell_vec[pos].m_num = 0;
        else
            _m_cell_vec[pos].m_num -= num;
    }
    return 0;
}

Item
StorageRoom::_GetItem(int pos)
#ifndef USE_INNER_LOCK
const
#endif
{
    if(pos < 0 || pos > _m_capacity)
        throw std::out_of_range("illegal position");

    {
#ifdef USE_INNER_LOCK
        std::shared_lock<std::shared_mutex> lk(_m_item_mutex);
#endif
        if (_m_cell_vec[pos].m_num == 0)
            return std::move(Item{});

        return std::move(Item(_m_cell_vec[pos].m_item));
    }
}

uint16_t
StorageRoom::_GetItemNum(int pos) const {
    if(pos < 0 || pos > _m_capacity)
        throw std::out_of_range("illegal position");

    return _m_cell_vec[pos].m_num;
}

std::pair<Item, uint16_t> StorageRoom::_GetItemInfo(int pos)
#ifndef USE_INNER_LOCK
const
#endif
{
    return std::move(
            std::pair<Item, uint16_t>(_GetItem(pos), _GetItemNum(pos))
                    );
}
