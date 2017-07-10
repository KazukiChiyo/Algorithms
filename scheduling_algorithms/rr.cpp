/* This program first implements Round-robin (RR) algorithm with a simple job scheduling scheme: Each process is assigned a quantum and is interrupted if the process is not completed by then. A modified version, called Dynamic Time Quantum Round-robin, dynamically adjusts quantum and order of execution (by SRF) once a new process arrives.
   @Author: Kexuan(Klaus) Zou.
   @Date: 06/01/17 */

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>
using namespace std;

/* individual jobs are represented by this struct with labels represented by the pid based on a discrete-time implementation */
struct job{
    int pid, arrival, burst, wait, remaining;
    bool arrived;
    job(int p, int a, int b):pid(p), arrival(a), burst(b), wait(0), remaining(b), arrived(false){}
};

/* pair structure pushed onto readyQueue */
struct jobPair {
    int pid, remaining;
    jobPair(int p, int r):pid(p), remaining(r){}
};

bool byPid(const job& lhs, const job& rhs) {
    return lhs.pid < rhs.pid;
}

bool byPriority(const jobPair& lhs, const jobPair& rhs) {
    return lhs.remaining < rhs.remaining;
}

bool isFinished(const job target) {
    return target.remaining == 0 ? true : false;
}

/* update all jobs in the sequence and the planner within the quantum */
void updateRr(vector<job>& target, list<int>& readyQueue, vector<int>& schedule, int& count, int& cycle, int quantum) {
    while (quantum != 0) {
        for (int i = 0; i < target.size(); i++) {
            if (target[i].remaining != 0 && target[i].arrival == cycle && target[i].arrived == false) {
                target[i].arrived = true;
                readyQueue.push_back(target[i].pid); // push pid onto the ready queue
            }
        }
        target[readyQueue.front()].remaining--;
        schedule.push_back(readyQueue.front()); // update schedule
        for (list<int>::iterator it = std::next(readyQueue.begin()); it != readyQueue.end() && readyQueue.size() >= 1; it++) // update waiting time for idle processes
            target[*it].wait++;
        cycle++;
        if (isFinished(target[readyQueue.front()])) { // if process is finished before the assigned quantum
            readyQueue.pop_front();
            count--;
            return;
        }
        quantum--;
    }
    // put unfinished process back onto the queue
    int head = readyQueue.front();
    readyQueue.pop_front();
    readyQueue.push_back(head);
}

/* run rr on the input vector */
void rr(vector<job>& target, vector<int>& schedule, int& count, int& cycle, int quantum) {
    std::list<int> readyQueue;
    while (count != 0) {
        updateRr(target, readyQueue, schedule, count, cycle, quantum);
    }
}

/* calculate the avg burst time by the processes on the ready queue */
int getAvgBurst(list<jobPair> readyQueue) {
    float sum = 0;
    float n = readyQueue.size();
    for (list<jobPair>::iterator it = readyQueue.begin(); it != readyQueue.end(); it++)
        sum += it->remaining;
    return (int) round(sum/n);
}

/* add job arrived at the current cycle and return true if new item arrived */
void addNew(vector<job>& target, list<jobPair>& readyQueue, int cycle) {
    for (int i = 0; i < target.size(); i++) {
        if (target[i].remaining != 0 && target[i].arrival == cycle && target[i].arrived == false) {
            target[i].arrived = true;
            readyQueue.push_back(jobPair(target[i].pid, target[i].burst)); // push jobPair onto ready queue
        }
    }
}

/* run DTQRR on the input vector */
void dtqrr(vector<job>& target, vector<int>& schedule, int& count, int& cycle) {
    std::list<jobPair> readyQueue;
    addNew(target, readyQueue, cycle); // update at cycle 0
    int ctr = 0;
    while (count != 0) { // iterates every quantum
        bool done = false;
        int quantum = getAvgBurst(readyQueue);
        readyQueue.sort(byPriority);
        jobPair head = readyQueue.front();
        while (quantum != 0) { // iterates every time cycle
            schedule.push_back(head.pid); // update schedule
            // update waiting time for idle processes
            for (list<jobPair>::iterator it = std::next(readyQueue.begin()); it != readyQueue.end() && readyQueue.size() >= 1; it++)
                target[it->pid].wait++;
            quantum--;
            ctr++;
            addNew(target, readyQueue, cycle+ctr);
            if (isFinished(target[head.pid])) { // if process is finished before the assigned quantum
                readyQueue.pop_front();
                count--;
                done = true;
                break;
            }
        }
        // put unfinished process back onto the queue
        if (done == false) {
            jobPair first = readyQueue.front();
            readyQueue.pop_front();
            readyQueue.push_back(first);
        }
        cycle += ctr;
        ctr = 0; // reset counter
    }
}

/* prints the schedule */
void printSchedule(vector<job> target, vector<int> schedule) {
    cout << "Total runtime is " << schedule.size() << endl;
    cout << "Schedule: ";
    for (int i = 0; i < schedule.size(); i++)
        cout << schedule[i] << " ";
    cout << endl;
    cout << "Job list with waiting time:" << endl;
    sort(target.begin(), target.end(), byPid);
    int sum = 0;
    for (int i = 0; i < target.size(); i++) {
        sum += target[i].wait;
        cout << target[i].pid << " " << target[i].arrival << " " << target[i].burst << " (" << target[i].wait << ")" << endl;
    }
    cout << "Average waiting time: " << (float) sum / (float) target.size() << endl;
}

int main(void) {
    std::vector<job> list;
    list.push_back(job(0, 0, 1));
    list.push_back(job(1, 0, 2));
    list.push_back(job(2, 0, 3));
    list.push_back(job(3, 0, 4));
    list.push_back(job(4, 0, 5));
    list.push_back(job(5, 0, 6));
    list.push_back(job(6, 16, 1));
    list.push_back(job(7, 16, 2));
    list.push_back(job(8, 16, 3));
    list.push_back(job(9, 16, 4));
    list.push_back(job(10, 27, 1));
    list.push_back(job(11, 27, 2));
    list.push_back(job(12, 27, 3));
    int count = list.size();
    std::vector<int> schedule;
    int cycle = 0;
    int quantum = 3;
    /* comment and uncomment the following two lines to run rr and dtqrr */
    //rr(list, schedule, count, cycle, quantum);
    dtqrr(list, schedule, count, cycle);
    printSchedule(list, schedule);
}