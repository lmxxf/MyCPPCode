#pragma once
#include <string>
using namespace std;

class Gun
{
private:
    /* data */
    int _bullet_count;
    string _type;
public:
    Gun(string type);
    ~Gun();

    void addBullet(int bullet_number);
    bool shoot();
};

