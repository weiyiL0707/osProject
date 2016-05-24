#ifndef JOB_H
#define JOB_H

using namespace std;

//Job class-storing information

class Job{
    private:

    long jobNum;
    long jobSize;
    long priority;
    long maxTime;
    long timeEntered;
    long timeLeft;
    long memAddr;
    bool inMem;
    bool doingIO;
    bool askForIO;
    bool isRunning;
    bool killed;
    bool blocked;
    bool swapping;


    //public no-argument constructor that initializes private variables above.
    public:
    Job(){
        jobNum=-1;
        jobSize=0;
        priority=-1;
        maxTime=-1;
        timeEntered=-1;
        timeLeft=-1;

        memAddr=-1;
        inMem=false;
        doingIO=false;
        askForIO=false;
        isRunning=false;
        killed=false;
        blocked=false;
        swapping=false;
    }

    //public constructor that sets known information of a job that is entered.

    Job(long jobNum,long priority, long jobSize, long maxCpu, long currentTime){

        this->jobNum=jobNum;
        this->jobSize=jobSize;
        this->priority=priority;
        this->maxTime=maxCpu;
        this->timeEntered=currentTime;

        timeLeft=maxCpu;
        memAddr=-1;
        inMem=false;
        doingIO=false;
        askForIO=false;
        isRunning=false;
        killed=false;
        blocked=false;
        swapping=false;
    }


    //accessors and mutators for all private member variables.
    void setJobNum(long jobNum){
        this->jobNum=jobNum;
    }
    long getJobNum(){
        return jobNum;
    }


    void setJobSize(long jobSize){
        this->jobSize=jobSize;
    }
    long getJobSize(){
        return jobSize;
    }


    void setPriority(long priority){
        this->priority=priority;
    }
    long getPriority(){
        return priority;
    }


    void setMaxTime(long maxTime){
        this->maxTime=maxTime;
    }
    long getMaxTime(){
        return maxTime;
    }


    void setTimeEntered(long timeEntered){
        this->timeEntered=timeEntered;
    }
    long getTimeEntered(){
        return timeEntered;
    }

    void setTimeLeft(long timeLeft){
        this->timeLeft=timeLeft;
    }

    long getTimeLeft(){
        return timeLeft;
    }

    void setMemAddr(long memAddr){
        this->memAddr=memAddr;
    }
    long getMemAddr(){
        return memAddr;
    }



    void setInMem(bool inMem){
        this->inMem=inMem;
    }
    bool getInMem(){
        return inMem;
    }


    void setDoingIO(bool doingIO){
        this->doingIO=doingIO;
    }
    bool getDoingIO(){
        return doingIO;
    }


    void setAskForIO(bool askForIO){
        this->askForIO=askForIO;
    }
    bool getAskForIO(){
        return askForIO;
    }


    void setIsRunning(bool isRunning){
        this->isRunning=isRunning;
    }
    bool getIsRunning(){
        return isRunning;
    }


    void setKilled(bool killed){
        this->killed=killed;
    }
    bool getKilled(){
        return killed;
    }



    void setBlocked(bool blocked){
        this->blocked=blocked;
    }
    bool getBlocked(){
        return blocked;
    }



    void setSwapping(bool swapping){
        this->swapping=swapping;
    }
    bool getSwapping(){
        return swapping;
    }

};
#endif
