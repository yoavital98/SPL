#include "Trainer.h"

class Trainer{
public:
    Trainer(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() { }
    int getCapacity() const
    {
        return capacity;
    }
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer()
    {
        open = true;
    }
    void closeTrainer()
    {
        open = false;
    }
    int getSalary();
    bool isOpen()
    {
        return  open;
    }
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif