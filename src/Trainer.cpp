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
    copy(other);
    for(int i = 0; i < other.customersList.size(); i++){
        other.customersList[i] = nullptr;
    }
}
//Copy Assignment
Trainer& Trainer::operator=(const Trainer &other){
    if(this == &other)
        return *this;
    clear();
    capacity = other.capacity;
    open = other.open;
    copy(other);
}
//Move Assignment
Trainer& Trainer::operator=(Trainer &&other){
    if (this != &other)
    {

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
    for (int i = 0; i < orderList.size(); i++) {
        if (orderList[i].first == id)
            orderList.erase(orderList.begin() + 1);
    }
}


void Trainer::removeCustomers() {
    customersList.clear();
}

Customer* Trainer::getCustomer(int id)
{
    if(id >= customersList.size())
        return nullptr;
    return customersList[id];
}

std::vector<Customer*>& Trainer::getCustomers()
{return customersList;}

std::vector<OrderPair>& Trainer::getOrders()
{return orderList;}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    for(int i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == customer_id)
            customersList[i]->order(workout_options);
        break;
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
    for(int i =0; i<orderList.size(); i++){
        sum = sum + orderList[i].second.getPrice();
    }
    return sum;
}

bool Trainer::isOpen()
{return open;}

void Trainer::clear(){
    for(int i=0; i < customersList.size(); i++){
        if(customersList[i]){
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    capacity = 0;
    open = false;
    orderList.clear();
}

void Trainer::copy(const Trainer &other){
    for(int i = 0; i < other.customersList.size(); i++){
        std::string type = customersList[i]->getType();
        Customer* otherCustomer;
        if(type == "swt")
            otherCustomer = new SweatyCustomer(customersList[i]->getName(), customersList[i]->getId());
        else if(type == "chp")
            otherCustomer = new CheapCustomer(customersList[i]->getName(), customersList[i]->getId());
        else if(type == "mcl")
            otherCustomer = new HeavyMuscleCustomer(customersList[i]->getName(), customersList[i]->getId());
        else if(type == "fbd")
            otherCustomer = new FullBodyCustomer(customersList[i]->getName(), customersList[i]->getId());
        customersList.push_back(otherCustomer);
    }
    for(int i = 0; i < other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
}

/*
int capacity;
bool open;
std::vector<Customer*> customersList;
std::vector<OrderPair> orderList;
 */