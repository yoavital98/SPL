#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include <iostream>
#include "Customer.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    //Constructor
    Trainer(int t_capacity);
    //Destructor
    virtual ~Trainer();
    //Copy Constructor
    Trainer(const Trainer &other);
    //Move Constructor
    Trainer(Trainer &&other);
    //Copy Assignment
    Trainer& operator=(const Trainer &other);
    //Move Assignment
    Trainer& operator=(Trainer &&other);
    //========================================
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    void removeCustomers();
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void addOrder(OrderPair order);
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    void clear();
    void copy(const Trainer &other);
};


#endif