#include "Gun.h"
#include <iostream>
using namespace std;

Gun::Gun(string type)
{
    this->_bullet_count = 0;
    this->_type = type;
}

Gun::~Gun()
{
}


void Gun::addBullet(int bullet_number) {
    this->_bullet_count += bullet_number;
}


bool Gun::shoot() {
    if (this->_bullet_count > 0) {
        this->_bullet_count--;
        cout << "Bang!" << endl;
        return true;
    } else {
        cout << "Click!" << endl;
        return true;
    }
}