#include "Trainer.h"

    Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() { }
    int Trainer::getCapacity() const
    {
        return capacity;
    }
    void Trainer::addCustomer(Customer* customer) { }
    void Trainer::removeCustomer(int id) { }
    Customer* Trainer::getCustomer(int id) { }
    std::vector<Customer*>& Trainer::getCustomers() { }
    std::vector<OrderPair>& Trainer::getOrders() { }
    void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options) { }
    void Trainer::openTrainer()
    {
        open = true;
    }
    void Trainer::closeTrainer()
    {
        open = false;
    }
    int Trainer::getSalary() { }
    bool Trainer::isOpen()
    {
        return  open;
    }