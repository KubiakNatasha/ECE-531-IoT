int main(void) {


    openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
    syslog(LOG_INFO. "starting sampled");

    pid_t pid = fork();

    if (pid < 0){
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_FORK;
    }

    if (pid > 0){
        return OK;
    }

    if(setsid() < -1) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_SETSID;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


    umask(S_IRUSR | S_IWUSR | SIRGRP |S_IROTH);

    //Set the working directory.
    //Set to Root to make navigation easy

    if(chdir("/") < 0) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_CHDIR;
    }

    //signal handling

    signal(SIGTERM, _signal_handler);
    signal(SIGHUP, _signal_handler);

    _do_work();



}

    //work will be done by daemon
    //counts and sleeps

static void _do_work(void){
    for (int i = 0; true; i++){
        syslog(LOG_INFO, "iteration:%d", i);
        sleep(1);
    }
}



    //signal handler which will handle any signals passed

static void _signal_handler(cons in signal) {
    switch(signal) {
        case SIGHUP:
            break;

        case SIGTERM:
            syslog(LOG_INFO, "recieved SIGTERM, exiting.");
            closelog();
            exit(OK);
            break;
        default:
            syslog(LOG_INFO, "recieved unhandled signal.");
    }
}