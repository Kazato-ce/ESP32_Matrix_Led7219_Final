#ifndef SCROLLING_TEXT_H
#define SCROLLING_TEXT_H

#include "Globals.h"

#include "ModeBase.h"

class ModeText : public DoubleHeightMode
{
public:
    void refreshData() override
    {
        strcpy(_buffer, msg);
    }
};

#endif