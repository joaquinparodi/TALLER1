#pragma once

#include<mutex>
#include<condition_variable>
#include<list>

enum struct Command : int {
	stop,
	left = 1,
	right,
	jump,
	crouch,
	disconnect,
	no_left,
	no_right,
	no_crouch,
	nothing,
	test
};

class CommandQueue {
public:
	void push(Command command);
	Command pop();
	Command pop_or_empty();
	void clear();
private:
	std::mutex m;
	std::condition_variable cv;
	std::list<Command> commands;
};

/*
--> Client --> [int32] (comando) --> servidor
*/
