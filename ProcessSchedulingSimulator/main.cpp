#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <queue>
#include <ctime>

using namespace std;

//////////////////////////////////////////////////////////////
//event structure
struct event{
    float arrivalTime;
    float burst;
};
///////////////////////////////////////////////////////////////
struct compare_burst { //used to organize the ready queue for Algorithm 2
    bool operator()(event const& p1, event const& p2)
    {
        return p1.burst > p2.burst;
    }
};
////////////////////////////////////////////////////////////////
// function definition
void init(float &timeGlobal,float &waitTime, float &aat, float numProc, int &done);
int run_sim(queue<event>process, queue<event> ready,priority_queue<event, vector<event>, compare_burst> srtReady, int scheduler, float aat, float &timeGlobal, float roundRobin, float ast, int &done, float &waitTime);
void generate_report(int scheduler, float numProc, float roundRobin, float timeGlobal, float waitTime, int done, float aat);
float genexp(float);


////////////////////////////////////////////////////////////////
//initializes many variables that are used throughout the program, that will be
//updated as we go through it.
void init(float &timeGlobal,float &waitTime, float &aat, float numProc, int &done)
{
    timeGlobal = 0;
    waitTime = 0;
    aat = numProc;
    done = 0;
}
////////////////////////////////////////////////////////////////
//Function to generate the Performance Metrics and output them to a results.txt file
void generate_report(int scheduler, float numProc, float roundRobin, float timeGlobal, float waitTime, int done, float aat)
{
    FILE *results;
    results = fopen("results.txt", "a");

    float turnaround, throughput, utilization, averageQueue;
    waitTime /= done; //gets the average wait time
    turnaround = timeGlobal / done; //calculates the turnaround
    throughput = done / timeGlobal; //calculates the throughput
    utilization = 100 - ((timeGlobal - waitTime) / timeGlobal); //calculates the computer utilization
    averageQueue = numProc * waitTime; //calculates the average queue length

    if(results)
    { //outputs the metrics and identifying information to results.txt if the file opens
        cout << "Appending to file results.txt now" << endl;
        fprintf(results, "%i, %.0f, %.2f, %.5f, %.5f, %.5f, %.5f\n", scheduler, numProc, roundRobin, turnaround, throughput, utilization, averageQueue);
    } else
    { //error message in case the file doesn't open
        cout << "File " << results << " doesn't exist." << endl;
        cout << "Failed to write to file." << endl;
    }
    fclose(results); //closes the file pointer after.
}
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
    return( (float) rand()/RAND_MAX );
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp(float lambda)
{
    float u,x;
    x = 0;
    while (x == 0)
    {
        u = urand();
        x = (-1/lambda)*log(u);
    }
    return(x);
}
////////////////////////////////////////////////////////////
int run_sim(queue<event>process, queue<event> ready,priority_queue<event, vector<event>, compare_burst> srtReady, int scheduler, float aat, float &timeGlobal, float roundRobin, float ast, int &done, float &waitTime)
{ //initializes the needed data variables for the running simulator
    event tempProcess {0, 0}, tempReady {0, 0};
    float burst = 0.06;
    float timeProcess = 0;
    int change = 0; //used in Algorithm 3, for if a process is being pushed to the back

    switch ((int)scheduler)
    {
        case 1: //First Come First Served
            while(done < 10000) //so it goes until 10,000 processes terminate
            {
                //This loop pre-creates the processes putting them into the "new" state
                while(process.size() < 10) //keeps process queue with 10 processes
                {
                    tempProcess = {timeProcess, burst}; //makes process, defining arrival time, and burst
                    process.push(tempProcess); //adds process to process queue
                    timeProcess += genexp(aat); //increments the time process by the next arrival time
                }
                //This loop moves processes from the New state to the Ready State
                while(timeGlobal >= process.front().arrivalTime && !process.empty())
                { //moves all the processes from the ProcessQueue into the ready queue
                    tempProcess = {process.front().arrivalTime, process.front().burst};
                    process.pop();
                    ready.push(tempProcess);
                }
                if(ready.empty()) //has the global time jump to the next arriving process
                {                 //if the current ready queue is empty
                    timeGlobal = process.front().arrivalTime;
                }else {
                    //crunches all the data and collects information for the metrics
                    //moves processes from ready into running
                    waitTime += timeGlobal - ready.front().arrivalTime;
                    timeGlobal += ready.front().burst;
                    done++; //increments terminate variable by +1
                    ready.pop(); //terminates the process
                }
            }
            break;
        case 2: //Shortest Remaining Time (ie Lowest burst time)
        while(done < 10000) //so it goes until 10,000 processes terminate
        {
            //This loop pre-creates the processes putting them into the "new" state
            while(process.size() < 1000) //keeps process queue with 1300 processes
            {
                tempProcess = {timeProcess, burst}; //makes process, defining arrival time, and burst
                process.push(tempProcess); //adds process to process queue
                timeProcess += genexp(aat); //increments the Process time by arrival times
            }
            //This loop moves processes from the New state to the Ready State
            while(timeGlobal >= process.front().arrivalTime && !process.empty())
            {
                tempProcess = {process.front().arrivalTime, process.front().burst};
                process.pop();
                srtReady.push(tempProcess);
            }
            if(srtReady.empty())//has the global time jump to the next arriving process
            {                   //if the current ready queue is empty
                timeGlobal = process.front().arrivalTime;
            }
            else{
                //crunches all the data and collects information for the metrics
                //moves processes from ready into running
              waitTime += timeGlobal - srtReady.top().arrivalTime;
              timeGlobal += srtReady.top().burst;
              done++; //increments terminate variable by +1
              srtReady.pop(); //terminates the process
            }

        }
            break;
        case 3: //Round Robin
            while(done < 10000) //so it goes until 10,000 processes terminate
            {
                while(process.size() < 50) //keeps process queue with 50 processes
                {
                    tempProcess = {timeProcess, burst}; //makes process, defining arrival time, and burst
                    process.push(tempProcess); //adds process to process queue
                    timeProcess += genexp(aat);
                }
                if(!ready.empty())
                {
                    if(ready.front().burst > roundRobin)
                    {
                        change = 1; //showing that the current process is being modified and not terminated
                        timeGlobal += roundRobin; //increments the time by the round robin q value
                        tempReady = {ready.front().arrivalTime, ready.front().burst}; //sets a temp variable equal to the process to run
                    }else
                    {
                        waitTime += timeGlobal - ready.front().arrivalTime; //increments wait time by the time it arrived initially compared to when it finished.
                        timeGlobal += ready.front().burst; //increments burst by the time remaining on the process
                        done++; //increments how many done processes there are (that have terminated)
                        ready.pop(); //terminates the process
                    }
                }

                //Moves all the processes from the creation queue (process queue) into the ready queue where they are waiting to be processed.
                while(timeGlobal >= process.front().arrivalTime && !process.empty())
                {
                    tempProcess = {process.front().arrivalTime, process.front().burst};
                    process.pop();
                    ready.push(tempProcess);
                }
                if(change == 1) //moves process from the front to he back of the queue
                {
                    tempReady.burst -= roundRobin; //adjusts the running process burst time by removing the q value from it
                    ready.push(tempReady); //puts the process that just ran at the back of the queue
                    ready.pop(); //removes the process from the front of the queue
                    change = 0; //used to track if the process that is being processed needs to be modified or terminated
                }
                if(ready.empty())
                { //moves the global time variable to the next arriving process if ready queue is empty
                    timeGlobal = process.front().arrivalTime;
                }
            }
            break;

        default: //error checking the algorithm number from input.
            cout << "Invalid Algorithm option." << endl;
            cout << "Options are:" << endl;
            cout << "1 - First-Come-First-Serve" << endl;
            cout << "2 - Shortest Remaining Time First" << endl;
            cout << "3 - Round Robin" << endl;
            cout << "Try again" << endl;
            exit(1);
    }
    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    //seeds the random number generator by the clock so you get different results each compile
    srand(clock());

    // parse arguments
    float ast = 0, roundRobin = 0, numProc = 0;
    int scheduler = 0;
    scheduler = atoi(argv[1]);
    numProc = atof(argv[2]);
    ast = atof(argv[3]);
    roundRobin = atof(argv[4]);

    // Making the queues
    queue <event> ready;
    queue <event> process;
    priority_queue<event, vector<event>, compare_burst> srtReady;

    //Creating variables
    float time = 0;
    float wait = 0;
    int counter = 0;
    float arrivalTime = numProc;
    float &aat = arrivalTime, &timeGlobal = time, &waitTime = wait;
    int &done = counter;

    //calls all the functions to run the simulator
    init(timeGlobal, waitTime, aat, numProc, done);
    run_sim(process, ready, srtReady, scheduler, aat, timeGlobal, roundRobin, ast, done, waitTime);
    generate_report(scheduler, numProc, roundRobin, timeGlobal, waitTime, done, aat); //also pass process num, algorithm number, round robin q value
    return 0;
}
