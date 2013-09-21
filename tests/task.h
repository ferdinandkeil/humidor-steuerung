// Simple Task Scheduler for Microcontrollers
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

#ifndef TASK_H
#define TASK_H

#include <stdint.h>

// System time
extern volatile uint32_t Time_now;

// Returns the system time.
#define millis() Time_now

// Increase system time.
#define tick() Time_now++

// Task_t
typedef struct {
	uint8_t (*canRun) (void *_self, uint32_t now);
	void (*run) (void *_self, uint32_t now);
	uint32_t runTime;
} Task_t;

// Set run-time for a task.
void Task_setRunTime(void *_self, uint32_t when);

// Increase the run-time of a task.
void Task_incRunTime(void *_self, uint32_t inc);

// Get the run-time of a task.
uint32_t Task_getRunTime(void *_self);

// TimedTask
// Determine if a task can run right now.
uint8_t TimedTask_canRun(void *_self, uint32_t now);

// TaskScheduler
// Include this in your main function.
void TaskScheduler(Task_t **tasks, uint8_t numTasks);

#endif