#include "Action.h"
#include "Studio.h"

BaseAction::BaseAction() { }
ActionStatus BaseAction::getStatus() const { return status; }
void BaseAction::complete() { status = COMPLETED; }
void BaseAction::error(std::string errorMsg) { BaseAction::errorMsg = errorMsg; status=ERROR; std::cout << errorMsg << std::endl;}
void BaseAction::setErrorMsg(std::string errorMsg)
{
    this->errorMsg = errorMsg;
}
void BaseAction::setStatus(ActionStatus status)
{
    this->status = status;
}
std::string BaseAction::getErrorMsg() const { return "Error: "+errorMsg; }
BaseAction::~BaseAction(){}

//==============================================================================================
//==============================================================================================
//Open Trainer
//Constructor
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) { }
//Destructor
OpenTrainer::~OpenTrainer(){
    clear();
}
//Copy Constructor
OpenTrainer::OpenTrainer(const OpenTrainer &other) : trainerId(other.trainerId){
    for(Customer *c: other.customers)
    {
        customers.push_back(c->getCustomer());
    }
    setErrorMsg(other.getErrorMsg());
    setStatus(other.getStatus());
}
//Move Constructor
OpenTrainer::OpenTrainer(OpenTrainer &&other) : trainerId(other.trainerId){
    for(Customer *c: other.customers) {
        customers.push_back(c);
        c = nullptr;
    }
}
void OpenTrainer::clear() {
    for(long unsigned int i = 0;i<customers.size();i++)
        if(customers[i] != nullptr)
            delete customers[i];
    customers.clear();
}
void OpenTrainer::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t== nullptr || t->isOpen())
        BaseAction::error("Workout session does not exist or is already open");
    else {
        long unsigned int slots = t->getCapacity() - t->getCustomers().size(); //size of free slots in a trainer
        std::vector<Customer*> c;
            for (long unsigned int i = 0; i < customers.size() && i<slots; i++){
                c.push_back(customers[i]->getCustomer());
            }
        for(long unsigned int i=0;i<c.size();i++)
            t->addCustomer(c[i]);
        complete();
        t->openTrainer();
    }
}
std::string OpenTrainer::toString() const {
    std::string customersName="";
    for(long unsigned int i=0;i<customers.size();i++)
        customersName = customersName + customers[i]->toString() + " ";
    if(getStatus() == COMPLETED)
        return "open " + std::to_string(trainerId) + " " + customersName +"Completed";
    return "open " + std::to_string(trainerId) + " " + customersName+getErrorMsg();
}
BaseAction* OpenTrainer::getAction()
{
    return new OpenTrainer(*this);
}
//==============================================================================================
//==============================================================================================
//Order
Order::Order(int id) : trainerId(id) { }
void Order::act(Studio &studio) {
    Trainer* t = studio.getTrainer((trainerId));
    if(t== nullptr || !t->isOpen())
        BaseAction::error("Trainer does not exist or is not open");
    else
    {
        std::vector<Customer*> customers = t->getCustomers();
        for(Customer * customer: customers)
            t->order(customer->getId(),customer->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        complete();
    }
}
std::string Order::toString() const {
    if(getStatus() == COMPLETED)
        return "order " + std::to_string(trainerId) + " Completed";
    return "order " + std::to_string(trainerId) + " " + getErrorMsg();
}
BaseAction* Order::getAction()
{
    Order* order = new Order(trainerId);
    order->setErrorMsg(getErrorMsg());
    order->setStatus(getStatus());
    return order;
}
//==============================================================================================
//==============================================================================================
//Move Customer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) { }
void MoveCustomer::act(Studio &studio) {
    Trainer* tSRC = studio.getTrainer(srcTrainer);
    Trainer* tDST = studio.getTrainer(dstTrainer);
    if(tSRC== nullptr || !tSRC->isOpen() || tDST== nullptr || !tDST->isOpen()
       || tSRC->getCustomer(id) == nullptr || (int)tDST->getCustomers().size() >= tDST->getCapacity())
        BaseAction::error("Cannot move customer");

    else{
        Customer* c = tSRC->getCustomer(id);
        std::vector<OrderPair>& orders = tSRC->getOrders();
        for(long unsigned int i=0; i<orders.size(); i++)
            if(orders[i].first == id)
                tDST->addOrder(orders[i]);
        tSRC->removeCustomer(id);
        tDST->addCustomer(c);
        if(tSRC->getCustomers().size()==0)
            tSRC->closeTrainer();
        complete();
    }
}

std::string MoveCustomer::toString() const {
    if(getStatus() == COMPLETED)
        return "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " Completed";
    return "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " " + getErrorMsg();
}
BaseAction* MoveCustomer::getAction()
{
    MoveCustomer* moveCustomer = new MoveCustomer(srcTrainer, dstTrainer, id);
    moveCustomer->setErrorMsg(getErrorMsg());
    moveCustomer->setStatus(getStatus());
    return moveCustomer;
}
//==============================================================================================
//==============================================================================================
//Close
Close::Close(int id) : trainerId(id) { }
void Close::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t == nullptr || !t->isOpen())
        std::cout << "Trainer does not exist or is not open" << std::endl;
    else{
        std::cout << "Trainer " << trainerId << " closed. Salary " << t->getSalary() << "NIS" << std::endl;
        t->removeCustomers();
        t->closeTrainer();
        complete();
    }
}

std::string Close::toString() const {
    if(getStatus() == COMPLETED)
        return "close " + std::to_string(trainerId) + " Completed";
    return "close " + std::to_string(trainerId) + " "+ getErrorMsg();
}
BaseAction* Close::getAction()
{
    Close* close = new Close(trainerId);
    close->setErrorMsg(getErrorMsg());
    close->setStatus(getStatus());
    return close;
}
//==============================================================================================
//==============================================================================================
//Close All
CloseAll::CloseAll() { }
void CloseAll::act(Studio &studio){
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Trainer *t = studio.getTrainer(i);
        if(t->isOpen()) {
            Close *closeTrainer = new Close(i);
            closeTrainer->act(studio);
            delete closeTrainer;
        }
    }
    complete();
}
std::string CloseAll::toString() const {
    if(getStatus() == COMPLETED)
        return "closeAll Completed";
    return "closeAll " + getErrorMsg();
}
BaseAction* CloseAll::getAction()
{
    CloseAll* closeAll = new CloseAll();
    closeAll->setErrorMsg(getErrorMsg());
    closeAll->setStatus(getStatus());
    return closeAll;
}
//==============================================================================================
//==============================================================================================
//Print Workout Options
PrintWorkoutOptions::PrintWorkoutOptions() { }
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts = studio.getWorkoutOptions();
    for(long unsigned int i=0;i<workouts.size();i++)
    {
        std::string workoutNames[] = { "Anaerobic", "Mixed", "Cardio" };
        std::cout << workouts[i].getName() << +", " << workoutNames[workouts[i].getType()] << ", " << workouts[i].getPrice() << std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {
    if(getStatus() == COMPLETED)
        return "workout_options Completed";
    return "workout_options " + getErrorMsg();
}
BaseAction* PrintWorkoutOptions::getAction()
{
    PrintWorkoutOptions* printWorkoutOptions = new PrintWorkoutOptions();
    printWorkoutOptions->setErrorMsg(getErrorMsg());
    printWorkoutOptions->setStatus(getStatus());
    return printWorkoutOptions;
}
//==============================================================================================
//==============================================================================================
//Print Trainer Status
PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id){ }
void PrintTrainerStatus::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(!t->isOpen())
        std::cout << "Trainer " << trainerId << " status: closed" << std::endl;
    else
    {
        std::cout << "Trainer " << trainerId << " status: open" << std::endl;
        std::cout << "Customers:" << std::endl;
        std::vector<Customer*> customers = t->getCustomers();
        for(long unsigned int i=0;i<customers.size();i++)
            std::cout << customers[i]->getId() << " " << customers[i]->getName() << std::endl;
        std::cout << "Orders:" << std::endl;
        std::vector<std::pair<int, Workout>> orders = t->getOrders();
        for(long unsigned int i=0;i<orders.size();i++)
            std::cout << orders[i].second.getName() << " " << orders[i].second.getPrice() << "NIS " << orders[i].first << std::endl;
        std::cout << "Current Trainer's Salary: " << t->getSalary() << "NIS" << std::endl;
        complete();
    }

}

std::string PrintTrainerStatus::toString() const {
    if(getStatus() == COMPLETED)
        return "status "+std::to_string(trainerId)+" Completed";
    return "status "+std::to_string(trainerId)+" " + getErrorMsg();
}
BaseAction* PrintTrainerStatus::getAction()
{
    PrintTrainerStatus* printTrainerStatus = new PrintTrainerStatus(trainerId);
    printTrainerStatus->setErrorMsg(getErrorMsg());
    printTrainerStatus->setStatus(getStatus());
    return printTrainerStatus;
}
//==============================================================================================
//==============================================================================================
//PrintActionsLog
PrintActionsLog::PrintActionsLog() { }
void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> actionsLog = studio.getActionsLog();
    for(BaseAction* baseAction: actionsLog)
        std::cout << baseAction->toString() << std::endl;
}
std::string PrintActionsLog::toString() const {
    if(getStatus() == COMPLETED)
        return "log Completed";
    return "log " + getErrorMsg();
}
BaseAction* PrintActionsLog::getAction()
{
    PrintActionsLog* printActionsLog = new PrintActionsLog();
    printActionsLog->setErrorMsg(getErrorMsg());
    printActionsLog->setStatus(getStatus());
    return printActionsLog;
}
//==============================================================================================
//==============================================================================================
//Backup Studio
BackupStudio::BackupStudio() { }
void BackupStudio::act(Studio &studio){
    if(backup)
        delete backup;
    backup = new Studio(studio);
}
std::string BackupStudio::toString() const {
    if(getStatus() == COMPLETED)
        return "backup Completed";
    return "backup " + getErrorMsg();
}
BaseAction* BackupStudio::getAction()
{
    BackupStudio* backupStudio = new BackupStudio();
    backupStudio->setErrorMsg(getErrorMsg());
    backupStudio->setStatus(getStatus());
    return backupStudio;
}
//==============================================================================================
//==============================================================================================
//Restore Studio
RestoreStudio::RestoreStudio() { }
void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr)
        BaseAction::error("No backup available");
    else{
        studio = Studio(*backup);
    }
}
std::string RestoreStudio::toString() const {
    if(getStatus() == COMPLETED)
        return "restore Completed";
    return "restore " + getErrorMsg();
}
BaseAction* RestoreStudio::getAction()
{
    RestoreStudio* restoreStudio = new RestoreStudio();
    restoreStudio->setErrorMsg(getErrorMsg());
    restoreStudio->setStatus(getStatus());
    return restoreStudio;
}