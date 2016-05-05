#include <iostream>
using namespace std;

#include <queue>
#include <vector>
#include "Job.h"
#include "memorymanager.h"
#include "cpuScheduler.h"


void ontrace();
void offtrace();
void siodisk(long jobnum);
void siodrum(long jobnum, long jobsize, long coreaddress, long direction);
void bookKeeper(long currentTime);
void removejob(long index);
void swapper(vector<Job> );
void dispatch(long & a ,long p[]);
void getIOJob();

//global variables

vector<Job> jobsTable;
long currRunningJob;
Memorymanager mem;
queue<long> ioQueue;
long currentIOJob;
CPUScheduler scheduler;
bool swappingStatus;

void startup()
{
currRunningJob=-1;
mem=Memorymanager();
swappingStatus=false;//not doing swapping

}

void Crint(long &a, long p[]){
    //bookkeep running job
    bookKeeper(p[5]);

    //create a job object with information passed in.
    //and save the job in the jobstable
    Job *newJob=new Job(p[1],p[2],p[3],p[4],p[5]);
    jobsTable.push_back(* newJob);

    swapper(jobsTable);

        currRunningJob=scheduler(jobsTable).getJobNum();

    dispatch(a,p);

}
/*update memory if a job indicates that it is in memory
 otherwise,if it is killed or left to do IO
 remove job from jobsTable and memory
*/

void update(){
    for(int index=0;index<jobsTable.size();index++){
        if(jobsTable[index].getInMem())
            mem.setmemory(jobsTable,index);
        //
        if(jobsTable[index].getKilled() || jobsTable[index].getAskForIO()){
            mem.deletejob(jobsTable,index);
            jobsTable.erase(jobsTable.begin()+index);
        }
    }

}

void drmint(long & a, long p []){
    bookKeeper(p[5]);
    //the drum has finished swapping a job in or out of memory

    swappingStatus=false;

    for(int index=0;index<jobsTable.size();index++){
        //if a job indicates that it just got swapped,
        //update its status
        if(jobsTable[index].getSwapping()){
            jobsTable[index].setSwapping(false);
            jobsTable[index].setInMem(true);
        }

    }
    currRunningJob=scheduler(jobsTable);
    dispatch(a,p);

}



//dispatch

void dispatch(long &a, long p[]){

    if(currRunningJob==-1 || !jobsTable[currRunningJob].getInMem() || jobsTable[currRunningJob].getBlocked() )
        a=1;
    else{
            p[2]=jobsTable[currRunningJob].getMemAddr();
            p[2]=jobsTable[currRunningJob].getJobSize();

            //we are using shortest job next algorithm,
            //so the time quantum for a job should be the maximum time that it is allowed to run.

            p[4]=jobsTable[currRunningJob].getMaxTime();

            //select this job to run on cpu
            jobsTable[currRunningJob].setIsRunning(true);
    }

}

/*remove job from memory
void removejob(long index)
{
    memory.deletejob(jobsList[index].getlocation());
    jobsList.erase(jobsList.begin()+index);
}
*/
/* bookkeeper--Weiyi
    -it is called every time an interrupt occurred.
    -if a job is running during the interrupt, update its maxTime
    -maxTime=maxTime-timeUsed
    -timeUsed=currentTime-timeEntered of running job.
    -if there is no job running,it does nothing.
*/
void bookKeeper(long currentTime){

    //currRunningJob-static variable

    if(currRunningJob!=-1){
        long usedTime=currentTime-jobsTable[currRunningJob].getTimeEntered();
        jobsTable[currRunningJob].setMaxTime( jobsTable[currRunningJob].getMaxTime()-usedTime);
    }

}


/* This function will move the jobs between memory and drum.
*/

//ng flag=1;
//parameter--currrunningjob
void swapper(vector<Job> jobsTable){
    //before swapping any jobs, first update the jobTbale.
    update();
    //check the flag and get the information from the job.
    if(flag==1&&!job.p[1]=-1){
        flag--;
        //if job is not in memory, check free space and set to that location.
        if(!job.p[1].inMen){
            if(memory.findJob(jobTable, job.p[1])==true){
                siodrum(job.p[1], memory.setMemory(jobTable, job.p[1]), job.p[3], 0);
            }
        }
        flag++;
    }
};


//interrupt when job finish current IO job.
void Dskint(long &a, int p[]){
    bookKeeper(p[5]);

    //check the IO queue.
    if(!ioQueue.empty()){
        getIOJob();

        //If it is blocked then remove it. And get the next IO job.
        if(jobsTable[currentIOJob].blocked()==true&&jobsTable[currentIOJob].inMem()==true){
        removejob(jobsTable, currRunningJob);
        ioQueue.pop();
        getIOJob();
        }
        //Let the disk do IO.
        siodisk(jobTable[currentIOJob].getJobNum());
    }
}

/* This function gets the job from the I/O queue.
*/
void getIOJob(){
    //Check the I/O queue if it is empty.
    if(ioQueue.empty()&&currentIOJob!=ioQueue.front()){
        //Set the first IO queue job to cuurent IO job.
        currentIOJob=ioQueue.front();
        //Remove the that job from I/O queue.
        ioQueue.pop();
    }
}
