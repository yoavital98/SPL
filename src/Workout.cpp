#include "Workout.h"
#include "../include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name), price(w_price),
                                                                                  type(w_type) {}
int Workout::getId() const {return id;}
std::string Workout::getName() const {return name;}
int Workout::getPrice() const {return price;}
WorkoutType Workout::getType() const {return type;}
Workout& Workout::operator=(const Workout& other)
{
this ->~Workout();
new (this) Workout(other);
return *this;
}
bool Workout::operator < (const Workout &other) const {
    return price < other.price;
}
/*
const int id;
const std::string name;
const int price;
const WorkoutType type;
 */
