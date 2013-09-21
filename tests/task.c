// Simple Task Scheduler for Microcontrollers
//
// This Task Scheduler is mainly a C-implementation of Alan Burlison's
// "A very simple Arduino task manager". One important improvement has
// been made to the code as it didn't handle timer rollovers gracefully.
// The idea is taken from the Arduino Playground article by Bombastic
// Bob.
//
// Links:
// - "A very simple Arduino task manager" by Alan Burlison
//   http://bleaklow.com/2010/07/20/a_very_simple_arduino_task_manager.html
// - "TimingRollover" by Bombastic Bob
//   http://www.arduino.cc/playground/Code/TimingRollover
//
// License: MIT
// Copyright (c) 2012 Ferdinand Keil, Alan Burlison, Bombastic Bob
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "task.h"

// System Time
volatile uint32_t Time_now;

// Set run-time for a task.
void Task_setRunTime(void *_self, uint32_t when) {
	Task_t *self = _self;
	self->runTime = when;
}

// Increase the run-time of a task.
void Task_incRunTime(void *_self, uint32_t inc) {
	Task_t *self = _self;
	self->runTime = self->runTime + inc;
}

// Get the run-time of a task.
uint32_t Task_getRunTime(void *_self) {
	Task_t *self = _self;
	return self->runTime;
}

// TimedTask
// Determine if a task can run right now.
uint8_t TimedTask_canRun(void *_self, uint32_t now) {
	Task_t *self = _self;
	return (int32_t) (now - self->runTime) >= 0;
}

// TaskScheduler
// Include this in your main function.
void TaskScheduler(Task_t **tasks, uint8_t numTasks) {
	static uint32_t now = 0;
	for (;;) {
		now = millis();
		Task_t **tpp = tasks;
		for (uint8_t t = 0; t < numTasks; t++) {
			Task_t *tp = *tpp;
			if (tp->canRun(tp, now)) {
				tp->run(tp, now);
				break;
			}
			tpp++;
		}
	}
}