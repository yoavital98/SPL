#include "Action.h"
#include "Studio.h"

    BaseAction::BaseAction() { }
    ActionStatus BaseAction::getStatus() const { return status; }
    void BaseAction::complete() { status = COMPLETED; }
    void BaseAction::error(std::string errorMsg) { BaseAction::errorMsg = errorMsg; status=ERROR; }
    std::string BaseAction::getErrorMsg() const { return "Error: "+errorMsg; }

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
    }


    }
    std::string OpenTrainer::toString() const { }

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
                   std::cout << customers[i]->toString() << " Is Doing " << workouts[ids[i]].getName();
                }
            }
        }
}
    std::string Order::toString() const { }

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
        }
    }
    std::string MoveCustomer::toString() const { }

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
        }

    }
    std::string Close::toString() const { }

    //Close All
    CloseAll::CloseAll() { }
    void CloseAll::act(Studio &studio){
        for (int i = 0; i < studio.getNumOfTrainers(); i++) {
            Close* closingTrainer = new Close(i);
            closingTrainer->act(studio);
        }
        studio.CloseStudio();
    }
    std::string CloseAll::toString() const { }

    //Print Workout Options
    PrintWorkoutOptions::PrintWorkoutOptions() { }
    void PrintWorkoutOptions::act(Studio &studio) { }
    std::string PrintWorkoutOptions::toString() const { }

    //Print Trainer Status
    PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id){ }
    void PrintTrainerStatus::act(Studio &studio) { }
    std::string PrintTrainerStatus::toString() const { }

    //PrintActionsLog
    PrintActionsLog::PrintActionsLog() { }
    void PrintActionsLog::act(Studio &studio) { }
    std::string PrintActionsLog::toString() const { }

    //Backup Studio
    BackupStudio::BackupStudio() { }
    void BackupStudio::act(Studio &studio){ }
    std::string BackupStudio::toString() const { }

    //Restore Studio
    RestoreStudio::RestoreStudio() { }
    void RestoreStudio::act(Studio &studio) { }
    std::string RestoreStudio::toString() const { }