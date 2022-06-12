template <typename T, class Container = std::list<T>>
class wait_adjusted_priority_queue {
    // priority is determined by the sum of usage-defined "priority" value and
    // natural logarithm of the time since it was added
    // we should probably use a tunable log, something like log(n * p) / q,
    // where p and q are tuning parameters
};
