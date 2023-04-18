
#include "Soldier.h"
#include <iostream>
using namespace std;

Soldier::Soldier(string name)
{
    this->name = name;
    this->_ptr_gun = nullptr;
}

void Soldier::addGun(Gun *gun)
{
    this->_ptr_gun = gun;
}

void Soldier::addBulletToGun()
{
    if (this->_ptr_gun != nullptr)
    {
        this->_ptr_gun->addBullet(1);
    }
}

bool Soldier::fire()
{
    if (this->_ptr_gun != nullptr)
    {
        return this->_ptr_gun->shoot();
    }
    return false;
}

Soldier::~Soldier()
{
    if (this->_ptr_gun != nullptr)
    {
        delete this->_ptr_gun;
        this->_ptr_gun = nullptr;
    }
}
