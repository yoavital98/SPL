#include "Action.h"
#include "Studio.h"

BaseAction::BaseAction() { }
ActionStatus BaseAction::getStatus() const { return status; }
void BaseAction::complete() { status = COMPLETED; }
void BaseAction::error(std::string errorMsg) { BaseAction::errorMsg = errorMsg; status=ERROR; std::cout << errorMsg << std::endl;}
std::string BaseAction::getErrorMsg() const { return "Error: "+errorMsg; }
//==============================================================================================
//==============================================================================================
//Open Trainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) { }
void OpenTrainer::act(Studio &studio) {
    Trainer* t = studio.getTrainer(trainerId);
    if(t== nullptr || t->isOpen())
        BaseAction::error("Workout session does not exist or is already open");
    else
    {
        if(customers.size()>t->getCapacity())
            BaseAction::error("Cannot open a trainer session with more customers than its capacity");
        else
            for(int i=0;i<customers.size();i++)
                t->addCustomer(customers[i]);
        complete();
    }
}

std::string OpenTrainer::toString() const {
    std::string customersName="";
    for(int i=0;i<customers.size();i++)
        customersName = customersName + customers[i]->getName() + ",";
    if(customersName.length()>=1)
        customersName.pop_back();
    if(getStatus() == COMPLETED)
        return "open " + std::to_string(trainerId) + " " + customersName +" Completed";
    else
        return "open " + std::to_string(trainerId) + " " + customersName+ " "+getErrorMsg();
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
        std::vector<int> ids;
        std::vector<Workout> workouts = studio.getWorkoutOptions();
        for(int i=0;i<customers.size();i++)
        {
            ids = customers[i]->order(workouts);
            for(int i=0;i<ids.size();i++)
            {
                std::cout << customers[i]->toString() << " Is Doing " << workouts[ids[i]].getName() << std::endl;
            }
        }
        complete();
    }
}
std::string Order::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "Order " << std::to_string(trainerId) << " Completed";
    else
        std::cout << "Order " << std::to_string(trainerId) << " "<< getErrorMsg();
}
//==============================================================================================
//==============================================================================================
//Move Customer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) { }
void MoveCustomer::act(Studio &studio) {
    Trainer* tSRC = studio.getTrainer(srcTrainer);
    Trainer* tDST = studio.getTrainer(dstTrainer);

    if(tSRC== nullptr || !tSRC->isOpen() || tDST== nullptr || !tDST->isOpen()
       || tSRC->getCustomer(id) == nullptr || tDST->getCustomers().size() >= tDST->getCapacity())
        BaseAction::error("Cannot move customer");

    else{
        Customer* Mosh = tSRC->getCustomer(id);
        std::vector<OrderPair>& orders = tSRC->getOrders();
        for(int i=0; i<orders.size(); i++)
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
        std::cout << "move " << srcTrainer << " " << dstTrainer << " " << id << " Completed";
    else
        std::cout << "move " << srcTrainer << " " << dstTrainer << " " << id << " " << getErrorMsg();
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
        std::cout << "Close " << std::to_string(trainerId) << " Completed";
    else
        std::cout << "Close " << std::to_string(trainerId) << " "<< getErrorMsg();

}
//==============================================================================================
//==============================================================================================
//Close All
CloseAll::CloseAll() { }
void CloseAll::act(Studio &studio){
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        Close* closingTrainer = new Close(i);
        closingTrainer->act(studio);
    }
    complete();
}
std::string CloseAll::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "CloseAll " << " Completed";
    else
        std::cout << "CloseAll "  << " "<< getErrorMsg();
}
//==============================================================================================
//==============================================================================================
//Print Workout Options
PrintWorkoutOptions::PrintWorkoutOptions() { }
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workouts = studio.getWorkoutOptions();
    for(int i=0;workouts.size();i++)
    {
        std::cout << workouts[i].getName() << +", " << workouts[i].getType() << ", " << workouts[i].getPrice() << std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "PrintWorkoutOptions " << " Completed";
    else
        std::cout << "PrintWorkoutOptions "  << " "<< getErrorMsg();
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
        std::cout << "Customers" << std::endl;
        std::vector<Customer*> customers = t->getCustomers();
        for(int i=0;i<customers.size();i++)
            std::cout << customers[i]->getId() << " " << customers[i]->getName() << std::endl;
        std::cout << "Orders:" << std::endl;
        std::vector<std::pair<int, Workout>> orders = t->getOrders();
        for(int i=0;i<orders.size();i++)
            std::cout << orders[i].second.getName() << " " << orders[i].second.getPrice() << " " << orders[i].first << std::endl;
        std::cout << "Current Trainer’s Salary: " << t->getSalary() << std::endl;
        complete();
    }

}
std::string PrintTrainerStatus::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "PrintTrainerStatus " << " Completed";
    else
        std::cout << "PrintTrainerStatus "  << " "<< getErrorMsg();
}
//==============================================================================================
//==============================================================================================
//PrintActionsLog
PrintActionsLog::PrintActionsLog() { }
void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> actionsLog = studio.getActionsLog();
    for(int i=0;i<actionsLog.size();i++)
        std::cout << actionsLog[i]->toString();
}
std::string PrintActionsLog::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "PrintActionsLog " << " Completed";
    else
        std::cout << "PrintActionsLog "  << " "<< getErrorMsg();
}
//==============================================================================================
//==============================================================================================
//Backup Studio
BackupStudio::BackupStudio() { }
void BackupStudio::act(Studio &studio){
    backup = new Studio(studio);
}
std::string BackupStudio::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "BackupStudio " << " Completed";
    else
        std::cout << "BackupStudio "  << " "<< getErrorMsg();
}
//==============================================================================================
//==============================================================================================
//Restore Studio
RestoreStudio::RestoreStudio() { }
void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr)
        BaseAction::error("Cannot move customer");
    else{
        studio = *backup;
    }

}
std::string RestoreStudio::toString() const {
    if(getStatus() == COMPLETED)
        std::cout << "RestoreStudio " << " Completed";
    else
        std::cout << "RestoreStudio "  << " "<< getErrorMsg();
}