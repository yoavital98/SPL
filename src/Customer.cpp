#include "Customer.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
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
std::string SweatyCustomer::toString() const { return Customer::getName(); }
std::string SweatyCustomer::getType() const { return "swt";}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<pair<int, int>*> workouts;
    for(int i=0;i<workout_options.size();i++) {
        int price = workout_options[i].getPrice();
        int id = workout_options[i].getId();
        workouts.push_back(new pair<int, int>(price, id));
    }
    std::vector<int>* workout_list = new std::vector<int>();
    std::sort(workouts.begin(), workouts.end(), [](const std::pair<int,int> *left, const std::pair<int,int> *right) {return left->first < right->first;});
    workout_list->push_back(workouts[0]->second);
    return *workout_list;
}
std::string CheapCustomer::toString() const { return Customer::getName();}
std::string CheapCustomer::getType() const { return "chp";}

//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<tuple<int, int, WorkoutType>*> workouts;
    for(int i=0;i<workout_options.size();i++) {
        int price = workout_options[i].getPrice();
        int id = workout_options[i].getId();
        workouts.push_back(new tuple<int, int, WorkoutType>(price, id, workout_options[i].getType()));
    }
    std::sort(workouts.begin(), workouts.end(), [](const tuple<int, int, WorkoutType> *left, const tuple<int, int, WorkoutType> *right) {return std::get<0>(*left) < std::get<0>(*right);});
    std::vector<int>* workout = new vector<int>();
    for(int i=0; i< workouts.size(); i++){
        if(std::get<2>(*workouts[i]) == ANAEROBIC) {
            workout->insert(workout->begin(),std::get<1>(*workouts[i]));
        }
    }
    return *workout;
}
std::string HeavyMuscleCustomer::toString() const { return Customer::getName();}
std::string HeavyMuscleCustomer::getType() const { return "mcl";}


//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<tuple<int, int, WorkoutType>*> workouts;
    for(int i=0;i<workout_options.size();i++) {
        int price = workout_options[i].getPrice();
        int id = workout_options[i].getId();
        workouts.push_back(new tuple<int, int, WorkoutType>(price, id, workout_options[i].getType()));
    }
    std::sort(workouts.begin(), workouts.end(), [](const tuple<int, int, WorkoutType> *left, const tuple<int, int, WorkoutType> *right) {return std::get<0>(*left) < std::get<0>(*right);});
    std::vector<int> cardio;
    std::vector<int> mixType;
    std::vector<int> anaerobic;
    for(int i=0;i<workouts.size();i++)
    {
        if(std::get<2>(*workouts[i])==CARDIO)
            cardio.push_back(std::get<1>(*workouts[i]));
        if(std::get<2>(*workouts[i])==MIXED)
            mixType.insert(mixType.begin(), std::get<1>(*workouts[i]));
        if(std::get<2>(*workouts[i])==ANAEROBIC)
            anaerobic.push_back(std::get<1>(*workouts[i]));
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
std::string FullBodyCustomer::getType() const { return "fbd";}