/* erin li
4/19/2022
this is main generate the scheduling algorithms
github : https://github.com/ErinLi1998/scheduling-Alg
*/
#include<algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <queue>
#include "process.h"
#include "processor.h"

bool cmp_arrive( Process& p1,Process& p2)
{
    return p1.arrive_time < p2.arrive_time;
}

const long long INF=0x3f3f3f3f3f3f3f3f;

// First Come First Serve
int doFIFO(std::vector<Process>& processes, std::vector<Processor>& processors){
    // sort by arrive time
    long long size = processes.size();
    for(Process& p : processes){
        // find a processor to exec
        long long pre_time = -1;
        Processor* select_p = nullptr;
        for(Processor& pr : processors){
            if(pr.memory >= p.require_memory){
                if(pre_time == -1 || pre_time > pr.pre_exec_time){
                    pre_time = pr.pre_exec_time;
                    select_p = &pr;
                }
            }
        }
        if(select_p == nullptr){
            continue;
        }
        long long start_time = std::max(p.arrive_time, select_p->pre_exec_time);
        // exec p
        p.last_exec_time = start_time + p.service_time;
        select_p->pre_exec_time = start_time + p.service_time;
    }
    return 0;
}

// Shortest Job First
long long doSJF(std::vector<Process>& processes, std::vector<Processor>& processors){
    // sort by arrive time
    long long size = processes.size();
    std::sort(processes.begin(), processes.end(), cmp_arrive);
    // get t time
    long long t=processes[0].arrive_time;
    long long left_size = processes.size();
    while(true){
        long long exec_p = -1;
        long long min = INF;
        // find shortest job
        for(int i = 0; i < size; i ++){
            if(processes[i].left_time != 0 && processes[i].arrive_time <= t && processes[i].service_time < min){
                exec_p = i;
                min = processes[i].service_time;
            }
        }
        bool can_exec = false;
        // exist shortest job
        if( min != INF ){
            // find avaiabe processor
            for(Processor& pr : processors){
                if(pr.memory >= processes[exec_p].require_memory && pr.pre_exec_time < t){
                    // exec p
                    can_exec = true;
                    pr.pre_exec_time = t + processes[exec_p].service_time;
                    processes[exec_p].last_exec_time = t + processes[exec_p].service_time;
                    processes[exec_p].left_time = 0;
                    left_size --;
                    if(left_size == 0){
                        return 0;
                    }
                    break;
                }
            }
            if(!can_exec){
                // no avaiable processor
                long long next_t = INF;
                for(Processor& pr : processors){
                    if(pr.pre_exec_time >= t){
                        next_t = std::min(next_t, pr.pre_exec_time+1);
                    }
                }
                t = next_t;
            }
        }else{
            t=INF;
            for( int j=0; j < size; j++)
            {
                if(processes[j].left_time != 0) {
                    t = std::min(t, processes[j].arrive_time);
                }
            }
        }
    }
    return 0;
}

long long doSRT(std::vector<Process>& processes, std::vector<Processor>& processors){
    // sort by arrive time
    long long size = processes.size();
    std::sort(processes.begin(), processes.end(), cmp_arrive);
    // get t time
    long long t=processes[0].arrive_time;
    long long left_size = processes.size();
    int switch_cnt = 0;
    while(true){
        long long exec_p = -1;
        long long min = INF;
        // find shortest job
        for(int i = 0; i < size; i ++){
            if(processes[i].left_time != 0 && processes[i].arrive_time <= t && processes[i].left_time < min){
                exec_p = i;
                min = processes[i].left_time;
            }
        }
        bool can_exec = false;
        // exist shortest job
        if( min != INF ){
            // find avaiabe processor
            for(Processor& pr : processors){
                if(pr.memory >= processes[exec_p].require_memory && pr.pre_exec_time < t){
                    // find next process
                    long long next_t=INF;
                    for( int j=0; j < size; j++)
                    {
                        if(processes[j].left_time != 0 && processes[j].arrive_time > t) {
                            next_t = std::min(next_t, processes[j].arrive_time);
                        }
                    }
                    if(pr.pre_exec_time != 0 &&
                        processes[pr.pre_exec_process_id].left_time > 0 &&
                        pr.pre_exec_process_id != exec_p){
                        switch_cnt ++;
                    }
                    // exec p
                    can_exec = true;
                    long long exec_time = std::min(processes[exec_p].service_time, next_t - t);
                    pr.pre_exec_time = t + exec_time;
                    pr.pre_exec_process_id = exec_p;
                    processes[exec_p].last_exec_time = t + exec_time;
                    processes[exec_p].left_time -= exec_time;
                    if(processes[exec_p].left_time == 0){
                        left_size --;
                        if(left_size == 0){
                            return switch_cnt;
                        }
                    }
                    break;
                }
            }
            if(!can_exec){
                // no avaiable processor
                long long next_t = INF;
                for(Processor& pr : processors){
                    if(pr.pre_exec_time >= t){
                        next_t = std::min(next_t, pr.pre_exec_time+1);
                    }
                }
                t = next_t;
            }
        }else{
            t=INF;
            for( int j=0; j < size; j++)
            {
                if(processes[j].left_time != 0) {
                    t = std::min(t, processes[j].arrive_time);
                }
            }
        }
    }
    return switch_cnt;
}

int doRR(std::vector<Process>& processes, std::vector<Processor>& processors, int timeslice){
    long long t=processes[0].arrive_time;
    long long left_size = processes.size();
    int switch_cnt = 0;
    //wait all process exec
    while (true) {
        // find all process which arrive before t
        std::queue <int> q;//q is ready queue
        for(int i = 0 ; i < processes.size(); i ++){
            if(processes[i].arrive_time <= t && processes[i].left_time != 0){
                q.push(i);
            }
        }
        long long next_t = INF;
        for(Processor& pr : processors){
            if(pr.pre_exec_time >= t){
                next_t = std::min(next_t, pr.pre_exec_time+1);
            }
        }
        while (!q.empty() && t < next_t) {
            /* code */
            int exec_p = q.front();
            for(Processor& pr : processors){
                if(pr.memory >= processes[exec_p].require_memory && pr.pre_exec_time < t){
                    // exec process
                    q.pop();
                    if(pr.pre_exec_time != 0 &&
                        processes[pr.pre_exec_process_id].left_time > 0 &&
                        pr.pre_exec_process_id != exec_p){
                        switch_cnt ++;
                    }
                    pr.pre_exec_time = t + timeslice;
                    pr.pre_exec_process_id = exec_p;
                    processes[exec_p].last_exec_time = t + timeslice;
                    processes[exec_p].left_time -= timeslice;
                    if(processes[exec_p].left_time == 0){
                        left_size --;
                        if(left_size == 0){
                            return switch_cnt;
                        }
                    }else if(t < next_t){
                        q.push(exec_p);
                    }
                    break;
                }
            }
            t++;
        }
        t = next_t;
    }
    return switch_cnt;
}


int doML(std::vector<Process>& processes, std::vector<Processor>& processors){
    // sort by arrive time
    long long size = processes.size();
    for(Process& p : processes){
        // find a processor to exec
        long long pre_time = -1;
        Processor* select_p = nullptr;
        for(Processor& pr : processors){
            if(pr.memory >= p.require_memory){
                if(pre_time == -1 || pre_time > pr.pre_exec_time){
                    pre_time = pr.pre_exec_time;
                    select_p = &pr;
                }
            }
        }
        if(select_p == nullptr){
            continue;
        }
        long long start_time = std::max(p.arrive_time, select_p->pre_exec_time);
        // exec p
        p.last_exec_time = start_time + p.service_time;
        select_p->pre_exec_time = start_time + p.service_time;
    }
    return 0;
}

int main()
{
    std::cout << "choose which of the four scenarios you wish to simulate: " << std::endl;
    std::cout << "1.All four processors are identical with a speed of 3GHz and a memory capacity of 8GB" << std::endl;
    std::cout << "2.All four processors have identical speeds, 3GHz, but varying memory availability. P1 and P2 have 2GB, P3 has 4GB, and P4 has 8GB" << std::endl;
    std::cout << "3.All four processors have identical memory availability, 8GB, but varying speeds. P1 and P2 are 2GHz, P3 is 3GHz, and P4 is 4GHz" << std::endl;
    std::cout << "4.All four processors are identical with a speed of 3GHz and a memory capacity of 8GB" << std::endl;
    int type;
    std::cin >> type;
    std::cout << "choose which of the five scheduling algorithms you wish to use" << std::endl;
    std::cout << "1.FIFO\t2.SJF\t3.SRT\t4.RR\t5.ML" << std::endl;
    int algo;
    std::cin >> algo;
    std::cout << "which seed value you wish to use for the random number generator" << std::endl;

    // Seed for random value
    long long seed;
    std::cin >> seed;

    int Ghz = 1000000000;

    // Choose a random values
    std::default_random_engine e1(seed);
    std::uniform_int_distribution<int64_t> uniform_service_time(10000000, 10000000000000);
    std::uniform_int_distribution<int64_t> uniform_arrive_time(1, 100000000000);
    std::uniform_int_distribution<int64_t> uniform_memory(1, 8);

    std::vector<Process> processes;
    for(int i = 0; i < 50; i ++){
        long long start = uniform_arrive_time(e1);
        long long cost = uniform_service_time(e1);
        long long memory = uniform_memory(e1);
        processes.emplace_back(Process(i+1, start, cost, memory));
    }

    std::vector<Processor> processors;

    // Keep track of the number of simulated cycles that have elapsed during execution
    int track_cycle = 0;
    bool need_arrive = false;
    if(type == 1){
        processors.emplace_back(Processor(1, 3*Ghz, 8));
        processors.emplace_back(Processor(2, 3*Ghz, 8));
        processors.emplace_back(Processor(3, 3*Ghz, 8));
        processors.emplace_back(Processor(4, 3*Ghz, 8));
        // all 3Ghz and 8GB
    }else if(type == 2){
        // all 3Ghz, 2GB 2GB 4GB 8GB
        processors.emplace_back(Processor(1, 3*Ghz, 2));
        processors.emplace_back(Processor(2, 3*Ghz, 2));
        processors.emplace_back(Processor(3, 3*Ghz, 4));
        processors.emplace_back(Processor(4, 3*Ghz, 8));
    }else if(type == 3){
        // all 8GB, 2Ghz 2Ghz 3Ghz 4Ghz
        processors.emplace_back(Processor(1, 2*Ghz, 8));
        processors.emplace_back(Processor(2, 2*Ghz, 8));
        processors.emplace_back(Processor(3, 3*Ghz, 8));
        processors.emplace_back(Processor(4, 4*Ghz, 8));
    }else{
        // all 3Ghz and 8GB
        processors.emplace_back(Processor(1, 3*Ghz, 8));
        processors.emplace_back(Processor(2, 3*Ghz, 8));
        processors.emplace_back(Processor(3, 3*Ghz, 8));
        processors.emplace_back(Processor(4, 3*Ghz, 8));
        need_arrive = true;
    }

    if(!need_arrive){
        // set process arrive time to 0
        for(Process& p : processes){
            p.arrive_time = 1;
        }
    }

    int process_switch = 0;
    if(algo == 1){
        //1.FIFO
        process_switch = doFIFO(processes, processors);
    }else if(algo == 2){
        //2.SJF
        process_switch = doSJF(processes, processors);
    }else if(algo == 3){
        //3.SRT
        process_switch = doSRT(processes, processors);
    }else if(algo == 4){
        //4.RR
        process_switch = doRR(processes, processors, 1);
    }else{
        //5.ML
        process_switch = doML(processes, processors);
    }

    // The total turnaround time for the system, i.e. the total number of simulated cycles that have elapsed
    // The average wait time, in cycles, across all processes
    long long avg_wait_time = 0;
    long long total_cycles = 0;
    for(Process& p : processes){
        total_cycles = std::max(total_cycles, p.last_exec_time);
        avg_wait_time += p.last_exec_time - p.service_time - p.arrive_time;
    }
    avg_wait_time = avg_wait_time / processes.size();
    // The list of processes in the order they were processed, and for each process
    // Its ID number,Its service time, Its arrival time, Its memory requirements, Its wait time in cycles, Its total time in the system in cycles, i.e. wait time + service time
    for(Process& p : processes){
        std::cout << "Process: " << p.process_id << " Service time: " << p.service_time << " Arrival time: " << p.arrive_time << " Memory Requere: " << p.require_memory << "GB Wait Time: " << (p.last_exec_time - p.service_time - p.arrive_time) << "Total Time:" << (p.last_exec_time - p.arrive_time) << std::endl;
    }
    // The number of context switches the system experienced during its execution
    std::cout << "The number of context switches: " << process_switch << std::endl;
}
