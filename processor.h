class Processor{
public:
    Processor(int id, long long speed, long long memory)
        :processor_id(id), speed(speed), memory(memory){
        pre_exec_time = 0;
    }
    int processor_id;
    long long speed;
    long long memory;
    // previous exec time
    long long pre_exec_time;
    // previous exec process
    long long pre_exec_process_id;
};
