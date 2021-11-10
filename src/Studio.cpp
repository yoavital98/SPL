#include "Studio.h"
#include <fstream>


//Studio
Studio::Studio() : open(false) {}
Studio::Studio(const std::string &configFilePath)
{
    int x;
    std::ifstream inFile;
    inFile.open(configFilePath);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string line;
    int lineCount = 0 ;
    int workOutId = 0;
    while(getline(inFile, line)) {
        if(lineCount==4)
        {
            std::vector<std::string> words;
            size_t index = 0;
            while ((index = line.find(",")) != std::string::npos) {
                words.push_back(line.substr(0, index));
                line.erase(0, index + 1);
            }
            words.push_back(line);
            for(int i=0;i<words.size();i++)
            {
                trainers.push_back(new Trainer(stoi(words[i])));
            }
        }
        else if(lineCount>=7)
        {
            std::vector<std::string> words;
            size_t index = 0;
            while ((index = line.find(", ")) != std::string::npos) {
                words.push_back(line.substr(0, index));
                line.erase(0, index + 2);
            }
            words.push_back(line);
            WorkoutType workoutType;
            if(words[1] == "Anaerobic")
                workoutType = WorkoutType(ANAEROBIC);
            if(words[1] == "Mixed")
                workoutType = WorkoutType(MIXED);
            if(words[1] == "Cardio")
                workoutType = WorkoutType(CARDIO);
            else
            {
                WorkoutType workoutType;
            }
            workout_options.push_back(Workout(workOutId, words[0],stoi(words[2]),workoutType));
            workOutId++;
        }
        lineCount++;
    }
}
void Studio::start() { open = true;
    std::cout << "Studio is now open!" << std::endl;
    std::string  input="";
    while(open)
    {
        std::getline(std::cin, input);
        std::vector<std::string> words;
        size_t index = 0;
        while ((index = input.find(" ")) != std::string::npos) {
            words.push_back(input.substr(0, index));
            input.erase(0, index + 1);
        }
        words.push_back((input));
        if(words[0] == "open")
        {
            std::vector<Customer*> customers;
            for(int i=0;i<words.size()-2;i++) {
                if(words[i+2].substr(words[i+2].length()-3,3) == "swt")
                    customers.push_back(new SweatyCustomer(words[i+2].substr(0, words[i+2].length()-4), i));
                else if(words[i+2].substr(words[i+2].length()-3,3) == "chp")
                    customers.push_back(new CheapCustomer(words[i+2].substr(0, words[i+2].length()-4), i));
                else if(words[i+2].substr(words[i+2].length()-3,3) == "fbd")
                    customers.push_back(new FullBodyCustomer(words[i+2].substr(0, words[i+2].length()-4), i));
                else if(words[i+2].substr(words[i+2].length()-3,3) == "mcl")
                    customers.push_back(new HeavyMuscleCustomer(words[i+2].substr(0,words[i+2].length()-4), i));
            }
            OpenTrainer actionOpenTrainer(stoi(words[1]), customers);
            actionOpenTrainer.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionOpenTrainer);
        }
        if(words[0] == "order")
        {
            Order actionOrder(stoi(words[1]));
            actionOrder.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionOrder);
        }
        if(words[0] == "close")
        {
            Close actionClose(stoi(words[1]));
            actionClose.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionClose);
        }
        if(words[0] == "status")
        {
            PrintTrainerStatus actionPrintTrainerStatus(stoi(words[1]));
            actionPrintTrainerStatus.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionPrintTrainerStatus);
        }
        if(words[0] == "move")
        {
            MoveCustomer actionMoveCustomer(stoi(words[2]),stoi(words[3]),stoi(words[4]));
            actionMoveCustomer.act(*this);
            CloseStudio();
            actionsLog.insert(actionsLog.begin(), &actionMoveCustomer);
        }
        if(words[0] == "closeall")
        {
            CloseAll actionCloseAll;
            actionCloseAll.act(*this);
            CloseStudio();
            actionsLog.insert(actionsLog.begin(), &actionCloseAll);
        }
        if(words[0] == "backup")
        {
            BackupStudio actionBackUp;
            actionBackUp.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionBackUp);
        }
        if(words[0] == "restore")
        {
            RestoreStudio actionRestore;
            actionRestore.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionRestore);
        }
        if(words[0] == "log")
        {
            PrintActionsLog actionsLog1;
            actionsLog1.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionsLog1);
        }
        if(words[0] == "workout_options")
        {
            PrintWorkoutOptions actionsPrintWorkoutOptions;
            actionsPrintWorkoutOptions.act(*this);
            actionsLog.insert(actionsLog.begin(), &actionsPrintWorkoutOptions);
        }
    }
}
void Studio::CloseStudio() {
    open=false;
}
int Studio::getNumOfTrainers() const { return trainers.size();}
Trainer* Studio::getTrainer(int tid) {
    if(tid >= trainers.size())
        return nullptr;
    return trainers[tid];
}
const std::vector<BaseAction*>& Studio::getActionsLog() const { return actionsLog; } // Return a  return actionsLog; to the history of actions
std::vector<Workout>& Studio::getWorkoutOptions() { return workout_options; }

/*
bool open;
std::vector<Trainer*> trainers;
std::vector<Workout> workout_options;
std::vector<BaseAction*> actionsLog;
*/