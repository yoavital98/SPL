#include "Workout.h"

class Workout{
public:
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name), price(w_price), type(w_type) { }
    int getId() const
    {
        return id;
    }
    std::string getName() const
    {
        return name;
    }
    int getPrice() const
    {
        return price;
    }
    WorkoutType getType() const {
        return type;
    }
private:
    const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif