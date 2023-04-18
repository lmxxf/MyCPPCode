#pragma once 
#include <string>
#include "Gun.h"

using namespace std;

class Soldier
{
private:
    /* data */
    string name;
    Gun *_ptr_gun;
public:
    Soldier(string name);
    ~Soldier();

    void addGun(Gun *gun);
    void addBulletToGun();
    bool fire();
};