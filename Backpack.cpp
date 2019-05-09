//
// Created by 苏嘉凯 on 2019-04-29.
//

#include "Backpack.h"

std::mutex Backpack::__m_instance_mutex;
Backpack* Backpack::__m_pInstance;

Backpack&
Backpack::GetInstance() {
    if(!__m_pInstance)
    {
        std::unique_lock<std::mutex> lk(__m_instance_mutex);
        if (!__m_pInstance) {
            __m_pInstance = new Backpack;
        }
    }
    return *__m_pInstance;
}

Backpack::Backpack() :
        StorageRoom(BP_CAPACITY)
{}

int
Backpack::AddItem(const Item &item, uint16_t num) {
    return StorageRoom::_AddItem(item, num);
}

int
Backpack::AddItem(Item && item, uint16_t num) {
    return StorageRoom::_AddItem(std::move(item), num);
}

int
Backpack::AddItem(int pos, const Item & item, uint16_t num) {
    return StorageRoom::_AddItem(pos, item, num);
}

int
Backpack::AddItem(int pos, Item && item, uint16_t num) {
    return StorageRoom::_AddItem(pos, std::move(item), num);
}

int
Backpack::DelItem(int pos, uint16_t num) {
    return StorageRoom::_DelItem(pos, num);
}

Item
Backpack::GetItem(int pos) const {
    return StorageRoom::_GetItem(pos);
}

uint16_t
Backpack::GetItemNum(int pos) const {
    return StorageRoom::_GetItemNum(pos);
}

std::pair<Item, uint16_t>
Backpack::GetItemInfo(int pos) const {
    return std::move(StorageRoom::_GetItemInfo(pos));
}
