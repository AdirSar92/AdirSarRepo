# WatchDog

## Description

WatchDog is a C project that deals with multi-process communication(IPC) and is used to detect and recover from softwere malfunctions.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
Prerequisites

    GCC compiler
    Unix-like operating system

Installing

    Clone the repository:

git clone..

    Navigate to the project directory:

cd watchdog

    Compile the project:

make

    You can now run the project with the following command:

./watchdog/make



## Usage / Services

To use WatchDog, you will need to call to WatchDogStart().
The watchdog process will monitor the health of the monitored processes and take action if a malfunction is detected.
If you like to stop the monitoring, you can call WhatchDogStop().


