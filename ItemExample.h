//
// Created by 苏嘉凯 on 2019-05-03.
//

#ifndef GAMEBACKPACK_ITEMEXAMPLE_H
#define GAMEBACKPACK_ITEMEXAMPLE_H

#include "Item.h"

Item::Property weapon_pro(Item::Kind::weapon);
const Item legendary_sword(1, "legendary sword", weapon_pro, []{
    std::cout << "\tEquip 'legendary sword'\n\tATK + 10\n";
});

const Item scabbard(142, "scabbard", weapon_pro, []{
    std::cout << "\tEquip 'scabbard'\n\tATK + 5\n";
});

const Item whetstone(144, "whetstone", weapon_pro, []{
    std::cout << "\tEquip 'whetstone'\n\tATK + 2\n";
});

const Item combined_sword(148, "combined_sword", weapon_pro, []{
    std::cout << "\tEquip 'combined_sword'\n\tATK + 2000\n";
});

Item::Property hp_pro(Item::Kind::medicine, 99);
const Item healing_medicine(2, "healing_medicine", hp_pro, []{
    std::cout << "\tDrink 'healing_medicine'\n\tHP + 30\n";
});

const Item apple(3, "apple", hp_pro, []{
    std::cout << "\tEat 'apple'\n\tHP + 5\n";
});

const Item medicine1(4, "medicine1", hp_pro, []{
    std::cout << "\tEat 'medicine1'\n\tHP + 1\n";
});

const Item medicine2(5, "medicine2", hp_pro, []{
    std::cout << "\tEat 'medicine2'\n\tHP + 2\n";
});

const Item medicine3(6, "medicine3", hp_pro, []{
    std::cout << "\tEat 'medicine3'\n\tHP + 3\n";
});

const Item medicine4(7, "medicine4", hp_pro, []{
    std::cout << "\tEat 'medicine4'\n\tHP + 4\n";
});

const Item medicine5(8, "medicine5", hp_pro, []{
    std::cout << "\tEat 'medicine5'\n\tHP + 5\n";
});

const Item medicine6(9, "medicine6", hp_pro, []{
    std::cout << "\tEat 'medicine6'\n\tHP + 5\n";
});

#endif //GAMEBACKPACK_ITEMEXAMPLE_H
