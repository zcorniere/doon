#ifndef _CONCEPTS_HPP_
#define _CONCEPTS_HPP_

#include "abstract/AObject.hpp"

template <class T>
concept isAObject = std::is_base_of<AObject, T>::value;

#endif    //_CONCEPTS_HPP_
