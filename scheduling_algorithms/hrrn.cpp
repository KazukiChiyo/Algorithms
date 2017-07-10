/* This program implements highest response ratio next (HRRN) algorithm with a simple job scheduling scheme: the process with the highest priority is selected to execute first. Priority is defined as priority = (wait time + burst time) / burst time.
   @Author: Kexuan(Klaus) Zou
   @Date: 05/26/17 */

#define idle -1
#define active 0
#define finished 1

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/* individual jobs are represented by this struct with labels represented by the pid */
struct job{
    int pid, arrival, burst, wait;
    float priority;
    int status; //idle, active, finished
    job(int p, int a, int b):pid(p), arrival(a), burst(b), wait(0), priority(1), status(idle){}
};

bool byPriority(const job& lhs, const job& rhs) {
    return (lhs.priority != rhs.priority) ? lhs.priority > rhs.priority : lhs.burst < rhs.burst;
}

bool byPid(const job& lhs, const job& rhs) {
    return lhs.pid < rhs.pid;
}

bool isArrived(int arrival, int cycle) {
    return arrival <= cycle ? true : false;
}

void updatePriority(vector<job>& target) {
    for (int i = 0; i < target.size(); i++)
        target[i].priority = (float) (target[i].wait+target[i].burst) / (float) target[i].burst;
}

/* update all jobs in the sequence and the planner within one time cycle */
void updateAll(vector<job>& target, int& count, int& cycle) {
    for (int i = 0; i < target.size(); i++) {
        if (target[i].status == idle && isArrived(target[i].arrival, cycle)) // if job is waiting to be done
            target[i].wait++;
    }
}

/* run hrrn on the input vector */
void hrrn(vector<job>& target, vector<int>& schedule, int& count, int& cycle) {
    while (count != 0) {
        updatePriority(target);
        sort(target.begin(), target.end(), byPriority); // sort by hrrn
        int step = 0;
        int idx = 0;
        for (idx = 0; idx < target.size(); idx++) {
            if (target[idx].status == idle && isArrived(target[idx].arrival, cycle)) {
                target[idx].status = active; // toggle active
                schedule.push_back(target[idx].pid);
                step = target[idx].burst; // add burst time to the clock
                break;
            }
        }
        for (int i = 0; i < step; i++, cycle++)
            updateAll(target, count, cycle);
        target[idx].status = finished;
        count--;
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
    hrrn(list, schedule, count, cycle);
    printSchedule(list, schedule, cycle);
}