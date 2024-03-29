#include "Customer.h"
#include <algorithm>
#include <vector>
#include <tuple>
using namespace std;
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) { }
std::string Customer::getName() const { return name;}
int Customer::getId() const { return id; }
Customer::~Customer(){}
//SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id)  { }
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> workout_list;
    for(long unsigned int i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO)
        {
            workout_list.push_back(workout_options[i].getId());
        }
    }
    return workout_list;
}
std::string SweatyCustomer::toString() const { return Customer::getName()+",swt"; }
Customer* SweatyCustomer::getCustomer()
{
    return new SweatyCustomer(this->getName(), this->getId());
}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> workout_list;
    if(workout_options.size() == 0)
        return workout_list;
    int price = workout_options[0].getPrice();
    int id = 0;
    for(long unsigned int i=1;i<workout_options.size();i++) {
        if(price>workout_options[i].getPrice())
        {
            price = workout_options[i].getPrice();
            id = workout_options[i].getId();
        }
    }
    workout_list.push_back(id);
    return workout_list;
}
std::string CheapCustomer::toString() const { return Customer::getName()+",chp";}
Customer* CheapCustomer::getCustomer()
{
    return new CheapCustomer(this->getName(), this->getId());
}

//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<tuple<int, int, WorkoutType>*> workouts;
    for(long unsigned int i=0;i<workout_options.size();i++) {
        int price = workout_options[i].getPrice();
        int id = workout_options[i].getId();
        workouts.push_back(new tuple<int, int, WorkoutType>(price, id, workout_options[i].getType()));
    }
    std::sort(workouts.begin(), workouts.end(), [](const tuple<int, int, WorkoutType> *left, const tuple<int, int, WorkoutType> *right) {return std::get<0>(*left) < std::get<0>(*right);});
    std::vector<int> workout;
    for(long unsigned int i=0; i< workouts.size(); i++){
        if(std::get<2>(*workouts[i]) == ANAEROBIC) {
            workout.insert(workout.begin(),std::get<1>(*workouts[i]));
        }
    }
    for(long unsigned int i=0;i<workouts.size();i++) {
        if(workouts[i])
            delete workouts[i];
    }
    workouts.clear();
    return workout;
}
std::string HeavyMuscleCustomer::toString() const { return Customer::getName()+",mcl";}
Customer* HeavyMuscleCustomer::getCustomer()
{
    return new HeavyMuscleCustomer(this->getName(), this->getId());
}


//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) { }
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<tuple<int, int, WorkoutType>*> workouts;
    for(long unsigned int i=0;i<workout_options.size();i++) {
        int price = workout_options[i].getPrice();
        int id = workout_options[i].getId();
        workouts.push_back(new tuple<int, int, WorkoutType>(price, id, workout_options[i].getType()));
    }
    std::sort(workouts.begin(), workouts.end(), [](const tuple<int, int, WorkoutType> *left, const tuple<int, int, WorkoutType> *right) {return std::get<0>(*left) < std::get<0>(*right);});
    std::vector<int> cardio;
    std::vector<int> mixType;
    std::vector<int> anaerobic;
    for(long unsigned int i=0;i<workouts.size();i++)
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
    for(long unsigned int i=0;i<workouts.size();i++) {
        if(workouts[i])
            delete workouts[i];
    }
    workouts.clear();
    return workout;
}
std::string FullBodyCustomer::toString() const { return getName()+",fbd" ;}
Customer* FullBodyCustomer::getCustomer()
{
    return new FullBodyCustomer(this->getName(), this->getId());
}