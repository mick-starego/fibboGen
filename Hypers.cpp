#include <iostream>
#include "Hypers.h"

using namespace std;

Hypers::Hypers(float l1, int b1, float l2, int b2, float l3) {
    this->l1 = l1;
    this->l2 = l2;
    this->l3 = l3;
    this->b1 = b1;
    this->b2 = b2;
}

Hypers::Hypers(float l1) {
    this->l1 = l1;
    this->b1 = 1000;
    this->l2 = 0;
    this->b2 = 0;
    this->l3 = 0;
}

Hypers::Hypers(float l1, int b1, float l2) {
    this->l1 = l1;
    this->b1 = b1;
    this->l2 = l2;
    this->b2 = 1000;
    this->l3 = 0;
}

float Hypers::getLoad1() {
    return l1;
}

int Hypers::getBound1() {
    return b1;
}

float Hypers::getLoad2() {
    return l2;
}

int Hypers::getBound2() {
    return b2;
}

float Hypers::getLoad3() {
    return l3;
}