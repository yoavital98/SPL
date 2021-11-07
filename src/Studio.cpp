#include "Studio.h"
#include <fstream>


//Studio
    Studio::Studio() : open(false) {}
    Studio::Studio(const std::string &configFilePath)
    {
        int x;
        std::ifstream inFile;
        inFile.open("ExampleInput.txt");
        if (!inFile) {
            std::cout << "Unable to open file";
            exit(1); // terminate with error
        }
    }
    void Studio::start() { open = true; }
    int Studio::getNumOfTrainers() const { return trainers.size();}
    Trainer* Studio::getTrainer(int tid) { return trainers[tid];}
    const std::vector<BaseAction*>& Studio::getActionsLog() const { return actionsLog; } // Return a  return actionsLog; to the history of actions
    std::vector<Workout>& Studio::getWorkoutOptions() { return workout_options; }

    /*
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    */