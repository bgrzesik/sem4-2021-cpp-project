//
// Created by Bartłomiej Grzesik on 03/06/2021.
//
#include <vector>
#include <memory>

#include "Spotters.hpp"

#ifndef TRIGGERBOX_HPP
#define TRIGGERBOX_HPP

namespace tb
{


template<typename Input>
class Pipeline
{

public:
    explicit Pipeline(bool debug = false);


private:
    bool debug;

    void showDebug();

};

}

#endif //TRIGGERBOX_HPP
