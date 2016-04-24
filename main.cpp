#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

static pid_t xvfbPid;

void signalHandler(int signum) {

    kill(xvfbPid, SIGTERM);
    // reset env display
    const char *name = "DISPLAY";
    const char *value = ":0";
    int ret;

    if ((ret = setenv(name,value, 1)) != 0) {
        cerr << "Set Env Failed" << endl;
    }
}

int main() {

    //register signal handler
    signal(SIGUSR1, signalHandler);

    pid_t pid = fork();

    if (pid == 0) {
        //child process

        // set env display
        const char *name = "DISPLAY";
        const char *value = ":99";
        int ret;

        if ((ret = setenv(name,value, 1)) != 0) {
            cerr << "Set Env Failed" << endl;
        }

        //exec Xvfb
        const char *path = "/usr/bin/Xvfb";
        const char *arg0 = ":99";
        const char *arg1 = "-screen";
        const char *arg2 = "2";
        const char *arg3 = "1600x1200x32";
        const char *arg4 = "-shmem";
        execl(path, arg0, arg4, (char *)0);
    }

    else if (pid > 0) {
        //parent process
        cout << pid << endl;
        //print pid
        xvfbPid = pid;

        int status;

        //hang until Xvfb terminates
        waitpid(pid, &status, WUNTRACED);
        cout << "XVFB terminated" << endl;
    }

    else {
        cerr << "Fork Failed" << endl;
        return -1;
    }

    return 0;
}