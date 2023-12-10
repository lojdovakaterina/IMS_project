/**
 * @file main.cpp
 * @author Andrea Michlíková, Kateřina Lojdová
 * @brief
 */

#include "main.h"

using namespace std;

class CustomerNational : public Process { // customer description
    double ArrivalTime;                   // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
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
        for (int i = 0; i < national; i++) {
            if (!NationalCounter[i].Busy()) {
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) {
            Into(NationalQueue); // go into queue
            Passivate();         // sleep
            for (int i = 0; i < national; i++) {
                if (!NationalCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }
            if (CounterNum == -1) {
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
    }

public:
    CustomerNational() {
        Activate();
    }
}; // Customer

class CustomerNationalTwoLines : public Process { // customer description
    double ArrivalTime;                   // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
        std::cout << "BEHAVIOR" << endl;
        double srvc_pct = Random() * 100;
        // determine how long will it take to take care of the customer
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
        int chosen_line = 0;

        // if both queues are empty, choose one at random
        if(NationalSplitQueueLeft.empty() && NationalSplitQueueRight.empty()){
            std::cout << "queues empty" << endl;
            double rnd_pct = Random() * 100;
            if(rnd_pct > 50){
                std::cout << "random left" << endl;
                chosen_line = GO_LEFT; // go left
            }else{
                std::cout << "random right" << endl;
                chosen_line = GO_RIGHT; // go right
            }
        //if left has fewer people waiting, choose left
        }else if(NationalSplitQueueLeft.Length() < NationalSplitQueueRight.Length()){
            std::cout << "left shorter" << endl;
            chosen_line = GO_LEFT;
        }else{
            std::cout << "right shorter" << endl;
            chosen_line = GO_RIGHT;
        }

        if (chosen_line == GO_LEFT) {
            std::cout << "go left" << endl;
            // LEFT LINE
            CounterNum = -1;
            for (int i = 3; i < 3 + nationalLeft; i++) {
                std::cout << "try national counter L " << i << endl;
                if (!NationalCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }

            if (CounterNum == -1) {
                Into(NationalSplitQueueLeft); // go into queue
                Passivate();         // sleep
                for (int i = 3; i < 3 + nationalLeft; i++) {
                    if (!NationalCounter[i].Busy()) {
                        std::cout << "try national counter L " << i << " vol2" << endl;
                        CounterNum = i;
                        break;
                    }
                }
                if (CounterNum == -1) {
                    printf("HELP N\n");
                }
            }

            std::cout << "seize counter" << endl;
            Seize(NationalCounter[CounterNum]); // start service

            ServiceTime = Time;
            Wait(service_time);
            NationalServiceLeft(Time - ServiceTime);
            Release(NationalCounter[CounterNum]); // end service
            if (!NationalSplitQueueLeft.empty()) {
                CustomerNational *z = (CustomerNational *)(NationalSplitQueueLeft.front());
                z->Out();      // remove z from queue
                z->Activate(); // wake-up
                NationalWaitingTableLeft(Time - ArrivalTime);
            }
        } else {
            std::cout << "go right" << endl;
            //RIGHT LINE
            CounterNum = -1;
            for (int i = 0; i < nationalRight; i++) {
                std::cout << "try national counter R " << i << endl;
                if (!NationalCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }

            if (CounterNum == -1) {
                Into(NationalSplitQueueRight); // go into queue
                Passivate();         // sleep
                for (int i = 0; i < nationalRight; i++) {
                    std::cout << "try national counter R " << i << " vol2" << endl;
                    if (!NationalCounter[i].Busy()) {
                        CounterNum = i;
                        break;
                    }
                }
                if (CounterNum == -1) {
                    printf("HELP N\n");
                }
            }

            Seize(NationalCounter[CounterNum]); // start service

            ServiceTime = Time;
            Wait(service_time);
            NationalServiceRight(Time - ServiceTime);
            Release(NationalCounter[CounterNum]); // end service
            if (!NationalSplitQueueRight.empty()) {
                CustomerNational *z = (CustomerNational *)(NationalSplitQueueRight.front());
                z->Out();      // remove z from queue
                z->Activate(); // wake-up
                NationalWaitingTableRight(Time - ArrivalTime);
            }
        }
    }

public:
    CustomerNationalTwoLines() {
        Activate();
    }
}; // Customer

class CustomerInter : public Process { // customer description
    double ArrivalTime;                // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
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
        CounterNum = -1;
        for (int i = 0; i < international; i++) {
            if (!InterCounter[i].Busy()) {
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) {
            Into(InterQueue); // go into queue
            Passivate();      // sleep
            for (int i = 0; i < international; i++) {
                if (!InterCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }
            if (CounterNum == -1) {
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
    }

public:
    CustomerInter() { Activate(); }
}; // Customer

class Generator : public Event { // generator of customers
    void Behavior() {            // --- customer behavior ---
        double rnd_pct = Random() * 100;
        if (rnd_pct > COUNTER_PCT) {
            NArrivalTable(Time);
            if(nationalLeft != 0){
                new CustomerNationalTwoLines; // create new customer for model with two lines
            }else{
                new CustomerNational; // create new customer for base model
            }
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
            std::cout << "Použití: [-i] <hodnota> [-n] <hodnota> [-t] <hodnota> [-p] <hodnota> [-b] [-sd] [-ld] [-sc]\n"; // TODOO
            std::cout << "[-i] [--international] Počet otevřených mezinárodních přepážek\n---> Povinné v intervalu <1,4>\n";
            std::cout << "[-n] [--national] Počet otevřených vnitrostátních přepážek\n---> Povinné v intervalu <1,4>\n";
            std::cout << "[-t] [--time] Simulační čas v minutách\n";
            std::cout << "[-p] [--people] Počet příchozích osob\n";
            std::cout << "[-b] [--base] Základní model\n";
            std::cout << "[-s] [--self_checkout] Model přidání automatu na jízdenky\n";
            std::cout << "[-w] [--side_window] Model otevření postraní přepážky\n";
            std::cout << "[-l] [--line_divider] Model rozdělení fronty\n---> Povinné v intervalu <1,3> <1,3> (L a R prepazky)\n";
            std::exit(EXIT_SUCCESS);
        case 'i':
            international = atoi(optarg);
            break;
        case 'n':
            national = atoi(optarg);
            break;
        case 't':
            sim_time = atoi(optarg) * 60;
            break;
        case 'p':
            people = atoi(optarg);
            break;
        case 'l':{
            cout << "found -l" << endl;
            if(optarg == nullptr){
                std::cout << "Chybi argumenty argument pro volbu -l.\n";
                std::exit(EXIT_FAILURE);
            }
            std::istringstream iss(optarg);
            if(!(iss >> nationalLeft >> nationalRight) || !iss.eof()){
                std::cout << "Nespravny pocet argumentu pro volbu -l.\n";
                std::exit(EXIT_FAILURE);
            }
            cout << "made it through sscanf" << endl;
            cout << nationalLeft << endl;
            cout << nationalRight << endl;
        }
        case 'b':
        case 's':
        case 'w':
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
    cout << "inside main" << endl;
    RandomSeed(time(nullptr)); // different outputs
    cout << "gonna parse arguments" << endl;
    argParse(argc, argv);
    cout << "Parsed arguments" << endl;
    int c;
    int i = 0;

    // set histogram
    InterWaitingTable.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTable.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTableRight.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTableLeft.Init(0, SIMULATION_TIME, 1);
    IArrivalTable.Init(0, SIMULATION_TIME, 1);
    NArrivalTable.Init(0, SIMULATION_TIME, 1);

    while ((c = args[i++]) != '\0') {
        switch (c) {
        case 'b':
            SetOutput("base.out");
            Print(" BASE MODEL \n");
            printf("Začátek simulace...\n");

            for (int i = 0; i < international; i++) {
                char buffer[12];
                std::sprintf(buffer, "CounterI[%d]", i);
                InterCounter[i].SetName(buffer);
            }

            for (int i = 0; i < national; i++) {
                char buffer[12];
                std::sprintf(buffer, "CounterN[%d]", i);
                NationalCounter[i].SetName(buffer);
            }

            Init(0, SIMULATION_TIME); // init experiment
            new Generator;            // create and activate generator

            Run(); // simulation run
            // print reports:
            for (int i = 0; i < international; i++) {
                InterCounter[i].Output();
            }
            for (int i = 0; i < national; i++) {
                NationalCounter[i].Output();
            }

            // NationalService.Output();
            // InterService.Output();
            InterWaitingTable.Output();
            NationalWaitingTable.Output();
            IArrivalTable.Output();
            NArrivalTable.Output();
            // ArrivalTable.Output();
            InterQueue.Output();
            NationalQueue.Output();

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
            Print(" LINE DIVIDER MODEL \n");
            printf("Začátek simulace...\n");

            for (int i = 0; i < international; i++) {
                char buffer[12];
                std::sprintf(buffer, "CounterI[%d]", i);
                InterCounter[i].SetName(buffer);
            }

            for (int i = 0; i < nationalRight; i++) {
                char buffer[13];
                std::sprintf(buffer, "CounterNR[%d]", i);
                NationalCounter[i].SetName(buffer);
            }
            for (int i = 3; i < (3 + nationalLeft); i++) {
                char buffer[13];
                std::sprintf(buffer, "CounterNL[%d]", i);
                NationalCounter[i].SetName(buffer);
            }

            Init(0, SIMULATION_TIME); // init experiment
            new Generator;            // create and activate generator

            Run(); // simulation run
            // print reports:
            for (int i = 0; i < international; i++) {
                InterCounter[i].Output();
            }
            for (int i = 0; i < nationalRight; i++) {
                NationalCounter[i].Output();
            }
            for (int i = 3; i < 3 + nationalLeft; i++) {
                NationalCounter[i].Output();
            }

            InterWaitingTable.Output();
            NationalWaitingTableLeft.Output();
            NationalWaitingTableRight.Output();
            IArrivalTable.Output();
            NArrivalTable.Output();
            InterQueue.Output();
            NationalSplitQueueLeft.Output();
            NationalSplitQueueRight.Output();

            printf("Konec simulace...\n");
            break;
        }
    }
    return 0;
}
/*** End of file main.cpp ***/