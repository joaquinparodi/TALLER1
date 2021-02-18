#include "thread.h"
#include <iostream>
#include <utility>

Thread::Thread() {}

Thread::~Thread() {}

void Thread::start() {
	thread = std::thread(&Thread::run, this);
}

void Thread::join() {
	thread.join();
}

bool Thread::joinable() {
	return thread.joinable();
}

Thread::Thread(Thread&& other) {
	this->thread = std::move(other.thread);
}

Thread& Thread::operator=(Thread&& other) {
	this->thread = std::move(other.thread);
	return *this;
}
