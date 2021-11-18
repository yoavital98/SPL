#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Action.h"
enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;
extern Studio* backup;
class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* getAction() = 0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
};

class OpenTrainer : public BaseAction {
public:
    //Constructor
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    //Destructor
    virtual ~OpenTrainer();
    //Copy Constructor
    OpenTrainer(const OpenTrainer &other);
    //Move Constructor
    OpenTrainer(OpenTrainer &&other);
    //Copy Assignment
    OpenTrainer& operator=(const OpenTrainer &other);
    //Move Assignment
    OpenTrainer& operator=(OpenTrainer &&other);
    //========================================
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
    private:
	const int trainerId;
	std::vector<Customer *> customers;
    void clear();
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
    const int trainerId;
};





class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    BaseAction* getAction();
};


#endif