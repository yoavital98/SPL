#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
    //Constructor
    Studio();
    Studio(const std::string &configFilePath);
    //Destructor
    virtual ~Studio();
    //Copy Constructor
    Studio(const Studio &other);
    //Move Constructor
    Studio(Studio &&other);
    //Copy Assignment
    Studio& operator=(const Studio &other);
    //Move Assignment
    Studio& operator=(Studio &&other);
    //========================================
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void CloseStudio();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void clear();
    void copy(const Studio &other);
};

#endif