#include "Action.h"
#include "Studio.h"

BaseAction::BaseAction() { }
ActionStatus BaseAction::getStatus() const { return status; }
void BaseAction::complete() { status = COMPLETED; }
void BaseAction::error(std::string errorMsg) { BaseAction::errorMsg = errorMsg; status=ERROR; std::cout << errorMsg << std::endl;}
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
}
//Move Constructor
OpenTrainer::OpenTrainer(OpenTrainer &&other) : trainerId(other.trainerId){
    for(Customer *c: other.customers) {
        customers.push_back(c);
        c = nullptr;
    }
}
//Copy Assignment
OpenTrainer& OpenTrainer::operator=(const OpenTrainer &other){
    if(this == &other)
        return *this;
    clear();
    for(Customer *c: other.customers)
        customers.push_back(c->getCustomer());
    return *this;
}
//Move Assignment
OpenTrainer& OpenTrainer::operator=(OpenTrainer &&other){
if(this!=&other)
{
    this->~OpenTrainer();
    new (this) OpenTrainer(other);
    for(Customer *c: other.customers) {
        customers.push_back(c);
        c = nullptr;
    }
}
return *this;
}
void OpenTrainer::clear() {
    for(Customer *c: customers)
    {
        if(c)
        {
            delete c;
            c = nullptr;
        }
    }
    customers.clear();
}
void OpenTrainer::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t== nullptr || t->isOpen())
        BaseAction::error("Workout session does not exist or is already open");
    else
    {
        if(customers.size()>t->getCapacity()-t->getCustomers().size())
            BaseAction::error("Cannot open a trainer session with more customers than its capacity");
        else
            for(long unsigned int i=0;i<customers.size();i++)
                t->addCustomer(customers[i]);
        complete();
        t->openTrainer();
    }
}
std::string OpenTrainer::toString() const {
    std::string customersName="";
    for(long unsigned int i=0;i<customers.size();i++)
        customersName = customersName + customers[i]->getName() + ",";
    if(customersName.length()>=1)
        customersName.pop_back();
    if(getStatus() == COMPLETED)
        return "open " + std::to_string(trainerId) + " " + customersName +" Completed";
    return "open " + std::to_string(trainerId) + " " + customersName+ " "+getErrorMsg();
}
BaseAction* OpenTrainer::getAction()
{
    std::vector<Customer*> customerslist;
    for(Customer *c: customers)
    {
        customerslist.push_back(c->getCustomer());
    }
    return new OpenTrainer(this->trainerId,customerslist);
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
        return "Order " + std::to_string(trainerId) + " Completed";
    return "Order " + std::to_string(trainerId) + " " + getErrorMsg();
}
BaseAction* Order::getAction()
{
    return new Order(this->trainerId);
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
        Customer* Mosh = tSRC->getCustomer(id);
        std::vector<OrderPair>& orders = tSRC->getOrders();
        for(long unsigned int i=0; i<orders.size(); i++)
            if(orders[i].first == id)
                tDST->addOrder(orders[i]);
        tSRC->removeCustomer(id);
        tDST->addCustomer(Mosh);
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
    return new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
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
        return "Close " + std::to_string(trainerId) + " Completed";
    return "Close " + std::to_string(trainerId) + " "+ getErrorMsg();
}
BaseAction* Close::getAction()
{
    return new Close(this->trainerId);
}
//==============================================================================================
//==============================================================================================
//Close All
CloseAll::CloseAll() { }
void CloseAll::act(Studio &studio){
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Trainer *t = studio.getTrainer(i);
        if(t->isOpen()) {
            Close *closingTrainer = new Close(i);
            closingTrainer->act(studio);
        }
    }
    complete();
}
std::string CloseAll::toString() const {
    if(getStatus() == COMPLETED)
        return "CloseAll Completed";
    return "CloseAll " + getErrorMsg();
}
BaseAction* CloseAll::getAction()
{
    return new CloseAll();
}
//==============================================================================================
//==============================================================================================
//Print Workout Options
PrintWorkoutOptions::PrintWorkoutOptions() { }
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts = studio.getWorkoutOptions();
    for(long unsigned int i=0;i<workouts.size();i++)
    {
        std::cout << workouts[i].getName() << +", " << workouts[i].getType() << ", " << workouts[i].getPrice() << std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {
    if(getStatus() == COMPLETED)
        return "PrintWorkoutOptions Completed";
    return "PrintWorkoutOptions " + getErrorMsg();
}
BaseAction* PrintWorkoutOptions::getAction()
{
    return new PrintWorkoutOptions();
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
        return "PrintTrainerStatus Completed";
    return "PrintTrainerStatus " + getErrorMsg();
}
BaseAction* PrintTrainerStatus::getAction()
{
    return new PrintTrainerStatus(this->trainerId);
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
        return "PrintActionsLog Completed";
    return "PrintActionsLog " + getErrorMsg();
}
BaseAction* PrintActionsLog::getAction()
{
    return new PrintActionsLog();
}
//==============================================================================================
//==============================================================================================
//Backup Studio
BackupStudio::BackupStudio() { }
void BackupStudio::act(Studio &studio){
    backup = new Studio(studio);
    std::vector<Workout> workouts = backup->getWorkoutOptions();
    for(Workout workout: workouts)
        std::cout << workout.getName() << std::endl;
}
std::string BackupStudio::toString() const {
    if(getStatus() == COMPLETED)
        return "BackupStudio Completed";
    return "BackupStudio " + getErrorMsg();
}
BaseAction* BackupStudio::getAction()
{
    return new BackupStudio();
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
        return "RestoreStudio Completed";
    return "RestoreStudio " + getErrorMsg();
}
BaseAction* RestoreStudio::getAction()
{
    return new RestoreStudio();
}