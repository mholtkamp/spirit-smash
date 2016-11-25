#ifndef FOREST_FIELD_H
#define FOREST_FIELD_H

#include "Field.h"

class ForestField : public Field
{
    ForestField();
    ~ForestField();


    void Update();

    void Generate();

};


#endif