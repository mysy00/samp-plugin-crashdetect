// Copyright (c) 2011 Zeex
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "interrupt.h"

Interrupt::Handler Interrupt::handler_;

#ifdef _WIN32

#include <Windows.h>

static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
	switch (dwCtrlType) {
	case CTRL_C_EVENT:
		Interrupt::Handler handler = Interrupt::GetHandler();
		if (handler != 0) {
			handler();
		}
	}
	return FALSE;
}

void Interrupt::SetHandler(Interrupt::Handler handler) {
	handler_ = handler;
	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
}

#else

#include <signal.h>

static void (*previousSignalHandler)(int);

static void HandleSIGINT(int sig) {
	Interrupt::Handler handler = Interrupt::GetHandler();
	if (handler != 0) {
		handler();
	}
	signal(sig, previousSignalHandler);
	raise(sig);
}

// static
void Interrupt::SetHandler(Interrupt::Handler handler) {
	handler_ = handler;
	previousSignalHandler = signal(SIGINT, HandleSIGINT);
}

#endif

// static
Interrupt::Handler Interrupt::GetHandler() {
	return handler_;
}
