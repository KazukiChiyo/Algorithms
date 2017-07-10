/* This program implements shortest remaining time first (SRTF) algorithm with a simple job scheduling scheme: the process with the shortest time remaining until completion is selected to execute first.
   @Author: Kexuan(Klaus) Zou
   @Date: 05/25/17 */

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/* individual jobs are represented by this struct with labels represented by the pid */
struct job{
    int pid, arrival, burst, wait, remaining;
    bool active; // 1 if active, 0 if inactive
    job(int p, int a, int b):pid(p), arrival(a), burst(b), wait(0), remaining(b), active(false){}
};

bool byRemaining(const job& lhs, const job& rhs) {
    return lhs.remaining < rhs.remaining;
}

bool byPid(const job& lhs, const job& rhs) {
    return lhs.pid < rhs.pid;
}

bool isArrived(int arrival, int cycle) {
    return arrival <= cycle ? true : false;
}

/* update all jobs in the sequence and the planner within one time cycle */
void updateAll(vector<job>& target, int& count, int cycle) {
    for (int i = 0; i < target.size(); i++) {
        if (target[i].active == true) { // if job is currently under execution
            target[i].remaining--;
            target[i].active = false;
            if (target[i].remaining == 0)
                count--;
        }
        else {
            if (target[i].remaining != 0 && isArrived(target[i].arrival, cycle)) // if job is waiting to be done
                target[i].wait++;
        }
    }
}

/* run srtf on the input vector based on a discrete-time implementation */
void srtf(vector<job>& target, vector<int>& schedule, int& count, int& cycle) {
    while (count != 0) {
        sort(target.begin(), target.end(), byRemaining); // sort by srtf
        for (int i = 0; i < target.size(); i++) {
            if (target[i].remaining != 0 && isArrived(target[i].arrival, cycle)) {
                target[i].active = true; // toggle active
                schedule.push_back(target[i].pid);
                break;
            }
        }
        updateAll(target, count, cycle);
        cycle++;
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
    srtf(list, schedule, count, cycle);
    printSchedule(list, schedule);
}