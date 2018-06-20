#include "validator.h"

Validator::Validator(QObject *parent, MainCtrl *mainCtr)
    : QObject(parent),
      m_mainCtrl(mainCtr)
{

}

bool Validator::hasCicle(){
    return true;
}

void Validator::DFS(){

}

