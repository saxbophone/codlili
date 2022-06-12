template <typename T, class Container = std::list<T>>
class wait_adjusted_priority_queue {
    // priority is determined by the sum of usage-defined "priority" value and
    // e raised to the power of the time since it was added
    // we should probably use a tunable "time out" function, something like exp(n / p) / q,
    // where p and q are tuning parameters
};
