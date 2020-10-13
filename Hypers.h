#ifndef HYPERS_H
#define HYPERS_H

#include <iostream>

using namespace std;

class Hypers {

    public:
        Hypers(float l1, int b1, float l2, int b2, float l3);
        Hypers(float l1);
        Hypers(float l1, int b1, float l2);
        float getLoad1();
        int getBound1();
        float getLoad2();
        int getBound2();
        float getLoad3();

    private:
        float l1;
        int b1;
        float l2;
        int b2;
        float l3;

};

#endif