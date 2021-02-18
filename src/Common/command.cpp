#include "command.h"

void CommandQueue::push(Command command) {
    std::unique_lock<std::mutex> lock(m);
    if (commands.empty() || command != commands.back()){
        commands.push_back(command);
        lock.unlock();
        cv.notify_one();
    }
}

Command CommandQueue::pop() {
    std::unique_lock<std::mutex> lock(m);
    while (commands.empty()) {
        cv.wait(lock);
    }
    Command return_command = commands.front();
    commands.pop_front();
    return return_command;
}

Command CommandQueue::pop_or_empty() {
    std::unique_lock<std::mutex> lock(m);
    if (commands.empty()) {
        return Command::nothing;
    }
    Command return_command = commands.front();
    commands.pop_front();
    return return_command;
}

void CommandQueue::clear() {
    commands.clear();
}
