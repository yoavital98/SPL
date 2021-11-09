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
    std::vector<Workout> workout_list(workout_options);
    std::sort(workout_list.begin(), workout_list.end(), greater<int>());
    std::vector<int>* workout = new vector<int>();
    for(int i=0; i< workout_list.size(); i++){
        if(workout_list[i].getType() == ANAEROBIC) {
            workout->push_back(workout_list[i].getId());
        }
    }
    return *workout;
}
std::string HeavyMuscleCustomer::toString() const { return Customer::getName() + ",mcl";}



//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> workout_list(workout_options);
    std::sort(workout_list.begin(), workout_list.end());
    std::vector<int> cardio;
    std::vector<int> mixType;
    std::vector<int> anaerobic;
    for(int i=0;i<workout_list.size();i++)
    {
        if(workout_list[i].getType()==CARDIO)
            cardio.push_back(workout_list[i].getId());
        if(workout_list[i].getType()==MIXED)
            mixType.insert(mixType.begin(), workout_list[i].getId());
        if(workout_list[i].getType()==ANAEROBIC)
            anaerobic.push_back(workout_list[i].getId());
        if(cardio.size()>0 && mixType.size()>0 && anaerobic.size()>0)
            break;
    }
    std::vector<int> workout;
    if(!cardio.empty())
        workout.push_back(cardio[0]);
    if(!mixType.empty())
        workout.push_back(mixType[0]);
    if(!cardio.empty())
        workout.push_back(anaerobic[0]);
    return workout;
}
std::string FullBodyCustomer::toString() const { return getName(); }
