#include "Studio.h"


//Studio
    Studio::Studio() : open(false) {}
    Studio::Studio(const std::string &configFilePath) {}
    void Studio::start() {}
    int Studio::getNumOfTrainers() const { }
    Trainer* Studio::getTrainer(int tid) {}
    const std::vector<BaseAction*>& Studio::getActionsLog() const { } // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

    /*
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    */