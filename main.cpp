#include <iostream>
#include "BackpackEvent.h"
#include "ItemExample.h"

using namespace std;
int main() {
    std::cout << "Hello, World!\n" << std::endl;
    BackpackEvent& be = BackpackEvent::GetInstance();

    cout << "PickItem(legendary_sword)\n";
    be.PickItem(legendary_sword);
    cout << "PickItem(healing_medicine, 2)\n";
    be.PickItem(healing_medicine, 2);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "PickItem(apple, 8)\n";
    be.PickItem(apple, 8);
    cout << "PickItem(medicine1, 3)\n";
    be.PickItem(medicine1, 3);
    cout << "PickItem(medicine2, 3)\n";
    be.PickItem(medicine2, 3);
    cout << "PickItem(medicine3, 3)\n";
    be.PickItem(medicine3, 3);
    cout << "PickItem(medicine4, 3)\n";
    be.PickItem(medicine4, 3);
    cout << "PickItem(medicine5, 3)\n";
    be.PickItem(medicine5, 3);
    cout << "PickItem(medicine6, 3)\n";
    be.PickItem(medicine6, 3);
    cout << "PickItem(medicine6, 3)\n";
    be.PickItem(medicine6, 3);

    for(int i=0; i<3; ++i){
        cout << "PickItem(scabbard)\n";
        be.PickItem(scabbard);
    }
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "UseItem(0)\n";
    be.UseItem(0);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "PickItem(apple, 30)\n";
    be.PickItem(apple, 30);
    cout << "UseItem(1)\n";
    be.UseItem(1);
    cout << "UseItem(1)\n";
    be.UseItem(1);
    cout << "UseItem(1)\n";
    be.UseItem(1);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "FromBpToBar(0)\n";
    be.FromBpToBar(0);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "PickItem(healing_medicine, 2)\n";
    be.PickItem(healing_medicine, 2);
    for(int i=0; i<3; ++i){
        cout << "PickItem(scabbard)\n";
        be.PickItem(scabbard);
    }
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "UseItem(7)\n";
    be.UseItem(7);
    cout << "UseItem(6)\n";
    be.UseItem(6);
    cout << "UseItem(5)\n";
    be.UseItem(5);
    cout << "UseItem(4)\n";
    be.UseItem(4);
    cout << "UseItem(2)\n";
    be.UseItem(2);
    cout << "UseItem(1)\n";
    be.UseItem(1);
    cout << "UseItem(0)\n";
    be.UseItem(0);
    cout << "FromBpToBar(2)\n";
    be.FromBpToBar(2);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "PickItem(legendary_sword)\n";
    be.PickItem(legendary_sword);
    cout << "PickItem(whetstone)\n";
    be.PickItem(whetstone);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "SetCombine3(whetstone, legendary_sword, scabbard, combined_sword)\n";
    be.SetCombine3(whetstone, legendary_sword, scabbard, combined_sword);
    cout << "FromBpToBar(6)\n";
    be.FromBpToBar(6);
    cout << "FromBpToBar(7)\n";
    be.FromBpToBar(7);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "UseItem(3)\n";
    be.UseItem(3);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "PickItem(legendary_sword)\n";
    be.PickItem(legendary_sword);
    cout << "PickItem(whetstone)\n";
    be.PickItem(whetstone);
    cout << "PickItem(scabbard)\n";
    be.PickItem(scabbard);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    cout << "ExchangeItemBar(0, 1)\n";
    be.ExchangeItemBar(0, 1);
    cout << "DropItemBar(2, 5)\n";
    be.DropItemBar(2, 5);
    cout << "DropBackpack(3)\n";
    be.DropBackpack(3);
    cout << "ShowItemBar()\n";
    be.ShowItemBar();
    cout << "ShowBackpack()\n";
    be.ShowBackpack();
    cout << endl;

    return 0;
}