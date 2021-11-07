#include "Customer.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
    Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) { }
    std::string Customer::getName() const { return name;}
    int Customer::getId() const { return id; }

    //SweatyCustomer
    SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id)  { }
    std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options)
    {
        std::vector<int>* workout_list = new std::vector<int>();
        for(int i = 0; i < workout_options.size(); i++) {
            if (workout_options[i].getType() == CARDIO)
            {
                workout_list->push_back(workout_options[i].getId());
            }
        }
        return *workout_list;
    }
    std::string SweatyCustomer::toString() const { return Customer::getName() + ",swt"; }

    //CheapCustomer
    CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) { }
    std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
    {
        std::vector<int>* workout_list = new std::vector<int>();
        std::vector<int>* workout_prices = new std::vector<int>();
        for(int i = 0; i < workout_options.size(); i++) {
            workout_prices->push_back(workout_options[i].getPrice());
        }
        std::sort(workout_prices[0], workout_prices[workout_prices->size()-1], greater<int>());
        int* min = workout_prices[workout_prices->size()-1].data();
        bool tmp = true;
        for(int i=0; i< workout_options.size() & tmp; i++){
            if(workout_options[i].getPrice() == *min) {
                workout_list->push_back(workout_options[i].getId());
                tmp = false;
            }
        }
        return *workout_list;
    }
    std::string CheapCustomer::toString() const { return Customer::getName() + ",chp";}

    //HeavyMuscleCustomer
    HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) { }
    std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
//        std::sort(workout_options.begin(), workout_options.end());
        std::vector<int>* workout_list = new std::vector<int>();
    for(int i=0; i< workout_options.size(); i++){
        if(workout_options[i].getType() == ANAEROBIC) {
            workout_list->push_back(workout_options[i].getId());
        }
    }
        return *workout_list;
}
    std::string HeavyMuscleCustomer::toString() const { return Customer::getName() + ",mcl";}



//FullBodyCustomer
    FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) { }
    std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {}
    std::string FullBodyCustomer::toString() const { }
