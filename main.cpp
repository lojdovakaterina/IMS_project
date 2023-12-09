////////////////////////////////////////////////////////////////////////////
// Model MODEL5                   SIMLIB/C++
//
// Example of 2 facilities with single queue (random)
//

#include "main.h"

class CustomerNational : public Process { // customer description
    double ArrivalTime;                   // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
        // ArrivalTable(Time);
        // NArrivalTable(Time);

        double srvc_pct = Random() * 100;
        double service_time = 0; // Default value
        if (srvc_pct >= 0 && srvc_pct <= 12.07) {
            service_time = std::max((double)1, Normal(18.3, 1));
            // service_time = 18; // Hodnota pro první rozsah
        } else if (srvc_pct > 12.07 && srvc_pct <= 74.14) {
            service_time = std::max((double)1, Normal(28.25, 5.12));
            // service_time = 28.25; // Hodnota pro druhý rozsah
        } else if (srvc_pct > 74.14 && srvc_pct <= 86.21) {
            service_time = std::max((double)1, Normal(46.57, 6.19));
            // service_time = 46.57; // Hodnota pro třetí rozsah
        } else if (srvc_pct > 86.21) {
            service_time = std::max((double)1, Normal(83.38, 32.12));
            // service_time = 83.38; // Hodnota pro čtvrtý rozsah
        }

        ArrivalTime = Time; // mark start time

        // check for empty counter
        CounterNum = -1;
        for(int i = 0; i < national; i++){
            if (!NationalCounter[i].Busy()){
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) {
            // CounterNum = (Random() < 0.5) ? 0 : 1;
            // if (NationalCounter[CounterNum].Busy()) {
            Into(NationalQueue); // go into queue
            Passivate();         // sleep
            for(int i = 0; i < national; i++){
                if (!NationalCounter[i].Busy()){
                    CounterNum = i;
                    break;
                }
            }
            if(CounterNum == -1){
                printf("HELP N\n");
            }
        }

        Seize(NationalCounter[CounterNum]); // start service

        ServiceTime = Time;
        Wait(service_time);
        NationalService(Time - ServiceTime);
        Release(NationalCounter[CounterNum]); // end service
        if (!NationalQueue.empty()) {
            CustomerNational *z = (CustomerNational *)(NationalQueue.front());
            z->Out();      // remove z from queue
            z->Activate(); // wake-up
            NationalWaitingTable(Time - ArrivalTime);
        }
        // Table(Time - ArrivalTime); // record total time

        // if (NationalCounter[0].In() == this)
        //     Release(NationalCounter[0]);  // end service
        // else Release(NationalCounter[1]); // end service

        // if queue contains a customer for released Counter, activate it
        // for (Queue::iterator p = NationalQueue.begin();
        //      p != NationalQueue.end();
        //      ++p) {
        //     CustomerNational *z = (CustomerNational *)(*p);
        //     if (z->CounterNum == CounterNum) {
        //         z->Out();      // remove z from queue
        //         z->Activate(); // wake-up
        //         break;
        //     }
        // }                          // for
    }

public:
    CustomerNational() {
        Activate();
    }
}; // Customer

class CustomerInter : public Process { // customer description
    double ArrivalTime;                // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
        // ArrivalTable(Time);
        // IArrivalTable(Time);

        double srvc_pct = Random() * 100;
        double service_time = 0; // Default value
        if (srvc_pct >= 0 && srvc_pct <= 22.58) {
            service_time = std::max((double)1, Normal(13.57, 5.17));
            // service_time = 13.57; // Hodnota pro první rozsah
        } else if (srvc_pct > 22.58 && srvc_pct <= 67.74) {
            service_time = std::max((double)1, Normal(27.89, 6.05));
            // service_time = 27.89; // Hodnota pro druhý rozsah
        } else if (srvc_pct > 67.74 && srvc_pct <= 83.87) {
            service_time = std::max((double)1, Normal(45.7, 6.31));
            // service_time = 45.7; // Hodnota pro třetí rozsah
        } else if (srvc_pct > 83.87) {
            service_time = std::max((double)1, Normal(109.1, 57.33));
            // service_time = 109.1; // Hodnota pro čtvrtý rozsah
        }

        ArrivalTime = Time; // mark start time
        // CounterNum = (Random() < 0.5) ? 0 : 1;
        // if (InterCounter[CounterNum].Busy()) {

        CounterNum = -1;
        for(int i = 0; i < international; i++){
            if (!InterCounter[i].Busy()){
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) {
            Into(InterQueue); // go into queue
            Passivate();      // sleep
            for(int i = 0; i < international; i++){
                if (!InterCounter[i].Busy()){
                    CounterNum = i;
                    break;
                }
            }
            if(CounterNum == -1){
                printf("HELP I\n");
            }
        }
        Seize(InterCounter[CounterNum]); // start service

        ServiceTime = Time;
        Wait(service_time);
        InterService(Time - ServiceTime);

        Release(InterCounter[CounterNum]); // end service

        if (!InterQueue.empty()) {
            CustomerInter *z = (CustomerInter *)(InterQueue.front());
            z->Out();      // remove z from queue
            z->Activate(); // wake-up
            InterWaitingTable(Time - ArrivalTime);
        }
        // if (InterCounter[0].In() == this)
        //     Release(InterCounter[0]); // end service
        // else
        //     Release(InterCounter[1]); // end service

        // if queue contains a customer for released Counter, activate it
        // for (Queue::iterator p = InterQueue.begin();
        //      p != InterQueue.end();
        //      ++p) {
        //     CustomerInter *z = (CustomerInter *)(*p);
        //     if (z->CounterNum == CounterNum) {
        //         z->Out();      // remove z from queue
        //         z->Activate(); // wake-up
        //         break;
        //     }
        // }                          // for
    }

public:
    CustomerInter() { Activate(); }
}; // Customer

class Generator : public Event { // generator of customers
    void Behavior() {            // --- customer behavior ---
        double rnd_pct = Random() * 100;
        if (rnd_pct > COUNTER_PCT) {
            NArrivalTable(Time);
            new CustomerNational; // create new customer
        } else {
            IArrivalTable(Time);
            new CustomerInter; // create new customer
        }
        Activate(Time + Exponential(ARRIVAL_TIME));
        ArrivalTable(Time);
    }

public:
    Generator() {
        Activate();
    }
};

void argParse(int argc, char *argv[]) {
    int c;
    while ((c = getopt_long(argc, argv, short_args, long_args, NULL)) != -1) {
        switch (c) {
        case 'h':
            std::cout << "Použití: [-i] <hodnota> [-n] <hodnota> [-b] [-sd] [-ld] [-sc]\n"; // TODOO
            std::cout << "[-i] [--international] Počet otevřených mezinárodních přepážek\n";
            std::cout << "[-n] [--national] Počet otevřených vnitrostátních přepážek\n";
            std::cout << "[-b] [--base] Základní model\n";
            std::cout << "[-s] [self_checkout] Model přidání automatu na jízdenky\n";
            std::cout << "[-w] [side_window] Model otevření postraní přepážky\n";
            std::cout << "[-l] [line_divider] Model rozdělení fronty\n";
            std::cout << "Parametry [international] [national] jsou povinné a zadávají se v intervalu <1,6>\n";
            std::exit(EXIT_SUCCESS);
        case 'i':
            international = atoi(optarg);
            break;
        case 'n':
            national = atoi(optarg);
            break;
        case 'b':
        case 's':
        case 'w':
        case 'l':
            if (args.find(c) == std::string::npos) {
                args += c;
            } else {
                std::cout << "Vícekrát zadán stejný argument.\n";
                std::exit(EXIT_FAILURE);
            }
            break;
        default:
            std::cout << "Neznámý argument.\n";
            std::exit(EXIT_FAILURE);
        }
    }

    // Check if mandatory parameters are provided //TODO
    if (national < 1 || national > 6) {
        fprintf(stderr, "Počet vnitrostátních pokladen je nutné zadat. Musí také být v rozmezí 1 až 6.\n");
        std::exit(EXIT_FAILURE);
    }
    if (international < 1 || international > 4) {
        fprintf(stderr, "Počet mezinárodních pokladen je nutné zadat. Musí také být v rozmezí 1 až 6.\n");
        std::exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    RandomSeed(time(nullptr));
    argParse(argc, argv);

    // NationalCounter.SetCapacity(national);
    // InterCounter.SetCapacity(international);

    // RandomSeed(time(nullptr));
    int c;
    int i = 0;
    while ((c = args[i++]) != '\0') {
        switch (c) {
        case 'b':
            SetOutput("base.out");
            Print(" BASE MODEL \n");
            printf("Začátek simulace...\n");

            for(int i= 0; i < international; i++){
                char buffer[12];
                std::sprintf(buffer, "CounterI[%d]", i);
                InterCounter[i].SetName(buffer);
            }

            for(int i= 0; i < national; i++){
                char buffer[12];
                std::sprintf(buffer, "CounterN[%d]", i);
                NationalCounter[i].SetName(buffer);
            }

            Init(0, SIMULATION_TIME); // init experiment, time:0..1000
            new Generator;            // create and activate generator

            Run(); // simulation run
            // print reports:
            for(int i= 0; i < international; i++){
                InterCounter[i].Output();
            }
            for(int i= 0; i < national; i++){
                NationalCounter[i].Output();
            }

            InterQueue.Output();
            NationalQueue.Output();

            NationalService.Output();
            InterService.Output();
            InterWaitingTable.Output();
            NationalWaitingTable.Output();
            ArrivalTable.Output();
            IArrivalTable.Output();
            NArrivalTable.Output();

            printf("Konec simulace...\n");
            break;
        case 's':
            SetOutput("self_checkout.out");
            break;
        case 'w':
            SetOutput("side_window.out");
            break;
        case 'l':
            SetOutput("line_divider.out");
            break;
        }
    }
    return 0;
}
/*** End of file main.cpp ***/