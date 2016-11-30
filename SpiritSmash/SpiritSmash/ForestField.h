#ifndef FOREST_FIELD_H
#define FOREST_FIELD_H

#include "Field.h"
#include "DirectionalLight.h"

class ForestField : public Field
{
public:

    ForestField();
    ~ForestField();


    void Update();

    void Generate();

private:

    DirectionalLight* m_pSun;
    Matter m_skysphere;
};


#endif