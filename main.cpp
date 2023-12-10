/**
 * @file main.cpp
 * @author Andrea Michlíková, Kateřina Lojdová
 * @brief IMS
 */

#include "main.h"

class CustomerNational : public Process { // customer description
    double ArrivalTime;                   // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
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

        // check for empty counter
        CounterNum = -1;
        for (int i = 0; i < national; i++) { // find free counter
            if (!NationalCounter[i].Busy()) {
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) {  // counters is busy, wait
            Into(NationalQueue); // go into queue
            Passivate();         // sleep
            for (int i = 0; i < national; i++) {
                if (!NationalCounter[i].Busy()) { // find free counter
                    CounterNum = i;
                    break;
                }
            }
            if (CounterNum == -1) { // should not occur
                printf("HELP N\n");
            }
        }

        Seize(NationalCounter[CounterNum]); // start service
        NationalWaitingTable(Time - ArrivalTime);

        ServiceTime = Time;
        Wait(service_time);
        NationalService(Time - ServiceTime);
        Release(NationalCounter[CounterNum]); // end service

        if (!NationalQueue.empty()) { // wake up next customer
            CustomerNational *z = (CustomerNational *)(NationalQueue.front());
            z->Out();      // remove z from queue
            z->Activate(); // wake-up
        }
    }

public:
    CustomerNational() {
        Activate();
    }
}; // Customer

class CustomerNationalTwoLines : public Process { // customer description
    double ArrivalTime;                           // start time
    double ServiceTime;
    int CounterNum;   // Counter to use
    void Behavior() { // --- customer behavior ---
        double srvc_pct = Random() * 100;
        // determine how long will it take to take care of the customer
        double service_time = 0; // Default value
        if (srvc_pct >= 0 && srvc_pct <= 12.07) {
            service_time = std::max((double)1, Normal(18.3, 1));
            // service_time = 18;
        } else if (srvc_pct > 12.07 && srvc_pct <= 74.14) {
            service_time = std::max((double)1, Normal(28.25, 5.12));
            // service_time = 28.25;
        } else if (srvc_pct > 74.14 && srvc_pct <= 86.21) {
            service_time = std::max((double)1, Normal(46.57, 6.19));
            // service_time = 46.57;
        } else if (srvc_pct > 86.21) {
            service_time = std::max((double)1, Normal(83.38, 32.12));
            // service_time = 83.38;
        }

        ArrivalTime = Time; // mark start time
        int chosen_line = 0;

        // if both queues are empty or the same length, choose one at random
        if ((NationalSplitQueueLeft.empty() && NationalSplitQueueRight.empty()) ||
            (NationalSplitQueueLeft.Length() == NationalSplitQueueRight.Length())) {
            double rnd_pct = Random() * 100;
            if (rnd_pct > 50) {
                chosen_line = GO_LEFT; // go left
            } else {
                chosen_line = GO_RIGHT; // go right
            }
        }
        // if left has fewer people waiting, choose left
        else if (NationalSplitQueueLeft.Length() < NationalSplitQueueRight.Length()) {
            chosen_line = GO_LEFT;
        } else {
            chosen_line = GO_RIGHT;
        }

        if (chosen_line == GO_LEFT) {
            // LEFT LINE
            CounterNum = -1;
            for (int i = 3; i < 3 + nationalLeft; i++) { // find free counter
                if (!NationalCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }

            if (CounterNum == -1) {                          // counters is busy, wait
                Into(NationalSplitQueueLeft);                // go into queue
                Passivate();                                 // sleep
                for (int i = 3; i < 3 + nationalLeft; i++) { // find free counter
                    if (!NationalCounter[i].Busy()) {
                        CounterNum = i;
                        break;
                    }
                }
                if (CounterNum == -1) { // should not occur
                    printf("HELP N\n");
                }
            }

            Seize(NationalCounter[CounterNum]); // start service
            NationalWaitingTableLeft(Time - ArrivalTime);

            ServiceTime = Time;
            Wait(service_time);
            NationalServiceLeft(Time - ServiceTime);
            Release(NationalCounter[CounterNum]); // end service

            if (!NationalSplitQueueLeft.empty()) { // wake up next customer
                CustomerNational *z = (CustomerNational *)(NationalSplitQueueLeft.front());
                z->Out();      // remove z from queue
                z->Activate(); // wake-up
            }
        } else {
            // RIGHT LINE
            CounterNum = -1;
            for (int i = 0; i < nationalRight; i++) { // find free counter
                if (!NationalCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }

            if (CounterNum == -1) {
                Into(NationalSplitQueueRight); // go into queue
                Passivate();                   // sleep

                for (int i = 0; i < nationalRight; i++) { // find free counter
                    if (!NationalCounter[i].Busy()) {
                        CounterNum = i;
                        break;
                    }
                }
                if (CounterNum == -1) { // should not occur
                    printf("HELP N\n");
                }
            }

            Seize(NationalCounter[CounterNum]); // start service
            NationalWaitingTableRight(Time - ArrivalTime);

            ServiceTime = Time;
            Wait(service_time);
            NationalServiceRight(Time - ServiceTime);
            Release(NationalCounter[CounterNum]); // end service

            if (!NationalSplitQueueRight.empty()) { // wake up next customer
                CustomerNational *z = (CustomerNational *)(NationalSplitQueueRight.front());
                z->Out();      // remove z from queue
                z->Activate(); // wake-up
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
        // determine how long will it take to take care of the customer
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

        // check for empty counter
        CounterNum = -1;
        for (int i = 0; i < international; i++) { // find free counter
            if (!InterCounter[i].Busy()) {
                CounterNum = i;
                break;
            }
        }

        if (CounterNum == -1) { // counters is busy, wait
            Into(InterQueue);   // go into queue
            Passivate();        // sleep
            for (int i = 0; i < international; i++) {
                if (!InterCounter[i].Busy()) {
                    CounterNum = i;
                    break;
                }
            }
            if (CounterNum == -1) { // should not occur
                printf("HELP I\n");
            }
        }
        Seize(InterCounter[CounterNum]); // start service
        InterWaitingTable(Time - ArrivalTime);

        ServiceTime = Time;
        Wait(service_time);
        InterService(Time - ServiceTime);
        Release(InterCounter[CounterNum]); // end service

        if (!InterQueue.empty()) { // wake up next customer
            CustomerInter *z = (CustomerInter *)(InterQueue.front());
            z->Out();      // remove z from queue
            z->Activate(); // wake-up
        }
    }

public:
    CustomerInter() { Activate(); }
}; // Customer

class BaseGenerator : public Event { // generator of customers for the base
    void Behavior() {                // --- customer behavior ---
        double rnd_pct = Random() * 100;
        if (rnd_pct > COUNTER_PCT) {
            NationalArrivalTable(Time);
            new CustomerNational; // create new customer for base model
        } else {
            InterArrivalTable(Time);
            new CustomerInter; // create new customer
        }
        Activate(Time + Exponential(ARRIVAL_TIME));
        ArrivalTable(Time);
    }

public:
    BaseGenerator() {
        Activate();
    }
};

class LineGenerator : public Event { // generator of customers for the line divider
    void Behavior() {                // --- customer behavior ---
        double rnd_pct = Random() * 100;
        if (rnd_pct > COUNTER_PCT) {
            NationalArrivalTable(Time);
            new CustomerNationalTwoLines; // create new customer for model with two lines
        } else {
            InterArrivalTable(Time);
            new CustomerInter; // create new customer
        }
        Activate(Time + Exponential(ARRIVAL_TIME));
        ArrivalTable(Time);
    }

public:
    LineGenerator() {
        Activate();
    }
};

void stdout_help(){
    std::cout << "\nPoužití: [-i] [-n] [-t] [-p] [-b] [-d] [-l] [-r] [-s]\n";
    std::cout << "\n [-i] [--international]\n";
    std::cout << "\tPočet otevřených mezinárodních přepážek. Povinný argument.\n\tHodnota povinná v intervalu<1,4>\n";
    std::cout << "\n [-n] [--national]\n";
    std::cout << "\tPočet otevřených vnitrostátních přepážek. Povinný argument.\n\tHodnota povinná v intervalu <1,6>\n";
    std::cout << "\n [-t] [--time]\n";
    std::cout << "\tSimulační čas v minutách. Implicitně nastaveno na 15 minut.\n";
    std::cout << "\n [-p] [--people]\n";
    std::cout << "\tPočet příchozích osob za simulační čas. Implicitně nastaveno na 112 osob.\n";
    std::cout << "\n [-b] [--base]\n\tZákladní model.\n";
    std::cout << "\n [-d] [--line_divider]\n";
    std::cout << "\tModel rozdělení fronty. Nutné spustit s [-l] a [-r].\n";
    std::cout << "\n [-l] [--left]\n";
    std::cout << "\tPočet přepážek vlevo. Hodnota v intervalu <1,3>\n";
    std::cout << "\n [-r] [--right]\n";
    std::cout << "\tPočet přepážek vpravo. Hodnota v intervalu <1,3>\n";
    std::cout << "\n [-s] [--skript]\n\tVypíše hodnoty pro skript k vyhodnocení statistik\n\n";
}

void argParse(int argc, char *argv[]) {
    int c;
    while ((c = getopt_long(argc, argv, short_args, long_args, NULL)) != -1) {
        switch (c) {
        case 'h':
            stdout_help();
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
        case 'l':
            nationalLeft = atoi(optarg);
            break;
        case 'r':
            nationalRight = atoi(optarg);
            break;
        case 'd':
        case 'b':
            if (model == '\0') {
                model = c;
            } else {
                std::cout << "Nespravny pocet argumentů, je možné spustit jen jeden model\n";
                std::exit(EXIT_FAILURE);
            }
            break;
        case 's':
            skript = true;
            break;
        default:
            std::cout << "Neznámý argument.\n";
            std::exit(EXIT_FAILURE);
        }
    }

    // Check if mandatory parameters are provided //TODO
    if (national < 1 || national > 6) {
        std::cout << "Počet vnitrostátních pokladen je nutné zadat. Musí také být v rozmezí 1 až 6.\n";
        std::exit(EXIT_FAILURE);
    }
    if (international < 1 || international > 4) {
        std::cout << "Počet mezinárodních pokladen je nutné zadat. Musí také být v rozmezí 1 až 4.\n";
        std::exit(EXIT_FAILURE);
    }
    if (model == 'd' && ((nationalLeft < 0 || nationalLeft > 3) || (nationalRight < 0 || nationalRight > 3))) {
        std::cout << "Při modelu [-d] je potřeba zadat i paramtery [-l] a [-r] v rozmezí 1 až 3.\n";
        std::exit(EXIT_FAILURE);
    }
}

void set_histogram() {
    InterWaitingTable.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTable.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTableRight.Init(0, SIMULATION_TIME, 1);
    NationalWaitingTableLeft.Init(0, SIMULATION_TIME, 1);
    InterArrivalTable.Init(0, SIMULATION_TIME, 1);
    NationalArrivalTable.Init(0, SIMULATION_TIME, 1);
}

void base_statistic_file() {
    Print("+---------------------- BASE MODEL ------------------------+\n");
    ArrivalTable.Output();

    Print("+--------------------- International ----------------------+\n");
    InterArrivalTable.Output();
    InterQueue.Output();
    InterWaitingTable.Output();
    // InterService.Output();
    for (int i = 0; i < international; i++) {
        std::string counterName = "International Counter " + std::to_string(i);
        InterCounter[i].SetName(counterName.c_str());
        InterCounter[i].Output();
    }

    Print("+------------------------ National ------------------------+\n");
    NationalArrivalTable.Output();
    NationalQueue.Output();
    NationalWaitingTable.Output();
    // NationalService.Output();
    for (int i = 0; i < national; i++) {
        std::string counterName = "National Counter " + std::to_string(i);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].Output();
    }
}

void base_statistic_script() {
    for (int i = 0; i < international; i++) {
        std::string counterName = "IC" + std::to_string(i);
        InterCounter[i].SetName(counterName.c_str());
        InterCounter[i].MyOutput();
    }
    for (int i = 0; i < national; i++) {
        std::string counterName = "NC" + std::to_string(i);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].MyOutput();
    }

    InterWaitingTable.SetName("IW");
    InterWaitingTable.MyOutput();
    NationalWaitingTable.SetName("NW");
    NationalWaitingTable.MyOutput();
}

void line_statistic_file() {
    Print("+------------------- LINE DIVIDER MODEL -------------------+\n");
    ArrivalTable.Output();

    Print("+--------------------- International ----------------------+\n");
    InterArrivalTable.Output();
    InterQueue.Output();
    InterWaitingTable.Output();
    // InterService.Output();
    for (int i = 0; i < international; i++) {
        std::string counterName = "International Counter " + std::to_string(i);
        InterCounter[i].SetName(counterName.c_str());
        InterCounter[i].Output();
    }

    Print("+------------------------ National ------------------------+\n");
    NationalArrivalTable.Output();
    Print("+-------------------------- Left --------------------------+\n");
    NationalSplitQueueLeft.Output();
    NationalWaitingTableLeft.Output();
    for (int i = 3; i < 3 + nationalLeft; i++) {
        std::string counterName = "National Counter Left " + std::to_string(i);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].Output();
    }
    Print("+-------------------------- Right -------------------------+\n");
    NationalSplitQueueRight.Output();
    NationalWaitingTableRight.Output();
    // NationalService.Output();
    for (int i = 0; i < nationalRight; i++) {
        std::string counterName = "National Counter Right " + std::to_string(i);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].Output();
    }
}

void line_statistic_script() {
    // for (int i = 0; i < international; i++) {
    //     std::string counterName = "IC" + std::to_string(i);
    //     InterCounter[i].SetName(counterName.c_str());
    //     InterCounter[i].MyOutput();
    // }
    for (int i = 0; i < nationalRight; i++) {
        std::string counterName = "RNC" + std::to_string(i);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].MyOutput();
    }
    for (int i = 3; i < (3 + nationalLeft); i++) {
        std::string counterName = "LNC" + std::to_string(i-3);
        NationalCounter[i].SetName(counterName.c_str());
        NationalCounter[i].MyOutput();
    }

    // InterWaitingTable.SetName("IW");
    // InterWaitingTable.MyOutput();
    NationalWaitingTableLeft.SetName("LNW");
    NationalWaitingTableLeft.MyOutput();
    NationalWaitingTableRight.SetName("RNW");
    NationalWaitingTableRight.MyOutput();
}

int main(int argc, char *argv[]) {
    RandomSeed(time(nullptr)); // different outputs
    argParse(argc, argv);      // analysis of arguments

    // set histogram
    set_histogram();

    if (model == 'b') {
        SetOutput("base.out");

        Init(0, SIMULATION_TIME); // init experiment
        new BaseGenerator;        // create and activate generator

        Run(); // simulation run

        base_statistic_file();   // output to file

        if (skript)
        {
            base_statistic_script(); // skript need this output
        }

    } else if (model == 'd') {
        SetOutput("line_divider.out");

        Init(0, SIMULATION_TIME); // init experiment
        new LineGenerator;        // create and activate generator

        Run(); // simulation run

        line_statistic_file();   // output to file

        if (skript){
            line_statistic_script(); // skript need this output
        }

    } else {
        printf("Nebyl spuštěn žádný model. Potřeba argumentu -b nebo -l\n");
    }
    return 0;
}
/*** End of file main.cpp ***/