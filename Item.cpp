#include <utility>

//
// Created by 苏嘉凯 on 2019-04-30.
//

#include "Item.h"

Item::Item(int id, const std::string& name, Property pro, std::function<void()> f) :
        m_id(id),
        m_name(name),
        m_property(pro),
        __m_fConsume(std::move(f))
{}

Item::Item(int id, std::string&& name, Property pro, std::function<void()> f) :
        m_id(id),
        m_name(std::move(name)),
        m_property(pro),
        __m_fConsume(std::move(f))
{}

Item::Item() :
        m_id(0),
        m_property(),
        __m_fConsume(nullptr)
{}

Item::Item(const Item& other) :
        Item(other.m_id, other.m_name, other.m_property, other.__m_fConsume)
{}

Item::Item(Item&& other) noexcept :
        Item(other.m_id, std::move(other.m_name), other.m_property, other.__m_fConsume)
{}

Item &
Item::operator=(Item && other) noexcept
{
    m_id = other.m_id;
    m_property = other.m_property;
    m_name = std::move(other.m_name);
    __m_fConsume = std::move(other.__m_fConsume);
    return *this;
}

#include <iostream>
void
Item::Use()
{
    return __m_fConsume();
}