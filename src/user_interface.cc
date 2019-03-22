#include <unistd.h>
#include "detection.h"


using namespace stopwatch;

UserInterface::UserInterface(StopWatch& sw) : Process("user input"), _stopwatch(sw) {
    initscr();   // Start ncurses
    timeout(1);  // Timeout for waiting for user input
    noecho();    // Do not echo user input to the screen
    curs_set(0); // Do not show the cursor
};


void UserInterface::update() {

    // USER INPUT
    // get a character from the user, if one is ready.
    // If no character is ready, getch() returns ERR (which is
    // ignored below since there is no condition in the switch statement)
    int c = getch();

    switch ( c ) {            
        case 's':
            emit(Event("start/stop"));
            if (startkey == true){
                mvprintw(10,1,"state: RUN ");
                startkey = false;
            }else if (startkey == false){
                mvprintw(10,1,"state: STOP ");
                startkey = true;
            }
            break;
        case 'r':
            emit(Event("reset"));
            startkey = false;
            clear(); // Clear the screen of old stuff
            mvprintw(10,1,"state:     ");
            break;
        // case 'l':
        //     emit(Event("lap"));
        //     break;
        case 'q':
            std::cout << "halting\n";
            halt();
            break;
    }
    
    dis = _stopwatch.getCM(); //gets the distance in centimeters from the Ultrasonic sensor
    //int dis = 24;
    if(dis>=22 && dis<=28){
        if (button == true){
            mvprintw(10,1,"state: STOP");
            button = false ;
            emit(Event("start/stop"));
        } 
    }
    else if(dis<22){
        if (button == false){
            mvprintw(10,1,"state: RUN ");
            button = true ;
            emit(Event("start/stop"));
        }
    } 
    else if(dis>28){
        if (button == false){
            button = true ;
            emit(Event("start/stop"));
            mvprintw(10,1,"state: RUN ");
        }
    } 

    mvprintw(1,1,"ELMA raspberry Pi detection proyect Detection");
    mvprintw(3,1,"start/stop(s), reset(r), quit(q)");
    //std::cout<<duration_cast<std::chrono::seconds>(_stopwatch.value()).count()<<'\n';
    double _pwm = 8 + ((std::chrono::duration_cast<std::chrono::milliseconds>(_stopwatch.value()).count())/1000)%37;
    
    if (_pwm>=26){
       temp = 52;
    }
    else if(_pwm<10){
       temp = 0;
    }
    _stopwatch.searching(abs(_pwm-temp)); // this function gives the PWM value to the servo motor
    mvprintw(4, 1, "----------------------------------");
    mvprintw(6, 1, "servo PWM %03.1lf", abs(_pwm-temp));
    //mvprintw(7, 1, "servo %03.1lf", _pwm);
    mvprintw(7, 1, "Distance: %d cm\n", dis);// distance in centimeters
    mvprintw(8, 1, "          %02.0lf inch\n", dis/2.54); // distance in inches
    
    // NOTE: Since the detection program is running every 10 ms, we should sleep
    //       the ui to give processing time back to the OS. It is debatable
    //       whether this is the right place to put this. It could also become
    //       an Elma feature, or it could go in the StopWatch class, etc.
    //       The number 9999 should also be a parameter and not a constant.
    usleep(9999);

}
