#ifndef memorymanager_h
#define memorymanager_h

#include <iostream>
using namespace std;

//data number is used based on 1k words = 1kb
class Memorymanager{
    private:
        long memory[100]; //memory table
    public:
     //constructor
    Memorymanager(){
        for(int i=0;i<100;i++)
            memory[i]=0;
    }
    //clean whole memory
    void clearmemory(){
        for(int i=0;i<100;i++)
            memory[i]=0;
    }
    //put a job into a place where is fit in memory table
    void setmemory(vector<Job> jobsTable,long index){
        long i=0;
        bool can=true;
        while(i<100)
        {
            if(memory[i]!=0)
                i++;
            for(long j=0;j<jobsTable[index].getJobSize();j++)//check if needed space is occupied or not
            {
                if(memory[j]!=0)
                    can=false;
            }
            if(can)//if there is a space, put job number into memory space
            {
                jobsTable[index].setMemAddr(i);
                for(long j=0;j<jobsTable[index].getJobSize();j++)
                    memory[i]=jobsTable[index].getJobNum();
                break;
            }
        }
    }
    //delete a job in memory
    void deletejob(vector<Job> jobsTable,long index)
    {
        for(long i=0;i<jobsTable[index].getJobSize();i++)
        memory[jobsTable[index].getMemAddr()]=0;
    }
    //check if a job is in memory or not
    long findjob(vector<Job> jobsTable, long index)
    {
        if(memory[jobsTable[index].getMemAddr()]==jobsTable[index].getJobNum())
            return true;
        return false;
    }

};
#endif // memorymanager_h
