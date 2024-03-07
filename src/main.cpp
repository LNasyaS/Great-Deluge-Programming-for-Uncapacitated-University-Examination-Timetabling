// THESIS LNASYA SYAFITRIE 2022
// GREAT DELUGE ALGORITHM FOR UNCAPACITATED UNIVERSITY EXAMINATION TIMETABLING

#include <iostream> // input output stream
#include <cstdlib> // general utilities
#include <fstream> // file Stream
#include <vector> // store data
#include <set>
#include <iterator>
#include <algorithm>   
#include <random>
#include <map> 
#include <time.h>

using namespace std; 

/************************************************************** helper functions **************************************************************/
// For comparing 2nd column order
bool sortcol(const vector<int>& v1,const vector<int>& v2) { 
    return v1[1] > v2[1]; 
} 

// function to find max element of a vector
int maxElement(vector<int> vector) {
    int max = *max_element(vector.begin(), vector.end());
    return max;
}

// function to find if there are duplicates in a vector
bool noDuplicates(vector<int> a, vector<int> b) {
    a.insert(a.end(), b.begin(), b.end());
    sort(a.begin(), a.end());
    auto it = unique(a.begin(), a.end());
    bool wasUnique = (it == a.end());

    return wasUnique;
}

// sort by second element of pairs
bool sortbysec(const pair<int,int> &a,const pair<int,int> &b) {
    return (a.second > b.second);
}

// function to count duplicate elements
// modified by komar
int countDuplicates(vector<int> a, vector<int> b) {
    // a.insert(a.end(), b.begin(), b.end());
    // int allElements = a.size();
    // sort(a.begin(), a.end());
    // //count the same
    //a.erase(unique(a.begin(), a.end()), a.end());
    // //int duplicateElements = allElements - a.size();

    //return duplicateElements;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    int indexA = 0;
    int indexB = 0;
    int duplicateCount = 0;
    while(indexA < a.size() && indexB < b.size()){
        if(a[indexA] == b[indexB]){
            duplicateCount++;
            if(indexA < a.size())
                indexA++;
            else
                indexB++;
        }else if(a[indexA] < b[indexB]){
            indexA++;
        }else if(a[indexA] > b[indexB]){
            indexB++;
        }
    }
    return duplicateCount;
}

// count conflicts
int countConflicts(vector<int> solution, vector<vector<int>> conflictMatrix) {
    int conflicts = 0;
    for (int exam = 0; exam < solution.size(); exam++) {
        for (int exam2 = exam + 1; exam2 < solution.size(); exam2++) {
            if (solution[exam] == solution[exam2] && conflictMatrix[exam][exam2] > 0) {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// count conflicts 2
// added by komar
int countConflicts2(vector<int> solution, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {
    int conflicts = 0;
    for (int exam = 0; exam < solution.size(); exam++) {
        for(int index = 0; index < conflictSet[exam].size(); index++){
            if(exam < conflictSet[exam][index]){
                if(solution[exam] == solution[conflictSet[exam][index]] ){
                conflicts++;
                }
            }
        }
    }
    return conflicts;
}

int countConflictsKempe(set<int> exams, vector<vector<int>> conflictMatrix) {
    int conflicts = 0;
    for (auto exam = begin(exams); exam != end(exams); ++exam) {
        for (auto exam2 = exam; exam2 != end(exams); ++exam2) {
            if (conflictMatrix[*exam][*exam2] != 0)
                conflicts++;
        }
    }
    return conflicts;
}

// list of all exams in conflict
vector<int> inConflict(vector<int> solution, vector<vector<int>> conflictMatrix) {
    vector<int> conflictedExams;
    for (int exam = 0; exam < solution.size(); exam++) {
        for (int exam2 = exam + 1; exam2 < solution.size(); exam2++) {
            if (solution[exam] == solution[exam2] && conflictMatrix[exam][exam2] > 0) {
                conflictedExams.push_back(exam);
                goto nextExam;
            }
        }
        nextExam:;
    }
    return conflictedExams;
}

// list periods with no conflict
vector<int> noConflict(int exam, int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix) {
    vector<int> safePeriods;
    for (int period = 0; period < noOfPeriods; period++) {
        for(int exam2 = 0; exam2 < solution.size(); exam2++) {
            if(period == solution[exam2] && conflictMatrix[exam][exam2] != 0) {
                goto nextPeriod;
            }
        }
        safePeriods.push_back(period);
        nextPeriod:;
    }
    return safePeriods;
}

// list of exams in conflict with exam e
vector<int> unsafeExams(int exam, vector<int> solution, vector<vector<int>> conflictMatrix) {
    vector<int> unsafe;
    for(int exam2 = 0; exam2 < solution.size(); exam2++) {
        if (solution[exam] != solution[exam2] && conflictMatrix[exam][exam2] > 0) {
            unsafe.push_back(exam2);
        }
    }
    return unsafe;
}

// calculate penalty
int calculatePenalty(vector<int> solution, vector<vector<int>> conflictMatrix) {
    int penaltyWeight[] = {0,16,8,4,2,1};
    int penalty = 0;
    for (int exam = 0; exam < solution.size(); exam++) {
        for (int exam2 = exam + 1; exam2 < solution.size(); exam2++) {
            int distance = abs(solution[exam2] - solution[exam]);
            if (distance <= 5) {
                penalty += conflictMatrix[exam][exam2] * penaltyWeight[distance];
            }
        }
    }
    return penalty;
}

// calculate partial penalty
int calculatePenaltyPartial(vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> movedExams) {
    int penaltyWeight[] = {0,16,8,4,2,1};
    int penalty = 0;
//     for (int exam = 0; exam < movedExams.size(); exam++) {
//         for (int exam2 = 0; exam2 < solution.size(); exam2++) {
//             int distance = abs(solution[exam2] - solution[movedExams[exam]]);
//             if (distance <= 5) {
//                 penalty += conflictMatrix[movedExams[exam]][exam2] * penaltyWeight[distance];
//             }
//         }
//     }
    for (auto exam = begin(movedExams); exam != end(movedExams); ++exam) {
        for (int exam2 = 0; exam2 < solution.size(); exam2++) {
            int distance = abs(solution[exam2] - solution[*exam]);
            if (distance <= 5) {
                penalty += conflictMatrix[*exam][exam2] * penaltyWeight[distance];
            }
        }
    }
    return penalty;
}

// calculate new penalty using partial penalty
int calculateNewPenalty(vector<int> solution, vector<int> newSolution, vector<vector<int>> conflictMatrix, vector<int> exams, int penalty) {
    int penaltyDiff = calculatePenaltyPartial(solution, conflictMatrix, exams) - calculatePenaltyPartial(newSolution, conflictMatrix, exams);
    penalty -= penaltyDiff;
    return penalty;
}

// list exams with distace cost
vector<int> examWithDistance(vector<int> solution, vector<vector<int>> conflictMatrix) {
    vector<int> withDistance;
    for (int exam = 0; exam < solution.size(); exam++) {
        for (int exam2 = exam + 1; exam2 < solution.size(); exam2++) {
            int distance = abs(solution[exam2] - solution[exam]);
            if (distance <= 5 && distance != 0) {
               withDistance.push_back(exam);
               goto nextExam;
            }    
        }
        nextExam:;
    }
    return withDistance;
}

/*********************************************************** local search operators ***********************************************************/

// move exam
vector<int> moveExam(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix) {
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randAllExam(0, solution.size() - 1);
    int e = randAllExam(rng);

    vector<int> periodIsSafe = noConflict(e, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe.size() - 1); 
    int p = periodIsSafe[randPeriod(rng)];
    
    solution.at(e) = p;
    return solution;
}

vector<int> moveExamPartial(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randAllExam(0, solution.size() - 1);
    int e = randAllExam(rng);
    
    // Inserts all affected exams
    movedExams.push_back(e);
    // copy(conflictSet[e].begin(), conflictSet[e].end(), inserter(movedExams, movedExams.end()));

    vector<int> periodIsSafe = noConflict(e, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe.size() - 1); 
    int p = periodIsSafe[randPeriod(rng)];
    
    solution.at(e) = p;
    return solution;
}

// swap exam
vector<int> swapExams(vector<int> solution, vector<vector<int>> conflictMatrix) {
    mt19937 rng(time(NULL));
    
    vector<int> examConflicts = inConflict(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, examConflicts.size()-1);
    int e1 = examConflicts[randExam(rng)];

    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    uniform_int_distribution<int> randExam2(0, sameStudents.size()-1);
    int e2 = sameStudents[randExam2(rng)];
    // int e2 = examConflicts[randExam(rng)];
    
    int temp = solution[e1];
    solution.at(e1) = solution[e2];
    solution.at(e2) = temp;
    return solution;
}

vector<int> swapExamsPartial(vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));
    
    vector<int> examConflicts = inConflict(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, examConflicts.size()-1);
    int e1 = examConflicts[randExam(rng)];
    movedExams.push_back(e1);

    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    uniform_int_distribution<int> randExam2(0, sameStudents.size()-1);
    int e2 = sameStudents[randExam2(rng)];
    movedExams.push_back(e2);
    // int e2 = examConflicts[randExam(rng)];
    
    int temp = solution[e1];
    solution.at(e1) = solution[e2];
    solution.at(e2) = temp;
    return solution;
}

// kick exam
vector<int> kickExam(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix) {
    mt19937 rng(time(NULL));
    
    vector<int> withDistance = examWithDistance(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, withDistance.size()-1);
    int e1 = withDistance[randExam(rng)]; 

    vector<int> periodIsSafe = noConflict(e1, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    vector<int> safeSame;
    for(auto it = begin(sameStudents); it != end(sameStudents); ++it) {
        for(auto it2 = begin(periodIsSafe); it2 != end(periodIsSafe); ++it2) {
            if(solution[*it] == *it2) {
                safeSame.push_back(*it);
                goto nextExam;
            }
        }
        nextExam:;
    }
    if(safeSame.size() < 1) {
        return solution;
    }
    uniform_int_distribution<int> randExam2(0, safeSame.size()-1);
    int e2 = safeSame[randExam2(rng)];

    vector<int> periodIsSafe2 = noConflict(e2, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe2.size() - 1); 
    int p2 = periodIsSafe2[randPeriod(rng)];

    solution.at(e1) = solution[e2];
    solution.at(e2) = p2;
    return solution;
}

vector<int> kickExamPartial(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));
    
    vector<int> withDistance = examWithDistance(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, withDistance.size()-1);
    int e1 = withDistance[randExam(rng)];
    movedExams.push_back(e1);

    vector<int> periodIsSafe = noConflict(e1, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    vector<int> safeSame;
    for(auto it = begin(sameStudents); it != end(sameStudents); ++it) {
        for(auto it2 = begin(periodIsSafe); it2 != end(periodIsSafe); ++it2) {
            if(solution[*it] == *it2) {
                safeSame.push_back(*it);
                goto nextExam;
            }
        }
        nextExam:;
    }
    if(safeSame.size() < 1) {
        movedExams.clear();
        return solution;
    }
    uniform_int_distribution<int> randExam2(0, safeSame.size()-1);
    int e2 = safeSame[randExam2(rng)];
    movedExams.push_back(e2);

    vector<int> periodIsSafe2 = noConflict(e2, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe2.size() - 1); 
    int p2 = periodIsSafe2[randPeriod(rng)];

    solution.at(e1) = solution[e2];
    solution.at(e2) = p2;
    return solution;
}

// double kick exam
vector<int> doubleKickExam(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix) {
    mt19937 rng(time(NULL));
    
    vector<int> withDistance = examWithDistance(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, withDistance.size()-1);
    int e1 = withDistance[randExam(rng)]; 

    vector<int> periodIsSafe = noConflict(e1, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    vector<int> safeSame;
    for(auto it = begin(sameStudents); it != end(sameStudents); ++it) {
        for(auto it2 = begin(periodIsSafe); it2 != end(periodIsSafe); ++it2) {
            if(solution[*it] == *it2) {
                safeSame.push_back(*it);
                goto nextExam;
            }
        }
        nextExam:;
    }
    if(safeSame.size() < 1) {
        return solution;
    }
    uniform_int_distribution<int> randExam2(0, safeSame.size()-1);
    int e2 = safeSame[randExam2(rng)];

    vector<int> periodIsSafe2 = noConflict(e2, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents2 = unsafeExams(e2, solution, conflictMatrix);
    vector<int> safeSame2;
    for(auto it = begin(sameStudents2); it != end(sameStudents2); ++it) {
        for(auto it2 = begin(periodIsSafe2); it2 != end(periodIsSafe2); ++it2) {
            if(solution[*it] == *it2) {
                safeSame2.push_back(*it);
                goto nextExam2;
            }
        }
        nextExam2:;
    }
    if(safeSame2.size() < 1) {
        return solution;
    }
    uniform_int_distribution<int> randExam3(0, safeSame2.size()-1);
    int e3 = safeSame2[randExam3(rng)];


    vector<int> periodIsSafe3 = noConflict(e3, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe3.size() - 1); 
    int p = periodIsSafe3[randPeriod(rng)];

    solution.at(e1) = solution[e2];
    solution.at(e2) = solution[e3];
    solution.at(e3) = p;
    return solution;
}

vector<int> doubleKickExamPartial(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));
    
    vector<int> withDistance = examWithDistance(solution, conflictMatrix);
    uniform_int_distribution<int> randExam(0, withDistance.size()-1);
    int e1 = withDistance[randExam(rng)];
    movedExams.push_back(e1);

    vector<int> periodIsSafe = noConflict(e1, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents = unsafeExams(e1, solution, conflictMatrix);
    vector<int> safeSame;
    for(auto it = begin(sameStudents); it != end(sameStudents); ++it) {
        for(auto it2 = begin(periodIsSafe); it2 != end(periodIsSafe); ++it2) {
            if(solution[*it] == *it2) {
                safeSame.push_back(*it);
                goto nextExam;
            }
        }
        nextExam:;
    }
    if(safeSame.size() < 1) {
        movedExams.clear();
        return solution;
    }
    uniform_int_distribution<int> randExam2(0, safeSame.size()-1);
    int e2 = safeSame[randExam2(rng)];
    movedExams.push_back(e2);

    vector<int> periodIsSafe2 = noConflict(e2, noOfPeriods, solution, conflictMatrix);
    vector<int> sameStudents2 = unsafeExams(e2, solution, conflictMatrix);
    vector<int> safeSame2;
    for(auto it = begin(sameStudents2); it != end(sameStudents2); ++it) {
        for(auto it2 = begin(periodIsSafe2); it2 != end(periodIsSafe2); ++it2) {
            if(solution[*it] == *it2) {
                safeSame2.push_back(*it);
                goto nextExam2;
            }
        }
        nextExam2:;
    }
    if(safeSame2.size() < 1) {
        movedExams.clear();
        return solution;
    }
    uniform_int_distribution<int> randExam3(0, safeSame2.size()-1);
    int e3 = safeSame2[randExam3(rng)];
    movedExams.push_back(e3);


    vector<int> periodIsSafe3 = noConflict(e3, noOfPeriods, solution, conflictMatrix);
    uniform_int_distribution<int> randPeriod(0, periodIsSafe3.size() - 1); 
    int p = periodIsSafe3[randPeriod(rng)];

    solution.at(e1) = solution[e2];
    solution.at(e2) = solution[e3];
    solution.at(e3) = p;
    return solution;
}

// kempe chain
vector<int> kempeChain(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix) {
    mt19937 rng(time(NULL));

    // Choosing e and p1
    uniform_int_distribution<int> randExam(0, solution.size()-1);
    int e = randExam(rng);
    int p1 = solution[e];

    // Choosing p2
    vector<int> period2;
    for (int i = 0; i < noOfPeriods; i++) {
        if (i != p1)
            period2.push_back(i);
    }
    uniform_int_distribution<int> randPeriod(0, period2.size()-1);
    int p2 = period2[randPeriod(rng)];

    set<int> p1Conflicts;
    set<int> p2Conflicts;

    bool firstIter = 1;
    startChain:
    
    // Finding conflicts in p2
    for (auto exam = begin(p1Conflicts); exam != end(p1Conflicts); ++exam) {
        for (int exam2 = 0; exam2 < solution.size(); exam2++) {
            if (solution[exam2] == p2 && conflictMatrix[*exam][exam2])
                p2Conflicts.insert(exam2);
        }
    }

    p1Conflicts.clear();
    if (firstIter) {
        p1Conflicts.insert(e);
        firstIter = 0;
    }

    // Finding conflicts in p1
    for (auto exam = begin(p2Conflicts); exam != end(p2Conflicts); ++exam) {
        for (int exam2 = 0; exam2 < solution.size(); exam2++) {
            if (solution[exam2] == p1 && conflictMatrix[*exam][exam2])
                p1Conflicts.insert(exam2);
        }
    }

    // Swapping p1 and p2
    for (auto exam = begin(p1Conflicts); exam != end(p1Conflicts); ++exam) {
        solution.at(*exam) = p2;
    }
    for (auto exam = begin(p2Conflicts); exam != end(p2Conflicts); ++exam) {
        solution.at(*exam) = p1;
    }

    if (countConflicts(solution, conflictMatrix) > 0) {
        p2Conflicts.clear();
        goto startChain;
    }

    return solution;
}

vector<int> kempeChainPartial(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));

    // Choosing e and p1
    uniform_int_distribution<int> randExam(0, solution.size()-1);
    int e = randExam(rng);
    int p1 = solution[e];

    // Choosing p2
    vector<int> period2;
    for (int i = 0; i < noOfPeriods; i++) {
        if (i != p1)
            period2.push_back(i);
    }
    uniform_int_distribution<int> randPeriod(0, period2.size()-1);
    int p2 = period2[randPeriod(rng)];

    set<int> p1Conflicts;
    set<int> p2Conflicts;

    bool firstIter = 1;
    startChain:
    
    // Finding conflicts in p2
    for (auto exam = begin(p1Conflicts); exam != end(p1Conflicts); ++exam) {
        for (int exam2 = 0; exam2 < solution.size(); exam2++) {
            if (solution[exam2] == p2 && conflictMatrix[*exam][exam2])
                p2Conflicts.insert(exam2);
        }
    }

    p1Conflicts.clear();
    if (firstIter) {
        p1Conflicts.insert(e);
        firstIter = 0;
    }

    // Finding conflicts in p1
    for (auto exam = begin(p2Conflicts); exam != end(p2Conflicts); ++exam) {
        for (int exam2 = 0; exam2 < solution.size(); exam2++) {
            if (solution[exam2] == p1 && conflictMatrix[*exam][exam2])
                p1Conflicts.insert(exam2);
        }
    }

    // Swapping p1 and p2
    for (auto exam = begin(p1Conflicts); exam != end(p1Conflicts); ++exam) {
        solution.at(*exam) = p2;
        movedExams.push_back(*exam);
    }
    for (auto exam = begin(p2Conflicts); exam != end(p2Conflicts); ++exam) {
        solution.at(*exam) = p1;
        movedExams.push_back(*exam);
    }

    if (countConflicts(solution, conflictMatrix) > 0) {
        p2Conflicts.clear();
        goto startChain;
    }

    return solution;
}

// kempe chain
vector<int> kempeChainPartial2(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, vector<int> &movedExams) {
    mt19937 rng(time(NULL));

    // Choosing e and p1
    uniform_int_distribution<int> randExam(0, solution.size()-1);
    int e = randExam(rng);
    int p1 = solution[e];

    // Choosing p2
    vector<int> period2;
    for (int i = 0; i < noOfPeriods; i++) {
        if (i != p1)
            period2.push_back(i);
    }
    uniform_int_distribution<int> randPeriod(0, period2.size()-1);
    int p2 = period2[randPeriod(rng)];

    // Creating sets for exams in p1 and p2
    set<int> p1Exams;
    set<int> p2Exams;
    for (int exam = 0; exam < solution.size(); exam ++) {
        if (solution[exam] == p1) {
            p1Exams.insert(exam);
            movedExams.push_back(exam);
        } else if (solution[exam] == p2) {
            p2Exams.insert(exam);
            movedExams.push_back(exam);
        }
    }

    set<int> p1ExamsMoved = {e};
    set<int> p2ExamsMoved;

    startChain:
    for (auto exam = begin(p1ExamsMoved); exam != end(p1ExamsMoved); ++exam) {
        solution.at(*exam) = p2;
        for(auto exam2 = begin(p2Exams); exam2 != end(p2Exams); ++exam2) {
            if (conflictMatrix[*exam][*exam2] != 0) {
                p2ExamsMoved.insert(*exam2);
            }     
        }
        p1Exams.erase(*exam);
        p2Exams.insert(*exam);
    }
    p1ExamsMoved.clear();

    for (auto exam = begin(p2ExamsMoved); exam != end(p2ExamsMoved); ++exam) {
        solution.at(*exam) = p1;
        for(auto exam2 = begin(p1Exams); exam2 != end(p1Exams); ++exam2) {
            if (conflictMatrix[*exam][*exam2] != 0) {
                p1ExamsMoved.insert(*exam2);
            }     
        }
        p2Exams.erase(*exam);
        p1Exams.insert(*exam);
    }
    p2ExamsMoved.clear();

    if (countConflictsKempe(p1Exams, conflictMatrix) + countConflictsKempe(p2Exams, conflictMatrix) > 0) {
        goto startChain;
    }
 
    return solution;
}

// swap periods
vector<int> swapPeriods(int noOfPeriods, vector<int> solution) {
    mt19937 rng(time(NULL));

    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);
    int p1 = randPeriod(rng);

    vector<int> period2;
    for (int i = 0; i < noOfPeriods; i++) {
        if (i != p1)
            period2.push_back(i);
    }
    uniform_int_distribution<int> randPeriod2(0, period2.size()-1);
    int p2 = period2[randPeriod2(rng)];

    // Assign exams in p1 to temp period
    int temp = noOfPeriods;
    for (int exam = 0; exam < solution.size(); exam++) {
        if (solution[exam] == p1)
            solution.at(exam) = temp;
    }
    // Assign exams in p2 to p1
    for (int exam = 0; exam < solution.size(); exam++) {
        if (solution[exam] == p2)
            solution.at(exam) = p1;
    }    
    // Assign exams in temp period to p2
    for (int exam = 0; exam < solution.size(); exam++) {
        if (solution[exam] == temp)
            solution.at(exam) = p2;
    }

    return solution;
}

vector<int> swapPeriodsPartial(int noOfPeriods, vector<int> solution, vector<int> &movedExams) {
    mt19937 rng(time(NULL));

    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);
    int p1 = randPeriod(rng);

    vector<int> period2;
    for (int i = 0; i < noOfPeriods; i++) {
        if (i != p1)
            period2.push_back(i);
    }
    uniform_int_distribution<int> randPeriod2(0, period2.size()-1);
    int p2 = period2[randPeriod2(rng)];

    // Assign exams in p2 to p1
    for (int exam = 0; exam < solution.size(); exam++) {
        if (solution[exam] == p2) {
            movedExams.push_back(exam);
            solution.at(exam) = p1;
        } else if (solution[exam] == p1) {
            movedExams.push_back(exam);
            solution.at(exam) = p2;
        }
    }


    return solution;
}

/*********************************************************** solution algorithms ***********************************************************/

// initialize solution
vector<int> initialSolution(vector<vector<int>> orderingSelection, int noOfPeriods, vector<vector<int>> conflictMatrix) {
    vector<int> solution;
    for (int exam = 0; exam < orderingSelection.size(); exam++) {
        solution.push_back(-1);
    }

    for (int exam = 0; exam < orderingSelection.size(); exam++) {
        // int sortedExam = studentsInExam[exam][0] - 1;
        int sortedExam = orderingSelection[exam][0];
        // cout << sortedExam << ": ";
        // finding safe periods
        for (int period = 0; period < noOfPeriods; period++) {
            bool isSafe = 1;
            for (int exam2 = 0; exam2 < orderingSelection.size(); exam2++) {
                if (conflictMatrix[sortedExam][exam2] > 0 && period == solution[exam2]) {
                    isSafe = 0;
                }
            }
            if (isSafe) {
                solution.at(sortedExam) = period;
                goto nextExam;
            }
        }
        solution.at(sortedExam) = 0;
        nextExam:;
    }
    return solution;
}

// initialize solution pair
vector<int> initialSolutionPair(vector<pair<int,int>> orderingSelection, int noOfPeriods, vector<vector<int>> conflictMatrix) {
    vector<int> solution;
    for (int exam = 0; exam < orderingSelection.size(); exam++) {
        solution.push_back(-1);
    }

    for (int exam = 0; exam < orderingSelection.size(); exam++) {
        // int sortedExam = studentsInExam[exam][0] - 1;
        int sortedExam = orderingSelection[exam].first;
        // cout << sortedExam << ": ";
        // finding safe periods
        for (int period = 0; period < noOfPeriods; period++) {
            bool isSafe = 1;
            for (int exam2 = 0; exam2 < orderingSelection.size(); exam2++) {
                if (conflictMatrix[sortedExam][exam2] > 0 && period == solution[exam2]) {
                    isSafe = 0;
                }
            }
            if (isSafe) {
                solution.at(sortedExam) = period;
                goto nextExam;
            }
        }
        solution.at(sortedExam) = 0;
        nextExam:;
    }
    return solution;
}

// initialize solution (Saturation Degree)
vector<int> initialSolutionSD(int noOfExams, int noOfPeriods, vector<vector<int>> conflictMatrix) {
    vector<int> solution;
    vector<vector<int>> saturationDegree;
    for (int exam = 0; exam < noOfExams; exam++) {
        solution.push_back(-1);
        saturationDegree.push_back(vector<int>());
        saturationDegree[exam].push_back(exam);
        saturationDegree[exam].push_back(0);
    }

    // Initialize saturation degree
    nextExam:
    for (int exam = 0; exam < saturationDegree.size(); exam++) {
        saturationDegree[exam].at(1) = 0;
        for (int period = 0; period < noOfPeriods; period++) {
            bool isSafe = 1;
            for (int exam2 = 0; exam2 < noOfExams; exam2++) {
                if (conflictMatrix[saturationDegree[exam][0]][exam2] > 0 && period == solution[exam2]) {
                    isSafe = 0;
                }
            }
            saturationDegree[exam].at(1) += isSafe;
        }
    }
    sort(saturationDegree.begin(), saturationDegree.end(),sortcol);

    for (int period = 0; period < noOfPeriods; period++) {
        bool isSafe = 1;
        for (int exam2 = 0; exam2 < noOfExams; exam2++) {
            if (conflictMatrix[saturationDegree.back()[0]][exam2] > 0 && period == solution[exam2]) {
                isSafe = 0;
            }
        }
        if (isSafe) {
            solution.at(saturationDegree.back()[0]) = period;
            // cout << "Exam " << saturationDegree.back()[1] << ": " << solution.at(saturationDegree.back()[1]) << endl;
            goto checkDone;
        }
        // cout << "Exam " << saturationDegree.back()[1] << ": " << solution.at(saturationDegree.back()[1]) << endl;
    }
    solution.at(saturationDegree.back()[0]) = 0;
    
    checkDone:
    saturationDegree.pop_back();
    if (saturationDegree.size() > 0) {
        goto nextExam;
    }

    return solution;
}

// reduce conflicts using move exam
// added by komar
vector<int> ReduceConflictsUsingMoveExam(vector<int> solution3, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution3;

    // check the exams that are still have conflicts
    set<int> examWithConflicts;
    vector<int> examList;
    for(int i = 0; i < solution3.size(); i++)
        examList.push_back(i);
    // shuffle the sequence
    std::random_shuffle(examList.begin(), examList.end());
    int exam;
    for(int i = 0; i < solution2.size(); i++){
        exam = examList[i];
        for(int index = 0; index < conflictSet[exam].size(); index++){
            if(exam < conflictSet[exam][index]){
                if(solution2[exam] == solution2[conflictSet[exam][index]]){
                    examWithConflicts.insert(exam);
                }
            }
        }
    }

    // try to change the Period
    int nextPeriod = 0;
    bool foundConflict;
    int conflictBefore, conflictAfter = 0;
    set<int>::iterator itrExamWithConflicts;    
    for(itrExamWithConflicts = examWithConflicts.begin(); itrExamWithConflicts != examWithConflicts.end(); itrExamWithConflicts++){

        for(int index3 = 0; index3 < noOfPeriods; index3++){
            nextPeriod++;
            if(nextPeriod >= noOfPeriods){
                nextPeriod = 0;
            }
            conflictBefore = 0;
            conflictAfter = 0;
            for(int index2 = 0; index2 < conflictSet[*itrExamWithConflicts].size(); index2++){
                if(solution2[conflictSet[*itrExamWithConflicts][index2]] == solution2[*itrExamWithConflicts])
                    conflictBefore++;
                if(solution2[conflictSet[*itrExamWithConflicts][index2]] == nextPeriod)
                    conflictAfter++;
            }
            if(conflictAfter <= conflictBefore){
                solution2[*itrExamWithConflicts] = nextPeriod;
                break;
            }
        }
    }

    //int cost = countConflicts2(solution, conflictMatrix, conflictSet);
    //cout << "conflicts: " << cost << endl;
    return solution2;
}

// reduce conflicts using swap exam
// added by komar
vector<int> ReduceConflictsUsingSwapExam(vector<int> solution3, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution3;

    // check the exams that are still have conflicts
    bool hasConflict;
    int periodTemp1, periodTemp2;
    int conflictBefore1, conflictAfter1 = 0;
    int conflictBefore2, conflictAfter2 = 0;
    bool succeeded;
    // check the exams that are still have conflicts
    set<int> examWithConflicts;
    vector<int> examList;
    for(int i = 0; i < solution3.size(); i++)
        examList.push_back(i);
    // shuffle the sequence
    std::random_shuffle(examList.begin(), examList.end());
    int exam;
    for(int i = 0; i < solution2.size(); i++){
        exam = examList[i];
    //for(int exam = 0; exam < solution2.size(); exam++){
        hasConflict = false;
        for(int index = 0; index < conflictSet[exam].size(); index++){
            //if(exam < conflictSet[exam][index]){
                if(solution2[exam] == solution2[conflictSet[exam][index]]){
                    //this exam still has conflict, try to swap with the one that different with itrExamWithConflicts
                    hasConflict = true;
                    break;
                }
            //}
        }

        if(hasConflict){
            for(int index = 0; index < conflictSet[exam].size(); index++){
                if(solution2[exam] != solution2[conflictSet[exam][index]]){
                    // these two exams have two different period. 
                    // checked if they are being swapped, are the result reduce the conflict
                    conflictBefore1 = 0;
                    conflictAfter1 = 0;
                    conflictBefore2 = 0;
                    conflictAfter2 = 0;
                    int exam2 = conflictSet[exam][index];
                    periodTemp1 = solution2[exam];
                    periodTemp2 = solution2[exam2];                    
                    for(int checked1 = 0; checked1 < conflictSet[exam].size(); checked1++){
                        if(solution2[conflictSet[exam][checked1]] == periodTemp1)
                            conflictBefore1++;
                        if(conflictSet[exam][checked1] != exam2 && solution2[conflictSet[exam][checked1]] == periodTemp2)
                            conflictAfter1++;
                    }
                    for(int checked1 = 0; checked1 < conflictSet[exam2].size(); checked1++){
                        if(solution2[conflictSet[exam2][checked1]] == periodTemp2)
                            conflictBefore2++;
                        if(conflictSet[exam2][checked1] != exam && solution2[conflictSet[exam2][checked1]] == periodTemp1)
                            conflictAfter2++;
                    }
                    //if it reduce conflict, then swap the periods
                    //if(succeeded){
                    if(conflictAfter1 + conflictAfter2 <= conflictBefore1 + conflictBefore2){
                        solution2[exam] = periodTemp2;
                        solution2[exam2] = periodTemp1;
                        break;
                    }
                }
            }
        }
    }

    //int cost = countConflicts2(solution2, conflictMatrix, conflictSet);
    //cout << "conflicts: " << cost << endl;
    return solution2;
}

// reduce conflicts using kick exam
// added by komar
vector<int> ReduceConflictsUsingKickExam(vector<int> solution3, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution3;

    // check the exams that are still have conflicts
    bool hasConflict;
    int periodTemp1, periodTemp2, periodTemp3;
    bool succeeded, succeeded2;
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);
    vector<int> examList;
    for(int i = 0; i < solution3.size(); i++)
        examList.push_back(i);
    // shuffle the sequence
    std::random_shuffle(examList.begin(), examList.end());
    int exam;
    int conflictBefore1, conflictBefore2;
    int conflictAfter1, conflictAfter2;
    for(int i = 0; i < solution2.size(); i++){
        exam = examList[i];
        hasConflict = false;
        for(int index = 0; index < conflictSet[exam].size(); index++){
            //if(exam < conflictSet[exam][index]){
                if(solution2[exam] == solution2[conflictSet[exam][index]]){
                    //this exam still has conflict, try to swap with the one that different with itrExamWithConflicts
                    hasConflict = true;
                    break;
                }
            //}
        }

        if(hasConflict){
            for(int index = 0; index < conflictSet[exam].size(); index++){
                if(solution2[exam] != solution2[conflictSet[exam][index]]){
                    // these two exams have two different period. 
                    // checked if they are being kicked, are the result resolve the conflict
                    conflictBefore1 = 0;
                    conflictAfter1 = 0;
                    conflictBefore2 = 0;
                    conflictAfter2 = 0;
                    int exam2 = conflictSet[exam][index];
                    periodTemp1 = solution2[exam];
                    periodTemp2 = solution2[exam2];                    
                    periodTemp3 = randPeriod(rng); 
                    if(periodTemp1 == periodTemp3 || periodTemp2 == periodTemp3)
                        periodTemp3 = randPeriod(rng); 
                    for(int checked1 = 0; checked1 < conflictSet[exam].size(); checked1++){
                        if(solution2[conflictSet[exam][checked1]] == periodTemp1)
                            conflictBefore1++;
                        if(conflictSet[exam][checked1] != exam2 && solution2[conflictSet[exam][checked1]] == periodTemp2)
                            conflictAfter1++;
                    }
                    for(int checked1 = 0; checked1 < conflictSet[exam2].size(); checked1++){
                        if(solution2[conflictSet[exam2][checked1]] == periodTemp2)
                            conflictBefore2++;
                        if(conflictSet[exam2][checked1] != exam && solution2[conflictSet[exam2][checked1]] == periodTemp3)
                            conflictAfter2++;
                    }
                    //if it reduce conflict, then swap the periods
                    //if(succeeded){
                    if(conflictAfter1 + conflictAfter2 <= conflictBefore1 + conflictBefore2){
                        solution2[exam] = periodTemp2;
                        solution2[exam2] = periodTemp3;
                        break;
                    }
                }
            }
        }
    }

    //int cost = countConflicts2(solution2, conflictMatrix, conflictSet);
    //cout << "conflicts: " << cost << endl;
    return solution2;
}

// reduce conflicts using double kick exam
// added by komar
vector<int> ReduceConflictsUsingDoubleKickExam(vector<int> solution3, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution3;

    // check the exams that are still have conflicts
    bool hasConflict;
    int periodTemp1, periodTemp2, periodTemp3, periodTemp4;
    bool succeeded0, succeeded, succeeded2;
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);
    int exam2, exam3;
    for(int exam = 0; exam < solution2.size(); exam++){
        hasConflict = false;
        for(int index = 0; index < conflictSet[exam].size(); index++){
            //if(exam < conflictSet[exam][index]){
                if(solution2[exam] == solution2[conflictSet[exam][index]]){
                    //this exam still has conflict, try to swap with the one that different with itrExamWithConflicts
                    hasConflict = true;
                    break;
                }
            //}
        }

        if(hasConflict){
            for(int index = 0; index < conflictSet[exam].size(); index++){
                if(solution2[exam] != solution2[conflictSet[exam][index]]){
                    succeeded0 = false;
                    // exam and exam2 have two different period. now we find exam3 that has different period with exam2.
                    exam2 = conflictSet[exam][index];
                    for(int index3 = 0; index3 < conflictSet[exam2].size(); index3++){
                        if(solution2[exam2] != solution2[conflictSet[exam2][index3]]){
                            succeeded = true;
                            // exam2 and exam3 have two different period.
                            exam3 = conflictSet[exam2][index3];
                            periodTemp1 = solution2[exam];
                            periodTemp2 = solution2[exam2];
                            periodTemp3 = solution2[exam3];
                            // checked if they are being kicked, are the result resolve the conflict
                            for(int checked1 = 0; checked1 < conflictSet[exam].size(); checked1++){
                                if(conflictSet[exam][checked1] != exam2 && solution2[conflictSet[exam][checked1]] == periodTemp2){
                                    succeeded = false;
                                    break;
                                }
                            }
                            if(succeeded){
                                for(int i = 0; i < noOfPeriods; i++){
                                    succeeded2 = true;
                                    for(int checked2 = 0; checked2 < conflictSet[exam2].size(); checked2++){
                                        if(solution2[conflictSet[exam2][checked2]] == periodTemp3){
                                            succeeded2 = false;
                                            break;
                                        }
                                    }
                                    if(succeeded2)
                                        break;
                                }
                                if(!succeeded2)
                                    succeeded = false;
                            }
                            if(succeeded){
                                for(int i = 0; i < noOfPeriods; i++){
                                    periodTemp4 = randPeriod(rng); 
                                    succeeded2 = true;
                                    for(int checked2 = 0; checked2 < conflictSet[exam3].size(); checked2++){
                                        if(solution2[conflictSet[exam3][checked2]] == periodTemp4){
                                            succeeded2 = false;
                                            break;
                                        }
                                    }
                                    if(succeeded2)
                                        break;
                                }
                                if(!succeeded2)
                                    succeeded = false;
                            }
                            //if it reduce conflict, then swap the periods
                            if(succeeded){
                                solution2[exam] = periodTemp2;
                                solution2[exam2] = periodTemp3;
                                solution2[exam3] = periodTemp4;
                                succeeded0 = true;
                                break;
                            }

                        }
                    }
                    if(succeeded0){
                        break;
                    }
                }
            }
        }
    }

    //int cost = countConflicts2(solution2, conflictMatrix, conflictSet);
    //cout << "conflicts: " << cost << endl;
    return solution2;
}

// reset exam with conflicts
// added by komar
vector<int> ResetExamWithConflicts(vector<int> solution, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution;
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);

    // check the exams that are still have conflicts
    int samePeriod;
    bool foundConflict;
    vector<int> examWithConflicts;
    for(int exam = 0; exam < solution.size(); exam++){
        foundConflict = false;
        for(int index = 0; index < conflictSet[exam].size(); index++){
            if(solution2[exam] == solution2[conflictSet[exam][index]]){
                //this exam still has conflict, reset with a random period
                foundConflict = true;
                examWithConflicts.push_back(exam);
                break;
            }
        }
        // if(foundConflict){
        //     samePeriod = solution2[exam];
        //     solution2[exam] = randPeriod(rng); 
        //     for(int index = 0; index < conflictSet[exam].size(); index++){
        //         if(solution2[conflictSet[exam][index]] == samePeriod){
        //             solution2[conflictSet[exam][index]] = randPeriod(rng); 
        //         }
        //     }
        // }
        // if(foundConflict){
        //     samePeriod = solution2[exam];
        //     for(int i = 0; i < solution2.size(); i++){
        //         if(solution2[i] == samePeriod){
        //             solution2[i] = randPeriod(rng);
        //         }
        //     }
        // }
        for(int i = 0; i < examWithConflicts.size(); i++){
            solution2[examWithConflicts[i]] = randPeriod(rng);
        }
    }
    return solution2;
}

// reset one exam with conflicts
// added by komar
vector<int> ResetOneExamWithConflicts(vector<int> solution, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution;
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);

    vector<int> examWithConflicts;
    for(int exam = 0; exam < solution.size(); exam++){
        for(int index = 0; index < conflictSet[exam].size(); index++){
            if(solution2[exam] == solution2[conflictSet[exam][index]]){
                //this exam still has conflict, reset with a random period
                examWithConflicts.push_back(exam);
                break;
            }
        }
    }
    uniform_int_distribution<int> randExam(0, examWithConflicts.size()-1);
    int exam = examWithConflicts[randExam(rng)];
    solution2[exam] = randPeriod(rng);

    return solution2;

}

// reset conflict exam with swap periods
// added by komar
vector<int> ResetConflictExamWithSwapPeriods(vector<int> solution, int noOfPeriods, vector<vector<int>> conflictMatrix, vector<vector<int>> conflictSet) {

    vector<int> solution2 = solution;
    mt19937 rng(time(NULL));
    uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);

    // check the exams that are still have conflicts
    int period1, period2;
    bool foundConflict;
    vector<int> examWithConflicts;
    for(int exam = 0; exam < solution.size(); exam++){
        foundConflict = false;
        for(int index = 0; index < conflictSet[exam].size(); index++){
            if(solution2[exam] == solution2[conflictSet[exam][index]]){
                //this exam still has conflict, reset with a random period
                foundConflict = true;
                examWithConflicts.push_back(exam);
                break;
            }
        }
        // using built-in random generator to shuffle examWithConflicts
        std::random_shuffle ( examWithConflicts.begin(), examWithConflicts.end() );
        // for(int i = 0; i < examWithConflicts.size(); i+= 2){
        //     if(i + 1 <  examWithConflicts.size()){
        //         period1 = solution2[examWithConflicts[i]];
        //         period2 = solution2[examWithConflicts[i+1]];
        //         for(int j = 0; j < solution.size(); j++){
        //             if(solution2[j] == period1){
        //                 solution2[j] = period2;
        //             }else if(solution2[j] == period2){
        //                 solution2[j] = period1;
        //             }
        //         }
        //     }
        // }
        for(int i = 0; i < examWithConflicts.size(); i++){
            period1 = solution2[examWithConflicts[i]];
            period2 = randPeriod(rng);
            for(int j = 0; j < solution.size(); j++){
                if(solution2[j] == period1){
                    solution2[j] = period2;
                }
                else if(solution2[j] == period2){
                    solution2[j] = period1;
                }
            }
        }
    }
    return solution2;
}

// modified great deluge stage 2
vector<int> greatDelugePenalty(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, int maxIteration) {
    // Setting Parameters
    mt19937 rng(time(NULL));
    vector<int> bestSolution = solution;
    int cost = calculatePenalty(solution, conflictMatrix);
    double desired = cost * 0.8;
    double level = cost * 1.01;
    double rainSpeed = (cost - desired)/(maxIteration/4);
    int bestCost = cost;
    
    // Start Algorithm
    int i = 0;
    int lastImproved = 0;
    uniform_int_distribution<int> randOpt(1,100);
    uniform_int_distribution<int> randRise(1,5);
    while (i < maxIteration) {
        vector<int> newSolution;
        int chooseOpt = randOpt(rng);
        switch (chooseOpt) {
            case 1 ... 50:
                newSolution = moveExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 51 ... 65:
                newSolution = kempeChain(noOfPeriods, solution, conflictMatrix);
                break;
            case 66 ... 80:
                newSolution = kickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 81 ... 85:
                newSolution = doubleKickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 86 ... 100:
                newSolution = swapPeriods(noOfPeriods, solution);
                break;
                
        }

        // Update new cost:
        int newCost = calculatePenalty(newSolution, conflictMatrix);
        if (newCost <= cost || newCost <= level) {
            solution = newSolution;
            cost = newCost;
            lastImproved = 0;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        } else {
            lastImproved++;
        }

        // Update best cost
        if (newCost < bestCost) {
            bestSolution = newSolution;
            bestCost = newCost;
            level = min(cost * 1.01, level);
        }

        // Update water level
        level -= rainSpeed;

        if (lastImproved >= 50 || level <= bestCost || cost <= desired) {
            solution = bestSolution;
            cost = bestCost;
            level = cost * 1.01;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        }
        if (cost <= desired) {
            desired = cost * 0.8;
            rainSpeed = (cost - desired) / ((maxIteration - i)/4);
        }

        i++;
    }
    return bestSolution;
}

vector<int> greatDelugePenaltyPartial(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, int maxIteration) {
    // Setting Parameters
    mt19937 rng(time(NULL));
    vector<int> bestSolution = solution;
    int cost = calculatePenalty(solution, conflictMatrix);
    double desired = cost * 0.8;
    double level = cost * 1.01;
    double rainSpeed = (cost - desired)/(maxIteration/4);
    int bestCost = cost;
    
    // Start Algorithm
    int i = 0;
    int lastImproved = 0;
    uniform_int_distribution<int> randOpt(1,100);
    while (i < maxIteration) {
        vector<int> newSolution;
        vector<int> movedExams;
        int chooseOpt = randOpt(rng);
        switch (chooseOpt) {
            case 1 ... 50:
                newSolution = moveExamPartial(noOfPeriods, solution, conflictMatrix, movedExams);
                break;
            case 51 ... 65:
                newSolution = kempeChainPartial(noOfPeriods, solution, conflictMatrix, movedExams);
                break;
            case 66 ... 80:
                newSolution = kickExamPartial(noOfPeriods, solution, conflictMatrix, movedExams);
                break;
            case 81 ... 85:
                newSolution = doubleKickExamPartial(noOfPeriods, solution, conflictMatrix, movedExams);
                break;
            case 86 ... 100:
                newSolution = swapPeriodsPartial(noOfPeriods, solution, movedExams);
                break;
                
        }

        // Update new cost:
        int newCost = calculateNewPenalty(solution, newSolution, conflictMatrix, movedExams, cost);
        if (newCost <= cost || newCost <= level) {
            solution = newSolution;
            cost = newCost;
            lastImproved = 0;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        } else {
            lastImproved++;
        }

        // Update best cost
        if (newCost < bestCost) {
            bestSolution = newSolution;
            bestCost = newCost;
            level = min(cost * 1.01, level);
        }

        // Update water level
        level -= rainSpeed;

        if (lastImproved >= 50 || level <= bestCost || cost <= desired) {
            solution = bestSolution;
            cost = bestCost;
            level = cost * 1.01;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        }
        if (cost <= desired) {
            desired = cost * 0.8;
            rainSpeed = (cost - desired) / ((maxIteration - i)/4);
        }

        i++;
    }
    return bestSolution;
}

vector<int> greatDelugePenaltyDueck(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, int maxIteration) {
    // Setting Parameters
    mt19937 rng(time(NULL));
    vector<int> bestSolution = solution;
    int cost = calculatePenalty(solution, conflictMatrix);
    double desired = cost * 0.8;
    double level = cost * 1.01;
    double rainSpeed = (cost - desired)/(maxIteration);
    int bestCost = cost;
    
    // Start Algorithm
    int i = 0;
    int lastImproved = 0;
    uniform_int_distribution<int> randOpt(1,100);
    uniform_int_distribution<int> randRise(1,5);
    while (i < maxIteration) {
        vector<int> newSolution;
        int chooseOpt = randOpt(rng);
        switch (chooseOpt) {
            case 1 ... 50:
                newSolution = moveExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 51 ... 65:
                newSolution = kempeChain(noOfPeriods, solution, conflictMatrix);
                break;
            case 66 ... 80:
                newSolution = kickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 81 ... 85:
                newSolution = doubleKickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 86 ... 100:
                newSolution = swapPeriods(noOfPeriods, solution);
                break;
                
        }

        // Update new cost:
        int newCost = calculatePenalty(newSolution, conflictMatrix);
        if (newCost <= cost || newCost <= level) {
            solution = newSolution;
            cost = newCost;
            lastImproved = 0;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        } else {
            lastImproved++;
        }

        // Update best cost
        if (newCost < bestCost) {
            bestSolution = newSolution;
            bestCost = newCost;
        }

        // Update water level
        level -= rainSpeed;
        i++;
    }
    return bestSolution;
}

vector<int> greatDelugePenaltyModified(int noOfPeriods, vector<int> solution, vector<vector<int>> conflictMatrix, int maxIteration) {
    // Setting Parameters
    mt19937 rng(time(NULL));
    vector<int> bestSolution = solution;
    int cost = calculatePenalty(solution, conflictMatrix);
    double desired = cost * 0.8;
    double level = cost * 1.01;
    double rainSpeed = (cost - desired)/(maxIteration);
    int bestCost = cost;
    
    // Start Algorithm
    int i = 0;
    int lastImproved = 0;
    uniform_int_distribution<int> randOpt(1,100);
    uniform_int_distribution<int> randRise(1,5);
    while (i < maxIteration) {
        vector<int> newSolution;
        int chooseOpt = randOpt(rng);
        switch (chooseOpt) {
            case 1 ... 50:
                newSolution = moveExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 51 ... 65:
                newSolution = kempeChain(noOfPeriods, solution, conflictMatrix);
                break;
            case 66 ... 80:
                newSolution = kickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 81 ... 85:
                newSolution = doubleKickExam(noOfPeriods, solution, conflictMatrix);
                break;
            case 86 ... 100:
                newSolution = swapPeriods(noOfPeriods, solution);
                break;
                
        }

        // Update new cost:
        int newCost = calculatePenalty(newSolution, conflictMatrix);
        if (newCost <= cost || newCost <= level) {
            solution = newSolution;
            cost = newCost;
            lastImproved = 0;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        } else {
            lastImproved++;
        }

        // Update best cost
        if (newCost < bestCost) {
            bestSolution = newSolution;
            bestCost = newCost;
        }

        // Update water level
        level -= rainSpeed;
        
        if (lastImproved >= 50 || level <= bestCost || cost <= desired) {
            solution = bestSolution;
            cost = bestCost;
            // cout << "c, bc, lv: (" << cost << ", " << bestCost << ", " << level << ") " << maxIteration - i << endl;
        }
        if (cost <= desired) {
            desired = cost * 0.8;
            rainSpeed = (cost - desired) / ((maxIteration - i));
            level = cost * 1.01;
        }

        i++;
    }
    return bestSolution;
}

/************************************************************************main******************************************************************/
int main (int argc, char *argv[]) { 

    // calculate computation time
    clock_t tStart = clock();

    // random number seed
    mt19937 rng(time(NULL));

    /************************************************************ opening the file ************************************************************/
    cout << endl << "Opening the file...." << endl;
    if (argc != 2) {
        cout << "input_instance" << endl;
        exit(1);
    }
    
    ifstream inFile(argv[1], ios::in);
    
    // input file could not be opened
    if (!inFile) {
        cout << "Error!! Input file: " << argv[1] << " could not be opened" << endl;
        inFile.close();
        exit(-1);
    } // end if

    // input data
    // number of exams, students, and periods
    int noOfExams, noOfStudents, noOfPeriods = 0;

    string temp;

    inFile >> temp;
    noOfExams = stoi(temp);
    // cout << "Number of Exams: " << noOfExams << "\n";

    inFile >> temp;
    noOfStudents = stoi(temp);
    // cout << "Number of Student: " << noOfStudents << "\n";

    inFile >> temp;
    noOfPeriods = stoi(temp);
    // cout << "Number of Periods: " << noOfPeriods << "\n";

    // number of students registered to an exams
    string exams[noOfExams];
    double students[noOfStudents];
    vector<pair<int,int>> largestEnrolment;
    for (int i = 0; i < noOfExams; i++) {
        inFile >> temp;
        exams[i] = temp;
        int a = stoi(exams[i]) - 1;
        inFile >> temp;
        students[i] = stoi(temp) - 1;
        int b = students[i];
        
        largestEnrolment.push_back(make_pair(a,b));
        
        // cout << "Exam " << exams[i] << ": " << students[i] << " students. \n";
    }
    sort(largestEnrolment.begin(), largestEnrolment.end(), sortbysec); 

    // for (int i = 0; i < noOfExams; i++) {
    //     cout << studentsInExam[i][0] << "\t" << studentsInExam[i][1] << endl;
    // }

    // exams taken by a student
    vector<vector<int>> examStudents;
    // inFile >> temp;
    int fileReader;
    for (int i = 0; i < noOfExams; i++) {
        examStudents.push_back(vector<int>());
    }

    while(inFile >> temp) {
        int studentID = stoi(temp.erase(0,1)) - 1;
        inFile >> temp;
        examStudents[stoi(temp) - 1].push_back(studentID);
    }
    inFile.close(); 

    /****************************************************** opening literature solution file***********************************************************/

    // vector<int> literatureSolution;
    // if (argc < 3) {
    //     cout << "solution not found" << endl;
    //     //exit(1);
    // } else {
    //     cout << endl << "Opening the solution file.... " << argv[2] << endl;
    //     ifstream inFile(argv[2], ios::in);
        
    //     // input file could not be opened
    //     if (!inFile) {
    //         cout << "Error!! Input file: " << argv[2] << " could not be opened" << endl;
    //         inFile.close();
    //         //exit(-1);
    //     } // end if

    //     // input data
    //     // number of exams, students, and periods
    //     // int noOfExams, noOfStudents, noOfPeriods = 0;

    //     literatureSolution.resize(noOfExams);

    //     while(inFile >> temp) {
    //         int examID = stoi(temp) - 1;
    //         inFile >> temp;
    //         int periodID = stoi(temp) - 1;
    //         literatureSolution[examID] = periodID;
    //         //cout << "examID: " << examID << ", periodID: " << periodID << endl;
    //     }
    //     inFile.close(); 

    // }

    /**************************************************************** Conflict Matrix ****************************************************************/

    // conflict matrix
    cout << "Creating conflict matrix....";
    clock_t tConflictMatrix = clock();    
    vector<vector<int>> conflictMatrix;
    for (int exam = 0; exam < noOfExams; exam++) {
        conflictMatrix.push_back(vector<int>());
        for (int exam2 = 0; exam2 < noOfExams; exam2++) {
            if (exam == exam2) {
                conflictMatrix[exam].push_back(0);
                // cout << conflictMatrix[exam][exam2] << " ";
            } else {
                conflictMatrix[exam].push_back(countDuplicates(examStudents[exam], examStudents[exam2]));
                // cout << conflictMatrix[exam][exam2] << " ";
            }
        }
        // cout << endl;
    }
    printf(" (time taken: %.2fs)\n", (double)(clock() - tConflictMatrix)/CLOCKS_PER_SEC);


    // added by komar
    cout << "Creating conflict matrix set....";
    tConflictMatrix = clock();   
    vector<vector<int>> conflictSet;
    for (int exam = 0; exam < noOfExams; exam++) {
        conflictSet.push_back(vector<int>());
        for (int exam2 = 0; exam2 < noOfExams; exam2++) {
            if(conflictMatrix[exam][exam2] > 0){
                conflictSet[exam].push_back(exam2);
            }
        }
    }
    printf(" (time taken: %.2fs)\n", (double)(clock() - tConflictMatrix)/CLOCKS_PER_SEC);

    /*************************************************************** Initial Solution ***************************************************************/

    // sort exam by largest degree
    // vector<vector<int>> largestDegree;
    // for (int exam = 0; exam < noOfExams; exam++) {
    //     largestDegree.push_back(vector<int>());
    //     int conflicts = 0;
    //     for (int exam2 = 0; exam2 < noOfExams; exam2++) {
    //         conflicts += min(conflictMatrix[exam][exam2],1);
    //     }
    //     // largestDegree[exam].push_back(exam); //modified by komar
    //     largestDegree[exam].push_back(conflicts);
    // }
    // sort(largestDegree.begin(), largestDegree.end(),sortcol);
    vector<pair<int,int>> largestDegree;
    for (int exam = 0; exam < noOfExams; exam++) {
        int conflicts = 0;
        for (int exam2 = 0; exam2 < noOfExams; exam2++) {
            conflicts += min(conflictMatrix[exam][exam2],1);
        }
        largestDegree.push_back(make_pair(exam, conflicts));
    }
    sort(largestDegree.begin(), largestDegree.end(), sortbysec);

    // sort exam by largest weighted degree
    vector<vector<int>> largestWeightedDegree;
    for (int exam = 0; exam < noOfExams; exam++) {
        largestWeightedDegree.push_back(vector<int>());
        int conflicts = 0;
        for (int exam2 = 0; exam2 < noOfExams; exam2++) {
            conflicts += conflictMatrix[exam][exam2];
        }
        //largestWeightedDegree[exam].push_back(exam); //modified by komar
        largestWeightedDegree[exam].push_back(conflicts);
    }
    sort(largestWeightedDegree.begin(), largestWeightedDegree.end(),sortcol);

    // generate solutions per period
    clock_t tInitialSolution = clock();
    cout << "\nGenerating initial solution.... ";

    // vector<int> solution1 = initialSolutionSD(noOfExams, noOfPeriods, conflictMatrix);
    // vector<int> solution1 = initialSolution(largestDegree, noOfPeriods, conflictMatrix);
    vector<int> solution1 = initialSolutionPair(largestDegree, noOfPeriods, conflictMatrix);
    // vector<int> solution1 = initialSolutionPair(largestEnrolment, noOfPeriods, conflictMatrix);

    // for (int exam = 0; exam < noOfExams; exam++) {
    //     // int sortedExam = studentsInExam[exam][0] - 1;
    //     int sortedExam = potentialConflicts[exam][0];
    //     // cout << sortedExam << endl;
    //     for (int period = 0; period < noOfPeriods; period++) {
    //         bool isSafe = 1;
    //         for (auto examConflict = solutionPeriods[period].begin(); examConflict != solutionPeriods[period].end(); ++examConflict) {
    //             if (conflictMatrix[sortedExam][*examConflict] > 0) {
    //                 isSafe = 0;
    //             }
    //         }
    //         if (isSafe) {
    //             solutionPeriods[period].insert(sortedExam);
    //             solutionExam.at(sortedExam) = period;
    //             goto nextExam;
    //         }
    //         if (period == noOfPeriods - 1) {
    //             solutionPeriods[period].insert(sortedExam);
    //             solutionExam.at(sortedExam) = period;
    //         }
    //     }
    //     nextExam:;
    // }

    // for (int exam = 0; exam < noOfExams; exam++) {
    //     // int sortedExam = studentsInExam[exam][0] - 1;
    //     int sortedExam = largestDegree[exam][0];
    //     cout << sortedExam << ": ";
    //     // finding safe periods
    //     vector<int> safePeriods;
    //     for (int period = 0; period < noOfPeriods; period++) {
    //         bool isSafe = 1;
    //         for (int exam2 = 0; exam2 < noOfExams; exam2++) {
    //             if (conflictMatrix[sortedExam][exam2] > 0 && period == solution[exam2]) {
    //                 isSafe = 0;
    //             }
    //         }
    //         if (isSafe) {
    //             safePeriods.push_back(period);
    //             cout << period << " ";
    //         }
    //     }
    //     // randomly choosing safe periods
    //     uniform_int_distribution<int> randSafePeriod(0, safePeriods.size()-1);
    //     uniform_int_distribution<int> randPeriod(0, noOfPeriods-1);
    //     int chosenPeriod;
    //     if (safePeriods.size() < 1) {
    //         chosenPeriod = randPeriod(rng);
    //         cout << "(all conflicts)";
    //     } else {
    //         // chosenPeriod = safePeriods[randSafePeriod(rng)];
    //         chosenPeriod = safePeriods[0];
    //     }
    //     solution.at(sortedExam) = chosenPeriod;
    //     cout << ": " << chosenPeriod << endl;
    // }

    // Random Initial Solution
    // uniform_int_distribution<int> gen(0, noOfPeriods-1);
    // vector<int> solution;
    // solution.resize(noOfExams);
    // for (int i = 0; i < noOfExams; i++) {
    //     solution.at(i) = gen(rng);
    //     // cout << "exam " << i << ": period " << solutionExam[i] << endl;
    // }

    printf(" (time taken: %.2fs)\n", (double)(clock() - tInitialSolution)/CLOCKS_PER_SEC);

    // // printing initial solution
    // for (int exam = 0; exam < noOfExams; exam++) {
    //     cout << exam + 1 << " " << solution4[exam] + 1 << endl;
    // }

    // counting conflicting exams initial solution
    // int conflicts = countConflicts(solution1, conflictMatrix);
    int conflicts = countConflicts2(solution1, conflictMatrix, conflictSet);
    cout << "Initial solution conflicts: " << conflicts << endl;
    
    // calculating penalties initial solution
    double penalty = calculatePenalty(solution1, conflictMatrix);
    cout << "Initial solution penalties: " << penalty << endl;

    // calculating objective value initial solution
    double ofv = penalty / noOfStudents;
    cout << "Initial solution objective value: " << ofv << endl;

    /*********************************************************************** stage 1 ***************************************************************/

    // validation with litrature solution
    // conflicts = countConflicts2(literatureSolution, conflictMatrix, conflictSet);
    // cout << "literatureSolution, conflict: " << conflicts << endl;

    // generate stage 1 solution
    clock_t tStageOne = clock();

    cout << "\nGenerating stage one solution.... " << endl;
    
    // added by komar
    cout << "Reducing conflict ....";
    vector<int> solutionStage1;
    solutionStage1 =  ReduceConflictsUsingMoveExam(solution1, noOfPeriods, conflictMatrix, conflictSet) ;
    conflicts = countConflicts2(solutionStage1, conflictMatrix, conflictSet);
    // cout << "after ReduceConflictsUsingMoveExam, conflict: " << conflicts << endl;
    solutionStage1 =  ReduceConflictsUsingSwapExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
    conflicts = countConflicts2(solutionStage1, conflictMatrix, conflictSet);
    // cout << "after ReduceConflictsUsingSwapExam, conflict: " << conflicts << endl;
    solutionStage1 =  ReduceConflictsUsingKickExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
    conflicts = countConflicts2(solutionStage1, conflictMatrix, conflictSet);
    // cout << "after ReduceConflictsUsingKickExam, conflict: " << conflicts << endl;
    //solutionStage1 =  ResetExamWithConflicts(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
    //conflicts = countConflicts2(solutionStage1, conflictMatrix, conflictSet);
    //cout << "after ResetExamWithConflicts, conflict: " << conflicts << endl;
    int prevConflicts = conflicts;
    int count = 0;

    while (conflicts > 0) {
    //for(int i = 0; i < 10; i++){
        solutionStage1 =  ReduceConflictsUsingMoveExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
        solutionStage1 =  ReduceConflictsUsingSwapExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
        solutionStage1 =  ReduceConflictsUsingKickExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
        //solutionStage1 =  ReduceConflictsUsingDoubleKickExam(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
        //solutionStage1 = greatDeluge(solutionStage1, conflictMatrix, 500);
        conflicts = countConflicts(solutionStage1, conflictMatrix);
        // cout << "conflict: " << conflicts << endl;
        //if(conflicts > 0)
        if(prevConflicts == conflicts){
            //cout << "Reset With ResetOneExamWithConflicts ... " << endl;
            //solutionStage1 =  ResetExamWithConflicts(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
            //solutionStage1 =  ResetConflictExamWithSwapPeriods(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
            solutionStage1 =  ResetOneExamWithConflicts(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
            //  count++;
            //  if(count > 10){
            //      count = 0;
            //      cout << "ResetExamWithConflicts... " << endl;
            //      //solutionStage1 =  ResetExamWithConflicts(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
            //      solutionStage1 =  ResetOneExamWithConflicts(solutionStage1, noOfPeriods, conflictMatrix, conflictSet) ;
            //  }
        }

        prevConflicts = conflicts;

    }
    
    printf(" (time taken: %.2fs)\n", (double)(clock() - tStageOne)/CLOCKS_PER_SEC);

    // // printing initial solution
    // for (int exam = 0; exam < noOfExams; exam++) {
    //     cout << exam + 1 << " " << solutionExam[exam] + 1 << endl;
    // }

    // counting conflicting exams stage 1
    conflicts = countConflicts(solutionStage1, conflictMatrix);
    cout << "New solution conflicts stage 1: " << conflicts << endl;

    // calculating penalties stage 1
    penalty = calculatePenalty(solutionStage1, conflictMatrix);
    cout << "New solution penalties stage 1: " << penalty << endl;

    // calculating objective value stage 1
    ofv = penalty / noOfStudents;
    cout << "New solution objective value stage 1: " << ofv << endl;

    /*********************************************************************** stage 2 ***************************************************************/
    
    // generate stage 2 solution
    clock_t tStageTwo = clock();
    cout << "\nGenerating stage two solution.... ";

    vector<int> solutionStage2 = greatDelugePenaltyPartial(noOfPeriods, solutionStage1, conflictMatrix, 1000000);

    printf(" (time taken: %.2fs)\n", (double)(clock() - tStageTwo)/CLOCKS_PER_SEC);

    // printing stage 2 solution
    for (int exam = 0; exam < noOfExams; exam++) {
        cout << exam + 1 << " " << solutionStage2[exam] + 1 << endl;
    }

    // counting conflicting exams stage 2
    conflicts = countConflicts(solutionStage2, conflictMatrix);
    cout << "New solution conflicts stage 2: " << conflicts << endl;

    // calculating penalties stage 2
    penalty = calculatePenalty(solutionStage2, conflictMatrix);
    cout << "New solution penalties stage 2: " << penalty << endl;

    // calculating objective value stage 2
    ofv = penalty / noOfStudents;
    cout << "New solution objective value stage 2: " << ofv << endl;

    /****************************************************************** dueck & modified ***********************************************************/
    
    // generate dueck solution
    clock_t tDueck = clock();
    cout << "\nGenerating solution using Dueck GDA.... ";

    vector<int> solutionDueck = greatDelugePenalty(noOfPeriods, solutionStage1, conflictMatrix, 1000000);
    
    printf(" (time taken: %.2fs)\n", (double)(clock() - tDueck)/CLOCKS_PER_SEC);

    // counting conflicting exams dueck
    conflicts = countConflicts(solutionDueck, conflictMatrix);
    cout << "New solution conflicts Dueck: " << conflicts << endl;

    // calculating penalties dueck
    penalty = calculatePenalty(solutionDueck, conflictMatrix);
    cout << "New solution penalties Dueck: " << penalty << endl;

    // calculating objective value dueck         
    ofv = penalty / noOfStudents;
    cout << "New solution objective value Dueck: " << ofv << endl;

    // generate modified solution
    clock_t tModified = clock();
    cout << "\nGenerating solution using modified GDA.... ";

    vector<int> solutionModified = greatDelugePenaltyModified(noOfPeriods, solutionStage1, conflictMatrix, 1000000);
    
    printf(" (time taken: %.2fs)\n", (double)(clock() - tModified)/CLOCKS_PER_SEC);

    // counting conflicting exams modified
    conflicts = countConflicts(solutionModified, conflictMatrix);
    cout << "New solution conflicts Modified GDA: " << conflicts << endl;

    // calculating penalties modified
    penalty = calculatePenalty(solutionModified, conflictMatrix);
    cout << "New solution penalties Modified GDA: " << penalty << endl;

    // calculating objective value modified
    ofv = penalty / noOfStudents;
    cout << "New solution objective value Modified GDA: " << ofv << endl;


    /*********************************************************************** end *******************************************************************/
    
    // print computation time
    printf("\nTotal time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}