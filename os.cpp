#include <iostream>
using namespace std;

#include <queue>
#include <vector>
#include "job.h"
#include "memorymanager.h"
#include "cpuScheduler.h"


void ontrace();
void offtrace();
void siodisk(long jobnum);
void siodrum(long jobnum, long jobsize, long coreaddress, long direction);
void bookKeeper(long currentTime);
void removeJob(long index);
void swapper(long  );
void dispatch(long & a ,long p[]);
void getIOJob();
long notInMem();
long findJob(vector<Job> jobsTable, long index);

//global variables

vector<Job> jobsTable;
long currRunningJob;
Memorymanager mem;
queue<long> ioQueue;
long currentIOJob;
CPUScheduler scheduler;
//long flag;
bool busy;
long inTransit[2];

void startup()
{
currRunningJob=-1;
//flag=1;
busy=false;
ontrace();
}

void Crint(long &a, long p[]){
    //bookkeep running job
    //-Weiyi Lin
    bookKeeper(p[5]);

    //create a job object with information passed in.
    //and save the job in the jobstable
    //cout<<p[2]<<endl;
    Job *newJob=new Job(p[1],p[2],p[3],p[4],p[5]);
    jobsTable.push_back(* newJob);



    swapper(notInMem());

    currRunningJob=scheduler.scheduler(jobsTable);

    dispatch(a,p);

}
//interrupt when job finish current IO job.
void Dskint(long &a, long p[]){
    //Huangshuo Cao
    bookKeeper(p[5]);

    //check the IO queue.
    //if(!ioQueue.empty()){
        getIOJob();

        jobsTable[currentIOJob].setAskForIO(false);
        //If it is blocked then remove it. And get the next IO job.

        //if(!jobsTable[currentIOJob].getAskForIO()){
        jobsTable[currentIOJob].setBlocked(false);
       // }
         if(jobsTable[currentIOJob].getKilled() && jobsTable[currentIOJob].getInMem() ){
            removeJob(currentIOJob);
            ioQueue.pop();

            getIOJob();
        }
        //Let the disk do IO.

        siodisk(jobsTable[currentIOJob].getJobNum());
        swapper(notInMem());
        currRunningJob=scheduler.scheduler(jobsTable);
        dispatch(a,p);


}
/*update memory if a job indicates that it is in memory
 otherwise,if it is killed or left to do IO
 remove job from jobsTable and memory
*/

void update(){
    //-Weiyi Lin
    for(int index=0;index<jobsTable.size();index++){
        if(jobsTable[index].getInMem())
            mem.setMemory(jobsTable,index);
        //
        if(jobsTable[index].getKilled() || jobsTable[index].getAskForIO()){
            mem.deleteJob(jobsTable,index);
            jobsTable.erase(jobsTable.begin()+index);
        }
    }

}

void Drmint(long & a, long p []){
    //-Weiyi Lin
    bookKeeper(p[5]);
    //the drum has finished swapping a job in or out of memory


    busy=false;

    long i=findJob(jobsTable,inTransit[0]);
    jobsTable[i].setInMem(true);


        //if a job indicates that it just got swapped,
        //update its status
    long jobNum=notInMem();
    if(jobNum!=-1 && !jobsTable[i].getInMem())
        swapper(jobNum);

    swapper(notInMem());
    currRunningJob=scheduler.scheduler(jobsTable);
    dispatch(a,p);

}


void Svc (long &a, long p[])
{//zhouyun Gu
bookKeeper(p[5]);
    if(a==5)//termination
        removeJob(currRunningJob);
    else if(a==6)//do IO
    {
        if((ioQueue.empty()))
            siodisk(jobsTable[currRunningJob].getJobNum());

        ioQueue.push(jobsTable[currRunningJob].getJobNum());
        jobsTable[currRunningJob].setAskForIO(true);
    }
    else if(a==7)//blocked
    {
        jobsTable[currRunningJob].setBlocked(true);
        //ioQueue.push(p[1]);
    }
    else
        cerr<<"a is not an valid number"<<endl;

    swapper(notInMem());
    currRunningJob=scheduler.scheduler(jobsTable);
    dispatch(a,p);
}
void Tro (long &a, long p[])
{//zhouyun Gu
bookKeeper(p[5]);
    if(jobsTable[currRunningJob].getAskForIO())
        removeJob(currRunningJob);
    else
        jobsTable[currRunningJob].setKilled(1);
    swapper(notInMem());
    currRunningJob=scheduler.scheduler(jobsTable);
    dispatch(a,p);

}



//dispatch

void dispatch(long &a, long p[]){
//-Weiyi Lin
    if(currRunningJob==-1 || !jobsTable[currRunningJob].getInMem() || jobsTable[currRunningJob].getBlocked() )
        a=1;
    else{

            a=2;
            p[2]=jobsTable[currRunningJob].getMemAddr();
            p[3]=jobsTable[currRunningJob].getJobSize();

            //we are using shortest job next algorithm,
            //so the time quantum for a job should be the maximum time that it is allowed to run.

            p[4]=jobsTable[currRunningJob].getMaxTime();
            //p[4]=jobsTable[currRunningJob].getTimeLeft();
            //select this job to run on cpu
            jobsTable[currRunningJob].setTimeEntered(p[5]);
            jobsTable[currRunningJob].setIsRunning(true);
    }

}
//remove job from memory
void removeJob(long index)
{//zhouyun Gu
    mem.deleteJob(jobsTable,index);
    jobsTable.erase(jobsTable.begin()+index);
}

/* bookkeeper--Weiyi
    -it is called every time an interrupt occurred.
    -if a job is running during the interrupt, update its maxTime
    -maxTime=maxTime-timeUsed
    -timeUsed=currentTime-timeEntered of running job.
    -if there is no job running,it does nothing.
*/
void bookKeeper(long currentTime){
//-Weiyi Lin
    //currRunningJob-static variable

    if(currRunningJob!=-1){
        long usedTime=currentTime-jobsTable[currRunningJob].getTimeEntered();
        jobsTable[currRunningJob].setTimeLeft( jobsTable[currRunningJob].getMaxTime()-usedTime);
    }

}


void swapper(long jobNum)
{
    //Huangshuo Cao
    if (!busy && jobNum!=-1)
    {
       // busy=true;
        // update memory
        update();

        // look for a job in memory that can fit in the table
        long index =findJob(jobsTable,jobNum);
        mem.memTable(jobsTable,index);
        long i=jobsTable[index].getMemAddr();

        //cout<<i;
        //if it can fit
        if(i<100 && i >=0)
        {

            // if the job was not already in memory, set it in
            if(jobsTable[index].getInMem()==0)
            {
                jobsTable[index].setMemAddr(i);
                siodrum(jobsTable[index].getJobNum(),jobsTable[index].getJobSize(),jobsTable[index].getMemAddr(),0);
                inTransit[0]=jobNum;
                inTransit[1]=0;
                mem.setMemory(jobsTable,index);
                //flag=1; // swapper in now busy
                busy=true;

            }
        }
    }
}



long notInMem(){
    //Huangshuo Cao
    long num=-1;
    long temp;

    for(long i=0; i<jobsTable.size(); i++){
        if(jobsTable[i].getInMem()==false){
            //if(memory.MemTable(i)          /*There is no Memtable function.
            if(jobsTable[i].getMaxTime()<temp){
                temp=jobsTable[i].getMaxTime();
                num=i;
            }
        }
    }
    return num;
}


/* This function gets the job from the I/O queue.
*/
void getIOJob(){
    //Huangshuo Cao
    //Check the I/O queue if it is empty.
    if(!ioQueue.empty()){
        for(long index=0;index<jobsTable.size();index++){
            if(ioQueue.front()==jobsTable[index].getJobNum())
                currentIOJob=index;
        }
        //Set the first IO queue job to cuurent IO job.
        //currentIOJob=ioQueue.front();
        //Remove the that job from I/O queue.
        //ioQueue.pop();
    }
}

    //check if a job is in memory or not
long findJob(vector<Job> jobsTable, long index)
{//zhouyun Gu
    for(long i=0;i<=jobsTable.size();i++)
        if(jobsTable[i].getJobNum()==index)
            return i;
}
