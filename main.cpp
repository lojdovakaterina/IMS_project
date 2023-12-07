/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"

class NationalConnections : public Process {
    double ArrivalTime;
    void Behavior() {
        ArrivalTime = Time;
        Enter(NationalCounter, 1);
        Wait(Exponential(37.13)); // u pokladdny ... exponenciálně?
        Leave(NationalCounter, 1);
        Table(Time - ArrivalTime);
    }
};

class InternationalConnections : public Process {
    double ArrivalTime;
    void Behavior() {
        ArrivalTime = Time;
        Enter(InterCounter, 1);
        Wait(Exponential(43.58)); // u pokladny ... exponenciálně?
        Leave(InterCounter, 1);
        Table(Time - ArrivalTime);
    }
};

class Customer : public Process {
    double ArrivalTime;
    void Behavior() { // --- behavoir specification ---
        ArrivalTime = Time;   // incoming time
        Wait(180); // 3 minuty ... příchod do systému? ten první čtvereček :D
        double p = Uniform(0, 100);
        if (p > 45.39) {
            (new NationalConnections)->Activate();
        } else {
            (new InternationalConnections)->Activate();
        }
        Table(Time - ArrivalTime); // waiting and service time
    }
};

class Generator : public Event {         // generátor zákazníků
    void Behavior() {                    // --- popis chování  generátorů ---
        (new Customer)->Activate();      // nový zákazník, aktivace v čase time
        Activate(Time + Exponential(5)); // příchod cestujících do systému TODO
    }
};

void argParse(int argc, char *argv[]) {
    int c;
    while ((c = getopt_long(argc, argv, short_args, long_args, NULL)) != -1) {
        switch (c) {
        case 'h':
            printf("Použití: [-i] <hodnota> [-n] <hodnota> [-b] [-sd] [-ld] [-sc]\n" // TODOO
                   "[-i] [--international] Počet otevřených mezinárodních přepážek\n"
                   "[-n] [--national] Počet otevřených vnitrostátních přepážek\n"
                   "[-b] [--base] Základní model\n"
                   "[-s] [self_checkout] Model přidání automatu na jízdenky\n"
                   "[-w] [side_window] Model otevření postraní přepážky\n"
                   "[-l] [line_divider] Model rozdělení fronty\n"
                   "Parametry [international] [national] jsou povinné a zadávají se v intervalu <1,6>\n");
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
                std::cout << "Vícekrát zadán stejný argument." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "Neznámý argument `-%c'.\n", optopt);
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
    argParse(argc, argv);
    NationalCounter.SetCapacity(national);
    InterCounter.SetCapacity(international);
    int c;
    int i = 0;
    while ((c = args[i++]) != '\0') {
        switch (c) {
        case 'b':
            SetOutput("base.out");
            printf("Začátek simulace...\n");
            Init(0, (double)simulation_time);
            (new Generator)->Activate(); // customer generator
            Run();                       // simulation
            NationalCounter.Output();
            InterCounter.Output();
            Table.Output();
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