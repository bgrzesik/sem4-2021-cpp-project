//
// Created by Bartłomiej Grzesik on 09/06/2021.
//

#include <iostream>
#include "PipelineNode.hpp"

#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

namespace tb
{

template<typename Type>
class Notifier : public PipelineInputNode<Type>
{
public:
    virtual ~Notifier() = default;

    virtual void notify(Type input) = 0;

protected:
    void process(Type input, PipelineContext *ctx) final
    {
        notify(input);
    }

};

template<typename Type>
class StreamNotifier : public Notifier<Type>
{
public:
    StreamNotifier(std::ostream &ostream, std::string message)
        : ostream(ostream), message(std::move(message))
    {
    }

    void notify(Type input) override
    {
        this->ostream << message;
    }

private:
    std::ostream &ostream;
    std::string message;

};

template<typename Type>
class CallbackNotifier : public Notifier<Type>
{
public:
    typedef std::function<void (Type)> Callback;

    explicit CallbackNotifier(Callback callback)
        : callback(callback)
    {
    }

    void notify(Type input) override
    {
        callback(input);
    }

private:
    Callback callback;

};

}



#endif //NOTIFIER_HPP
