#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here


// Add your implementation of schedule() and other helper functions here
bool isValid(const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, const AvailabilityMatrix& avail) {
    if (sched.size() != avail.size()) { //Ensure that enough days have been counted
        return false;
    }
    size_t numWorkers = avail[0].size();
    std::vector<int> numShifts; //Counts how many times a worker has been worked (as to not exceed limit)
    for (size_t i = 0; i < numWorkers; i++) {
        numShifts.push_back(0);
    }
    for (size_t day = 0; day < sched.size(); day++) {
        if (sched[day].size() != dailyNeed) { //Ensure that each day has enough for the day
            return false;
        }
        std::set<int> avoidDuplicates;
        for (size_t i = 0; i < sched[day].size(); i++) {
            int workerAssigned = sched[day][i];
            if (avail[day][workerAssigned] == 0) { //Worker availability check
                return false;
            }
            if (avoidDuplicates.find(workerAssigned) != avoidDuplicates.end()) { //Check for accidentally putting worker on the same day
                return false;
            }
            avoidDuplicates.insert(workerAssigned);
            numShifts[workerAssigned] = numShifts[workerAssigned] + 1;
            if (numShifts[workerAssigned] > maxShifts) { //Ensure no worker exceeds their max shifts
                return false;
            }
        }
    }
    return true;
}
bool backtrack(size_t day, size_t space, const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts, DailySchedule& sched, size_t numDays, size_t numWorkers, std::vector<int>& totalShifts) {
    if (day == numDays) {
        return true;
    }
    if (space == dailyNeed) {
        return backtrack(day + 1, 0, avail, dailyNeed, maxShifts, sched, numDays, numWorkers, totalShifts); //Move forward a day
    }
    for (size_t worker = 0; worker < numWorkers; worker++) {
        Worker_T currWorker = static_cast<Worker_T>(worker);
        if (avail[day][currWorker] == 0) { //Checks for availability
            continue;
        }
        auto it = std::find(sched[day].begin(), sched[day].end(), currWorker); //Ensures no duplicates
        if (it != sched[day].end()) {
            continue;
        }
        if (totalShifts[currWorker] >= static_cast<int>(maxShifts)) { //Ensures that the worker doesn't exceed their shift limit
            continue;
        }
        sched[day][space] = currWorker;
        totalShifts[currWorker]++;
        if (backtrack(day, space + 1, avail, dailyNeed, maxShifts, sched, numDays, numWorkers, totalShifts) == true) {
            return true;
        }

        //Backtracking variables
        sched[day][space] = -1;
        totalShifts[currWorker]--;
    }
    return false;
}
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if (avail.size() == 0U) {
        return false;
    }
    sched.clear();
    // Add your code below
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();
    for (size_t day = 0; day < numDays; day++) { //Initializes sched with an unassigned value (-1)
        std::vector<Worker_T> row;
        for (size_t i = 0; i < dailyNeed; i++) {
            row.push_back(-1);
        }
        sched.push_back(row);
    }
    std::vector<int> totalShifts;
    for (int i = 0; i < numWorkers; i++) {
        totalShifts.push_back(0);
    }
    return backtrack(0, 0, avail, dailyNeed, maxShifts, sched, numDays, numWorkers, totalShifts);
}

