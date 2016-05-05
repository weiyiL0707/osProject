/* This class is to set up the cpu scheduler. We are using Shortest job next algorithm.
*/
#ifndef CPU_SECHEDULER
#define CPU_SECHEDULER

 // CPU_SECHEDULER
class CPUScheduler{
   private:
        long temp;
        long time=100000;
public:
    //return index
    Job scheduler(vector<Job> jobsTable){
        for(long i=0;i<jobsTable.size();i++){
            if(jobsTable[i].getInMem()&&jobsTable[i].getMaxTime()>0){
                if(jobsTable[i].getMaxTime()<time){
                    temp=i;
                    time=jobsTable[i].getMaxTime();
                }
            }
        }
        return jobsTable[temp];
    }
};
#endif
