#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "Coords.hpp"

struct Object {
    Coords<float> fPos;
    std::string sTexture;
    bool bRemove = false;
};

#endif    // _OBJECT_HPP_
