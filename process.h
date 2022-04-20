
class Process {
public:
    Process(int id, long long start, long long cost, long long memory)
        :process_id(id), arrive_time(start), service_time(cost), require_memory(memory){
        this->left_time = service_time;
        this->last_exec_time = arrive_time;
    }
    int process_id;
    long long arrive_time;
    long long service_time;
    long long require_memory;
    long long left_time;
    long long last_exec_time;
};
