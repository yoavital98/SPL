#include "Studio.h"
#include <fstream>


//Constructors
Studio::Studio() : open(false), trainers(), workout_options(), actionsLog() {}
Studio::Studio(const std::string &configFilePath) : open(false), trainers(), workout_options(), actionsLog()
{
    std::ifstream inFile;
    inFile.open(configFilePath);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    std::string line;
    int lineCount = 0 ;
    int workoutId = 0;
    while(getline(inFile, line)) {
        if(lineCount == 1)
        {
            trainers.reserve(std::stoi(line));
        }
        if(lineCount==4)
        {
            std::vector<std::string> words;
            size_t index = 0;
            while ((index = line.find(",")) != std::string::npos) {
                words.push_back(line.substr(0, index));
                line.erase(0, index + 1);
            }
            words.push_back(line);
            for(long unsigned int i=0;i<words.size();i++)
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
            if(words.size()<2)
                continue;
            WorkoutType workoutType;
            if(words[1] == "Anaerobic")
                workoutType = WorkoutType(ANAEROBIC);
            if(words[1] == "Mixed")
                workoutType = WorkoutType(MIXED);
            if(words[1] == "Cardio")
                workoutType = WorkoutType(CARDIO);
            workout_options.emplace_back(workoutId, words[0],stoi(words[2]),workoutType);
            workoutId++;
        }
        lineCount++;
    }
}

//Destructor
Studio::~Studio() {
    clear();
}
//Copy Constructor
Studio::Studio(const Studio &other) : open(other.open), trainers(), workout_options(other.workout_options), actionsLog() {
    copy(other);
}
//Move Constructor
Studio::Studio(Studio &&other) : open(other.open), trainers(), workout_options(other.workout_options), actionsLog(){
    copyMove(other);
}

//Copy Assignment
Studio& Studio::operator=(const Studio &other){
    if(this == &other){
        clear();
        workout_options = other.workout_options;
        open = other.open;
        copy(other);
    }
    return *this;
}
//Move Assignment
Studio& Studio::operator=(Studio &&other){
    if(this != &other){
        clear();
        open = other.open;
        workout_options = other.workout_options;
        copyMove(other);
    }
    return *this;
}
void Studio::clear() {
    for(long unsigned int i = 0;i<trainers.size();i++)
    {
        if(trainers[i])
        {
            delete (trainers[i]);
        }
    }
    for(long unsigned int i = 0;i<actionsLog.size();i++)
    {
        if(actionsLog[i])
        {
            delete (actionsLog[i]);
        }
    }
    open = false;
    actionsLog.clear();
    trainers.clear();
    workout_options.clear();
}
void Studio::copy(const Studio &other){
    for(Trainer *t : other.trainers){
        trainers.push_back(new Trainer(*t));
    }
    for(BaseAction* baseAction : other.actionsLog){
        actionsLog.push_back(baseAction->getAction());
    }
}
void Studio::copyMove(Studio &other){
    for(long unsigned int i=0;i<other.trainers.size();i++)
    {
        trainers.push_back(other.trainers[i]);
        other.trainers[i] = nullptr;
    }
    for(long unsigned int i=0;i<other.actionsLog.size();i++)
    {
        actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i] = nullptr;
    }
}
void Studio::start() {
    open = true;
    std::cout << "Studio is now open!" << std::endl;
    int customerID = 0;
    while(open)
    {
        std::vector<std::string> words = getInput();
        try {  makeAction(words, customerID); } catch (const std::exception& e) { std::cout << "illegal input" << std::endl; }
    }
}
void Studio::CloseStudio() {
    open=false;
}
int Studio::getNumOfTrainers() const { return trainers.size();}
Trainer* Studio::getTrainer(int tid) {
    if(tid >= (int)trainers.size())
        return nullptr;
    return trainers[tid];
}
const std::vector<BaseAction*>& Studio::getActionsLog() const { return actionsLog; } // Return a  return actionsLog; to the history of actions
std::vector<Workout>& Studio::getWorkoutOptions() { return workout_options; }
std::vector<std::string> Studio::getInput()
{
    std::string  input= "";
    std::getline(std::cin, input);
    std::vector<std::string> words;
    size_t index = 0;
    while ((index = input.find(" ")) != std::string::npos) {
        words.push_back(input.substr(0, index));
        input.erase(0, index + 1);
    }
    words.push_back((input));
    return words;
}
void Studio::makeAction(const std::vector<std::string> &inputWords, int& customerID) {
    if(inputWords[0] == "open")
    {
        if(!inputValid(inputWords))
            return;
        std::vector<Customer*> customers;
        for(long unsigned int i=0;i<inputWords.size()-2;i++) {
            if(inputWords[i+2].substr(inputWords[i+2].length()-3,3) == "swt")
                customers.push_back(new SweatyCustomer(inputWords[i+2].substr(0, inputWords[i+2].length()-4), customerID));
            else if(inputWords[i+2].substr(inputWords[i+2].length()-3,3) == "chp")
                customers.push_back(new CheapCustomer(inputWords[i+2].substr(0, inputWords[i+2].length()-4), customerID));
            else if(inputWords[i+2].substr(inputWords[i+2].length()-3,3) == "fbd")
                customers.push_back(new FullBodyCustomer(inputWords[i+2].substr(0, inputWords[i+2].length()-4), customerID));
            else if(inputWords[i+2].substr(inputWords[i+2].length()-3,3) == "mcl")
                customers.push_back(new HeavyMuscleCustomer(inputWords[i+2].substr(0,inputWords[i+2].length()-4), customerID));
            customerID++;
        }
        OpenTrainer* actionOpenTrainer = new OpenTrainer(stoi(inputWords[1]),  customers);
        actionOpenTrainer->act(*this);
        actionsLog.insert(actionsLog.begin(), actionOpenTrainer);
    }
    if(inputWords[0] == "order")
    {
        if(!inputValid(inputWords))
            return;
        Order* actionOrder = new Order(stoi(inputWords[1]));
        actionOrder->act(*this);
        actionsLog.insert(actionsLog.begin(), actionOrder);
    }
    if(inputWords[0] == "close")
    {
        if(!inputValid(inputWords))
            return;
        Close* actionClose = new Close(stoi(inputWords[1]));
        actionClose->act(*this);
        actionsLog.insert(actionsLog.begin(), actionClose);
    }
    if(inputWords[0] == "status")
    {
        if(!inputValid(inputWords))
            return;
        PrintTrainerStatus* actionPrintTrainerStatus = new PrintTrainerStatus(stoi(inputWords[1]));
        actionPrintTrainerStatus->act(*this);
        actionsLog.insert(actionsLog.begin(), actionPrintTrainerStatus);
    }
    if(inputWords[0] == "move")
    {
        if(!inputValid(inputWords))
            return;
        MoveCustomer* actionMoveCustomer = new MoveCustomer(stoi(inputWords[1]),stoi(inputWords[2]),stoi(inputWords[3]));
        actionMoveCustomer->act(*this);
        actionsLog.insert(actionsLog.begin(), actionMoveCustomer);
    }
    if(inputWords[0] == "closeall")
    {
        CloseAll* actionCloseAll = new CloseAll();
        actionCloseAll->act(*this);
        CloseStudio();
        actionsLog.insert(actionsLog.begin(), actionCloseAll);
    }
    if(inputWords[0] == "backup")
    {
        BackupStudio* actionBackUp = new BackupStudio();
        actionBackUp->act(*this);
        actionsLog.insert(actionsLog.begin(), actionBackUp);
    }
    if(inputWords[0] == "restore")
    {
        RestoreStudio* actionRestore = new RestoreStudio();
        actionRestore->act(*this);
        actionsLog.insert(actionsLog.begin(), actionRestore);
    }
    if(inputWords[0] == "log")
    {
        PrintActionsLog* actionsLog1 = new PrintActionsLog();
        actionsLog1->act(*this);
        actionsLog.insert(actionsLog.begin(), actionsLog1);
    }
    if(inputWords[0] == "workout_options")
    {
        PrintWorkoutOptions* actionsPrintWorkoutOptions = new PrintWorkoutOptions();
        actionsPrintWorkoutOptions->act(*this);
        actionsLog.insert(actionsLog.begin(), actionsPrintWorkoutOptions);
    }
}


bool Studio::inputValid(const std::vector<std::string> &inputWords) {
    if(inputWords.size()<2)
    {
        std::cout << "illegal input" << std::endl;
        return false;
    }
    return true;
}

/*
fields:
bool open;
std::vector<Trainer*> trainers;
std::vector<Workout> workout_options;
std::vector<BaseAction*> actionsLog;
*/