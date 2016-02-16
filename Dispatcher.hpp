#pragma once
#include <string>
#include <vector>

// сделан на основе http://www.pvsm.ru/c-3/14183

class SubscriptionBase
{
public:
    virtual std::string getEventName(void) const {return "";}
    virtual void invoke (void) const{};
    virtual ~SubscriptionBase(){};
};

template <typename ListenerType>
class Subscription : public SubscriptionBase{
private:
    std::string event;
    ListenerType * listener;
    void (ListenerType::*action)(void) const;
    
    virtual std::string getEventName(void) const override{
        return this->event;
    }
    virtual void invoke ( void ) const override{
        (this->listener->*this->action)();
    }
public:
    Subscription(
                 const std::string & event,
                 ListenerType * listener,
                 void (ListenerType::*action)(void) const
                 ){
        this->event = event;
        this->listener = listener;
        this->action = action;
    }
    ~Subscription(){}
};

class EventDispatcher{
protected:
    std::vector<SubscriptionBase*> subscriptions;
public:
    
    template <typename ListenerType>
    void addEventListener(
                          const std::string & event,
                          ListenerType* listener,
                          void (ListenerType::*action) () const
                          ){
        Subscription<ListenerType>* subscription =
            new Subscription<ListenerType>(event, listener, action);
        this->subscriptions.push_back(subscription);
    }

    
    void dispatchEvent(const std::string & event) const{
        for ( size_t key = 0 ; key < this->subscriptions.size() ; key++ )
        {
            if ( this->subscriptions[key]->getEventName() == event )
            {
                this->subscriptions[key]->invoke();
            }
        }
    }
    ~EventDispatcher(){
        for (auto & iter : subscriptions){
            delete iter;
        }
    }
};