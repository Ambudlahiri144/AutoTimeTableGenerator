#include "mycheckbox.h"
#include "selectordialog.h"

MyCheckBox::MyCheckBox(int x, int y) : QCheckBox()
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged2(int)));
    this->x = x;
    this->y = y;
}

void MyCheckBox::onStateChanged2(int state) {
    parentClass->changeState(this->x, this->y, state);
}

