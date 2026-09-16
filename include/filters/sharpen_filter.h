#pragma once


#include "filter.h"


class Sharpen : public Filter {
    public: 

    void Apply(Image& image) override;
};