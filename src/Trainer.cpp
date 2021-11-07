#include "Trainer.h"

    Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false) {}

    int Trainer::getCapacity() const
    {return capacity;}

    void Trainer::addCustomer(Customer* customer)
    {
        customersList.push_back(customer);
    }

    void Trainer::removeCustomer(int id)
    {
        for(int i = 0; i < customersList.size(); i++) {
            if (customersList[i]->getId() == id)
                customersList.erase(customersList.begin() + i);
            break;
        }
        for(int i = 0; i < orderList.size(); i++){
            if(orderList[i].first == id)
                orderList.erase(orderList.begin() + 1);
        }
    }

    Customer* Trainer::getCustomer(int id)
    {
        for (int i = 0; i < customersList.size(); i++)
            if (customersList[i]->getId() == id)
                return customersList[i];
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

    /*
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList;
     */