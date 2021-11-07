#include "Action.h"

    BaseAction::BaseAction() { }
    ActionStatus BaseAction::getStatus() const { }
    void BaseAction::complete() { }
    void BaseAction::error(std::string errorMsg) { }
    std::string BaseAction::getErrorMsg() const { }

    //Open Trainer
    OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) { }
    void OpenTrainer::act(Studio &studio) { }
    std::string OpenTrainer::toString() const { }

    //Order
    Order::Order(int id) : trainerId(id) { }
    void Order::act(Studio &studio) { }
    std::string Order::toString() const { }

    //Move Customer
    MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) { }
    void MoveCustomer::act(Studio &studio) { }
    std::string MoveCustomer::toString() const { }

    //Close
    Close::Close(int id) : trainerId(id) { }
    void Close::act(Studio &studio) { }
    std::string Close::toString() const { }

    //Close All
    CloseAll::CloseAll() { }
    void CloseAll::act(Studio &studio){ }
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