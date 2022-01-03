#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class process{
    public:
        // 0 = terminated, 1 = ready, 2 = running, 3 = blocked
        int currentStatus, nextStatus, processNumber, readyCycles;
        int R1, B1, R2, B2;

        process(int r1, int b1, int r2, int b2, int pN){
            processNumber = pN;
            currentStatus = 1;
            nextStatus = 1;
            readyCycles = 0;
            R1 = r1;
            B1 = b1;
            R2 = r2;
            B2 = b2;
        }

        int getStatus();

        void run();
        void block();
        int terminate();
};

int process::getStatus(){
    return currentStatus;
}

// function to run process
void process::run(){
    // if R1 is not 0 yet, decrement R1
    if(R1 > 0){
        R1-=1;

        // if R1 is now 0, set the next state to block
        if(R1 == 0){
            currentStatus = nextStatus;
            nextStatus = 3;
        }

        // otherwise, set the next state to running
        else{
            currentStatus = nextStatus;
            nextStatus = 2;
        }
    }

    // otherwise, if R2 is not 0 yet, decrement R2
    else if(R2 > 0){
        R2-=1;

        // if R2 is now 0, set the next state to block
        if(R2 == 0){
            currentStatus = nextStatus;
            nextStatus = 3;
        }

        // otherwise, set the next state to running
        else{
            currentStatus = nextStatus;
            nextStatus = 2;
        }
    }
}

// function to block process
void process::block(){

    // if B1 is not 0 yet, decrement B1
    if(B1 > 0){
        B1-=1;

        // if B1 is now 0, set the next state to ready
        if(B1 == 0){
            currentStatus = nextStatus;
            nextStatus = 1;
        }

        // otherwise, set the next state to blocking again 
        else{
            currentStatus = nextStatus;
            nextStatus = 3;
        }
    }

    // if B2 is not 0 yet, decrement B2
    else if(B2 > 0){
        B2-=1;

        // if B2 is now 0, set the next state to terminate
        if(B2 == 0){
            currentStatus = nextStatus;   
            nextStatus = 0;       
        }

        // otherwise, set the next state to blocking again 
        else{
            currentStatus = nextStatus;
            nextStatus = 3;
        }
    }
}

// function to terminate process
int process::terminate(){
    currentStatus = 0;
    return 0;
}

int fifo(vector<process>, int);
int rr(vector<process>, int, int);

int main(int argc, char *argv[]){

    // reading command line arguments
    int numP = argv[0][1];
    int SA = argv[0][2];
    int len = argc;

    // converting args to int
    vector<int> args;
    for(int i = 1; i < len; i++){
        stringstream argument(argv[i]);
        int a;
        argument >> a;
        args.push_back(a);
    }

    vector<process> processList;
    
    cout << "Cycle" << "     ";

    // Running FIFO
    if(args[1] == 1){
        int j = 2;
        // creating all process objects with command line arguments 
        for(int i = 0; i < args[0]; i++){
            cout << "P" << i << "              ";
            process process1 = process(args[j], args[j+1], args[j+2], args[j+3], i);
            processList.push_back(process1);
            j+=4;
        }
        cout << "\n";

        // run round robin function
        fifo(processList, args[0]);
    }

    // Running Round Robin
    else if(args[1] == 2){
        int j = 3;
        // creating all process objects with command line arguments 
        for(int i = 0; i < args[0]; i++){
            cout << "P" << i << "              ";
            process process1 = process(args[j], args[j+1], args[j+2], args[j+3], i);
            processList.push_back(process1);
            j+=4;
        }
        cout << "\n";

        // run round robin function
        rr(processList, args[0], args[2]);
    }

    else{
        cout << "Please enter 1 or 2 for FIFO or RR scheduling.";
    }

    return 1;
}

// FIFO function
int fifo(vector<process> processList, int nProcesses){

    queue<int> readyQueue;

    // preparing ready queue with all processes
    for(int n = 0; n < nProcesses; n++){
        readyQueue.push(n);
    }

    vector<int> blockedList;

    int cycle = 1;

    // initializing process states to be printed later
    int printedStates[nProcesses];
    for (int k = 0; k < nProcesses; k++){
        printedStates[k] = -1;
    }

    // while there are remaining ready or blocked processes 
    while(!blockedList.empty() || !readyQueue.empty()){
        int setStates[nProcesses];

        //updating current status
        for(int j = 0; j < processList.size(); j++){
            processList[j].currentStatus = processList[j].nextStatus;
            setStates[j] = 0;
        }
            
       
        if(!readyQueue.empty()){
            
            // if process is ready to run, run process
            if(processList[readyQueue.front()].currentStatus == 1 || (processList[readyQueue.front()].currentStatus == 2 && processList[readyQueue.front()].nextStatus == 2)){
                processList[readyQueue.front()].run();
                printedStates[processList[readyQueue.front()].processNumber] = 2;
                setStates[processList[readyQueue.front()].processNumber] = 1;

                // if the process is getting blocked next cycle, remove from ready queue and place in blocked list
                if(processList[readyQueue.front()].nextStatus == 3){
                    int last = processList[readyQueue.front()].processNumber;
                    readyQueue.pop();
                    blockedList.push_back(last);
                    
                }
            }

            // if process is ready to terminate, terminate process
            else if(processList[readyQueue.front()].currentStatus == 0){
                printedStates[processList[readyQueue.front()].processNumber] = 0;
                setStates[processList[readyQueue.front()].processNumber] = 1;
                processList[readyQueue.front()].terminate();
                readyQueue.pop();
            }
        }

        int size = blockedList.size();
        vector<int> tempBlocked = blockedList;

        // iterate through all blocked processes
        for (int n = 0; n < size; n++){
            // block process
            if(processList[tempBlocked[n]].currentStatus == 3){
                processList[tempBlocked[n]].block();
                printedStates[processList[tempBlocked[n]].processNumber] = 3;
                setStates[processList[tempBlocked[n]].processNumber] = 1;
            }   

            // if after blocking, process will be ready to run next cycle, remove from blocked list and place in ready queue
            if(processList[tempBlocked[n]].nextStatus == 1){
                readyQueue.push(processList[tempBlocked[n]].processNumber);
                int removeN = processList[tempBlocked[n]].processNumber;
                blockedList.erase(remove(blockedList.begin(), blockedList.end(), removeN), blockedList.end());
            }

            // if after blocking, process will be ready to terminate next cycle, remove from blocked list and place in ready queue
            else if(processList[tempBlocked[n]].nextStatus == 0){
                readyQueue.push(processList[tempBlocked[n]].processNumber);
                int removeN = processList[tempBlocked[n]].processNumber;
                blockedList.erase(remove(blockedList.begin(), blockedList.end(), removeN), blockedList.end());
            }
        }

        // printing out process states
        cout << cycle << "\t";
        for (int k = 0; k < nProcesses; k++){
            // if process state was not changed in this cycle and it is not a previously terminated process
            // set state to 1 = ready
            if(setStates[k] == 0 && printedStates[k]!=5){
                printedStates[k] = 1;
            }

            if(printedStates[k] == 2){
                cout << "Running         ";
            }
            else if(printedStates[k] == 0){
                cout << "Terminating     ";
                printedStates[k] = 5;
            }
            else if(printedStates[k] == 3){
                cout << "Blocked         ";
            }
            else if(printedStates[k] == 5){
                cout << "                ";
            }
            else if(printedStates[k] == 1){
                cout << "Ready           ";
            }

            if(k==nProcesses-1){
                cout << "\n";
            }
        }
        cycle++;
    }
    return 0;
}

// round robin function
int rr(vector<process> processList, int nProcesses, int qInput){

    vector<int> readyQueue;

    // preparing ready queue with all processes
    for(int n = 0; n < nProcesses; n++){
        readyQueue.push_back(n);
    }

    vector<int> blockedList;

    // initializing variables
    int cycle = 1;
    int previousProcess = -2;
    int q = -1;

    // initializing process states to be printed later
    int printedStates[nProcesses];
    for (int k = 0; k < nProcesses; k++){
        printedStates[k] = -1;
    }


    // while there are remaining ready or blocked processes 
    while(!blockedList.empty() || !readyQueue.empty()){

        // setStates is an array that stores wether the state of a process was updated in the current cycle or not
        int setStates[nProcesses];
        
    

        // update the number of cycles a process has been ready in a row
        for(int j = 0; j < processList.size(); j++){
            // initialize all to 0 because no process states have been set in this cycle yet
            setStates[j] = 0;

            // if the process was previously ready and is currently ready, increment the ready cycles
            if(processList[j].nextStatus == 1 && processList[j].currentStatus == 1){
                processList[j].readyCycles++;
            }

            // if the process is newly ready, set ready cycles to 1 
            else if (processList[j].nextStatus == 1 || processList[j].nextStatus == 2){
                processList[j].readyCycles = 1;
            }

            // if process is blocked or terminated, set ready cycles to 0
            else {
                processList[j].readyCycles = 0;
            }

            // update the current status
            processList[j].currentStatus = processList[j].nextStatus;
        }
       
        if(!readyQueue.empty()){
            
            // if the current front process in the ready queue is the same as the previous running process, increment q
            if(previousProcess == processList[readyQueue.front()].processNumber){
                q++;
            }

            // otherwise reset q back to 0
            else{
                q = 0;
            }

            // if there is more than 1 process in the ready queue, rearrange processes in correct order
            if(readyQueue.size() > 1){
                // if q has reached its limit
                if(q==qInput){
                    // removing process from front of the ready queue and placing it in the back
                    readyQueue.push_back(processList[readyQueue.front()].processNumber);
                    processList[readyQueue.front()].nextStatus = 1;
                    processList[readyQueue.front()].currentStatus = 1;
                    readyQueue.erase(readyQueue.begin());

                    // resetting q back to 0
                    q = 0;

                    // tie-breaking rule
                    int temp1 = -1;
                    int temp2 = -1;
                    for(int j = 0; j < readyQueue.size()-1; j++){

                        // if two processes have been ready for the same amount of cycles
                        if(processList[readyQueue[j]].readyCycles == processList[readyQueue[j+1]].readyCycles){

                            // if the second process has a smaller process number, it takes priority
                            if(processList[readyQueue[j+1]].processNumber < processList[readyQueue[j]].processNumber){

                                // swapping processes in ready queue
                                temp1 = processList[readyQueue[j]].processNumber;
                                temp2 = processList[readyQueue[j+1]].processNumber;
                                readyQueue[j] = temp2;
                                readyQueue[j+1] = temp1;

                            }
                        }
                    }
                }
            }
        
            // if process is ready to run, run process
            if(processList[readyQueue.front()].currentStatus == 1 || processList[readyQueue.front()].currentStatus == 2){
                processList[readyQueue.front()].run();
                printedStates[processList[readyQueue.front()].processNumber] = 2;
                setStates[processList[readyQueue.front()].processNumber] = 1;
                previousProcess = processList[readyQueue.front()].processNumber;

                // if the process is getting blocked next cycle, remove from ready queue and place in blocked list
                if(processList[readyQueue.front()].nextStatus == 3){
                    int last = processList[readyQueue.front()].processNumber;
                    readyQueue.erase(readyQueue.begin());
                    blockedList.push_back(last);
                    
                }
            }

            // if process is ready to terminate, terminate process
            if(processList[readyQueue.front()].currentStatus == 0){
                printedStates[processList[readyQueue.front()].processNumber] = 0;
                setStates[processList[readyQueue.front()].processNumber] = 1;
                processList[readyQueue.front()].terminate();
                previousProcess = processList[readyQueue.front()].processNumber;
                readyQueue.erase(readyQueue.begin());
            }
        }

        int size = blockedList.size();
        vector<int> tempBlocked = blockedList;

        // iterate through all blocked processes
        for (int n = 0; n < size; n++){

            // block process
            if(processList[tempBlocked[n]].currentStatus == 3){
                processList[tempBlocked[n]].block();
                printedStates[processList[tempBlocked[n]].processNumber] = 3;
                setStates[processList[tempBlocked[n]].processNumber] = 1;
            }   

            // if after blocking, process will be ready to run next cycle, remove from blocked list and place in ready queue
            if(processList[tempBlocked[n]].nextStatus == 1){
                readyQueue.push_back(processList[tempBlocked[n]].processNumber);
                int removeN = processList[tempBlocked[n]].processNumber;
                blockedList.erase(remove(blockedList.begin(), blockedList.end(), removeN), blockedList.end());
            }

            // if after blocking, process will be ready to terminate next cycle, remove from blocked list and place in ready queue
            else if(processList[tempBlocked[n]].nextStatus == 0){
                readyQueue.push_back(processList[tempBlocked[n]].processNumber);
                int removeN = processList[tempBlocked[n]].processNumber;
                blockedList.erase(remove(blockedList.begin(), blockedList.end(), removeN), blockedList.end());
            }
        }

        // printing out process states
        cout << cycle << "\t";
        for (int k = 0; k < nProcesses; k++){
            // if process state was not changed in this cycle and it is not a previously terminated process
            // set state to 1 = ready
            if(setStates[k] == 0 && printedStates[k]!=5){
                printedStates[k] = 1;
            }

            if(printedStates[k] == 2){
                cout << "Running         ";
            }
            else if(printedStates[k] == 0){
                cout << "Terminating     ";
                printedStates[k] = 5;
            }
            else if(printedStates[k] == 3){
                cout << "Blocked         ";
            }
            else if(printedStates[k] == 5){
                cout << "                ";
            }
            else if(printedStates[k] == 1){
                cout << "Ready           ";
            }

            if(k==nProcesses-1){
                cout << "\n";
            }
        }

        cycle++;
    }

    return 0;
}
