#ifndef memorymanager_h
#define memorymanager_h

#include <iostream>
using namespace std;

//data number is used based on 1k words = 1kb
class Memorymanager{
   // private:
//zhouyun Gu
    public:
    long memory[100]; //memory table
     //constructor
    Memorymanager(){
        for(int i=0;i<100;i++)
            memory[i]=0;
    }

    //put a job into a place where is fit in memory table
    void setMemory(vector<Job> &jobsTable,long index){
        if(jobsTable[index].getMemAddr()!=-1){
            for(int i=0;i<jobsTable[index].getJobSize();i++)
                memory[jobsTable[index].getMemAddr()+i]=jobsTable[index].getJobNum();

            jobsTable[index].setInMem(true);
        }
    }
    //delete a job in memory
    void deleteJob(vector<Job> jobsTable,long index)
    {
        for(long i=0;i<jobsTable[index].getJobSize();i++)
            memory[jobsTable[index].getMemAddr()+i]=0;
    }


    void memTable(vector<Job> &jobsTable,long index)
    {
        long i;
        bool can;
        while(i<100)
        {
            can=true;
            while(memory[i]!=0)
                i++;

            for(long j=0;j<jobsTable[index].getJobSize();j++)
            {
                if(memory[i+j]!=0)
                    can=false;
            }
            if(can){
                jobsTable[index].setMemAddr(i);
                return;
            }
            i++;
        }
        jobsTable[index].setMemAddr(-1);
    }
};
#endif // memorymanager_h
