
#include "Gun.h"
#include "Soldier.h"
#include <iostream>

void test()
{
    Soldier *soldier = new Soldier("John");
    Gun *gun = new Gun("AK47");
    soldier->addGun(gun);
    soldier->addBulletToGun();
    soldier->fire();
    delete soldier;
    soldier = nullptr;
}

int main()
{
    cout << "This is a test string..." << endl;
    test();
    return 0;
}

// 直接编译：g++ main.cpp src/Gun.cpp src/Soldier.cpp -Iinclude -o myexe -Wall -g -O2