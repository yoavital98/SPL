#include "Trainer.h"

//Constructor
Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false) {}
//Destructor
Trainer::~Trainer() {
    clear();
}
//Copy Constructor
Trainer::Trainer(const Trainer &other) : capacity(other.capacity), open(other.open){
    copy(other);
}
//Move Constructor
Trainer::Trainer(Trainer &&other) : capacity(other.capacity), open(other.open){
    move(other);
}
//Copy Assignment
Trainer& Trainer::operator=(const Trainer &other){
    if(this == &other)
        return *this;
    clear();
    capacity = other.capacity;
    open = other.open;
    copy(other);
    return *this;
}
//Move Assignment
Trainer& Trainer::operator=(Trainer &&other){
    if (this != &other)
    {
        clear();
        capacity = other.capacity;
        open = other.open;
        move(other);
    }
    return *this;
}

int Trainer::getCapacity() const
{return capacity;}

void Trainer::addCustomer(Customer* customer)
{
    customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {
    customersList.erase(customersList.begin() + id);
    for (int i = (int)orderList.size()-1; i >= 0; i--) {
        if (orderList[i].first == id)
            orderList.erase(orderList.begin() + i);
    }
}


void Trainer::removeCustomers() {
    for(long unsigned int i=0 ;i<customersList.size();i++)
        if(customersList[i])
            delete customersList[i];
    customersList.clear();
}

Customer* Trainer::getCustomer(int id)
{
    if(id >= (int)customersList.size())
        return nullptr;
    return customersList[id];
}

std::vector<Customer*>& Trainer::getCustomers()
{return customersList;}

std::vector<OrderPair>& Trainer::getOrders()
{return orderList;}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    for(long unsigned int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == customer_id) {
            for(long unsigned int j=0;j<workout_ids.size();j++)
            {
                this->addOrder(OrderPair (customersList[i]->getId(), workout_options[workout_ids[j]]));
                std::cout << customersList[i]->toString() << " Is Doing " << workout_options[workout_ids[j]].getName() << std::endl;
            }
            break;
        }
    }
}

void Trainer::addOrder(OrderPair order){
    orderList.push_back(order);
}


void Trainer::openTrainer()
{open = true;}

void Trainer::closeTrainer()
{open = false;}

int Trainer::getSalary()
{
    int sum = 0;
    for(long unsigned int i =0; i<orderList.size(); i++){
        sum = sum + orderList[i].second.getPrice();
    }
    return sum;
}

bool Trainer::isOpen()
{return open;}

void Trainer::clear(){
    for(long unsigned int i=0; i < customersList.size(); i++){
        if(customersList[i]){
            delete (customersList[i]);
        }
    }
    capacity = 0;
    open = false;
    orderList.clear();
    customersList.clear();
}

void Trainer::copy(const Trainer &other){
    for(Customer *c : other.customersList){
        customersList.push_back(c->getCustomer());
    }
    for(OrderPair order : other.orderList){
        orderList.push_back(OrderPair(order));
    }
}

void Trainer::move(Trainer &other){
    for(long unsigned int i = 0; i < other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]);
        other.customersList[i] = nullptr;
    }
    for(OrderPair order : other.orderList){
        orderList.push_back(order);
    }
}
/*
int capacity;
bool open;
std::vector<Customer*> customersList;
std::vector<OrderPair> orderList;
 */