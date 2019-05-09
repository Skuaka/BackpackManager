//
// Created by 苏嘉凯 on 2019-05-03.
//

#include "BackpackEvent.h"

std::mutex BackpackEvent::__m_instance_mutex;
BackpackEvent* BackpackEvent::__m_pInstance;

BackpackEvent&
BackpackEvent::GetInstance()
{
    if(!__m_pInstance)
    {
        std::unique_lock<std::mutex> lk(__m_instance_mutex);
        if (!__m_pInstance) {
            __m_pInstance = new BackpackEvent;
        }
    }
    return *__m_pInstance;
}

BackpackEvent::BackpackEvent() :
    __m_bp(Backpack::GetInstance()),
    __m_bar(ItemBar::GetInstance())
{}

int
BackpackEvent::FromBpToBar(int pos)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);

    auto info1 = __m_bp.GetItemInfo(pos);
    if(info1.second <= 0)
        return E_NO_ITEM;

    int ret = __m_bp.DelItem(pos);
    if(ret != 0)
        return ret;

    auto info2 = __m_bar.SqueezeItem(info1.first, info1.second);
    int id = info2.first.m_id;
    if(info2.second == 0) {
        if (info2.first.m_id < 0){
            // 发生错误，回滚
            ret = __m_bp.AddItem(pos, std::move(info1.first), info1.second);
            assert(ret >= 0); //因为刚刚删除，所以这次添加必定成功。
            return info2.first.m_id;
        }
    }else{
        ret = __m_bp.AddItem(std::move(info2.first), info2.second);
        assert(ret >= 0); //因为刚刚删除，所以这次添加必定成功，除非之前有其他不合逻辑的Api产生了冲突隐患
        __TestCombine(info1.first.m_id);
        return ret;
    }

    assert(info2.second == 0 && info2.first.m_id == 0);
    __TestCombine(info1.first.m_id);

    return 0;
}

int
BackpackEvent::PickItem(const Item &item, uint16_t num)
{
    return PickItem(std::move(Item(item)), num);
}

int
BackpackEvent::PickItem(Item &&item, uint16_t num)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);

    int ret = __m_bar.AddItem(item, num);

    if (ret == E_ROOM_FULL)
    {
        return __m_bp.AddItem(std::move(item), num);
    }

    __TestCombine(item.m_id);
    return ret;
}

void
BackpackEvent::ShowBackpack()
{
    std::shared_lock<std::shared_mutex> lk(__m_main_sMutex);

//    std::cout << "In Backpack :" << std::endl;
    for(int i=0; i<BP_CAPACITY; ++i){
        auto info = __m_bp.GetItemInfo(i);
        if(info.second > 0){
            std::cout << '\t' << i << ": " << info.first.m_name << " x " << info.second << std::endl;
        }
    }
}

void
BackpackEvent::ShowItemBar()
{
    std::shared_lock<std::shared_mutex> lk(__m_main_sMutex);

//    std::cout << "In Item Bar :" << std::endl;
    for(int i=0; i<BAR_CAPACITY; ++i){
        auto info = __m_bar.GetItemInfo(i);
        if(info.second > 0){
            std::cout << '\t' << i << ": " << info.first.m_name << " x " << info.second << std::endl;
        }
    }
}

#include <set>
void
BackpackEvent::SetCombine3(const Item& item1, const Item& item2, const Item& item3, const Item& item4)
{
    int id1 = item1.m_id;
    int id2 = item2.m_id;
    int id3 = item3.m_id;
    int id4 = item4.m_id;

    if(id1 <= 0 || id2 <= 0 || id3 <= 0 || id4 <= 0)
        throw std::domain_error("illegal item id");

    std::set<int> id_set;
    id_set.insert(id1);
    id_set.insert(id2);
    id_set.insert(id3);

    if(id_set.size() != 3 || id_set.find(id4) != id_set.end())
        throw std::invalid_argument("id of the 4 items to be combined must be different");

    std::unique_lock<std::shared_mutex> lk(__m_comb_sMutex);

    std::vector<int> ids;
    for(int id : id_set){
        ids.push_back(id);
    }

    __m_comb_map[ids[0]][ids[1]][ids[2]] = item4;
    __m_comb_map[ids[1]][ids[0]][ids[2]] = item4;
    __m_comb_map[ids[2]][ids[0]][ids[1]] = item4;
}

void
BackpackEvent::__TestCombine(int id1)
{
    auto _find = [this](int id){
        for(int p=0; p<BAR_CAPACITY; ++p){
            if(__m_bar.GetItemNum(p) > 0 &&
                __m_bar.GetItem(p).m_id == id){
                return p;
            }
        }
        return -1;
    };

    int pos1 = _find(id1);

    std::shared_lock<std::shared_mutex> lk(__m_comb_sMutex);

    bool combined = true;

    while(combined) {
        combined = false;

        for (auto const &mp2 : __m_comb_map[id1]) {
            int pos2 = _find(mp2.first);
            if (pos2 < 0)
                continue;

            int id2 = __m_bar.GetItem(pos2).m_id;
            for (auto const &mp3 : __m_comb_map[id1][id2]) {
                int pos3 = _find(mp3.first);
                if (pos3 < 0)
                    continue;

                int id3 = __m_bar.GetItem(pos3).m_id;

                __m_bar.DelItem(pos1);
                __m_bar.DelItem(pos2);
                __m_bar.DelItem(pos3);

                pos1 = __m_bar.AddItem(__m_comb_map[id1][id2][id3]);
                id1 = __m_bar.GetItem(pos1).m_id;

                combined = true;
                break;
            }

            if (combined)
                break;
        }
    }
}


int
BackpackEvent::UseItem(int pos)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);
    return __m_bar.UseItem(pos);
}

void
BackpackEvent::ExchangeItemBar(int pos1, int pos2)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);
    return __m_bar.ExchangeItems(pos1, pos2);
}

int
BackpackEvent::DropBackpack(int pos, uint16_t num)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);
    return __m_bp.DelItem(pos, num);
}

int
BackpackEvent::DropItemBar(int pos, uint16_t num)
{
    std::unique_lock<std::shared_mutex> lk(__m_main_sMutex);
    return __m_bar.DelItem(pos, num);
}
