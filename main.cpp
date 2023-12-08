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

class BaseModelCustomer : public Process {
    double ArrivalTime;
    double ServiceTime;
    void Behavior() {
        ArrivalTime = Time;
        double rnd_pct = Random() * 100;
        // Store &counter = (rnd_pct > COUNTER_PCT) ? NationalCounter : InterCounter; // ktera prepazka?

        // Enter(counter, 1);
        // (rnd_pct > COUNTER_PCT) ? NationalWaitingTable(Time - ArrivalTime) : InterWaitingTable(Time - ArrivalTime);
        // ServiceTime = Time;
        // Wait((rnd_pct > COUNTER_PCT) ? NATIONAL_SERVICE : INTER_SERVICE); // obsluha
        // (rnd_pct > COUNTER_PCT) ? NationalService(Time - ServiceTime) : InterService(Time - ServiceTime);
        // Leave(counter, 1);

        if (rnd_pct > COUNTER_PCT){
            Enter(NationalCounter, 1);
            NationalWaitingTable(Time - ArrivalTime);
            double srvc_pct = Random() * 100;

            double service_time = 0; // Default value
            if (srvc_pct >= 0 && srvc_pct <= 12.07) {
                service_time = 18; // Hodnota pro první rozsah
            } else if (srvc_pct > 12.07 && srvc_pct <= 74.14) {
                service_time = 28.25; // Hodnota pro druhý rozsah
            } else if (srvc_pct > 74.14 && srvc_pct <= 86.21) {
                service_time = 46.57; // Hodnota pro třetí rozsah
            } else if (srvc_pct > 86.21) {
                service_time = 83.38; // Hodnota pro čtvrtý rozsah
            }

            ServiceTime = Time;
            Wait(service_time); // obsluha
            NationalService(Time - ServiceTime);
            Leave(NationalCounter, 1);
        } else {
            Enter(InterCounter, 1);
            InterWaitingTable(Time - ArrivalTime);
            double srvc_pct = Random() * 100;

            double service_time = 0; // Default value
            if (srvc_pct >= 0 && srvc_pct <= 22.58) {
                service_time = 13.57; // Hodnota pro první rozsah
            } else if (srvc_pct > 22.58 && srvc_pct <= 67.74) {
                service_time = 27.89; // Hodnota pro druhý rozsah
            } else if (srvc_pct > 67.74 && srvc_pct <= 83.87) {
                service_time = 45.7; // Hodnota pro třetí rozsah
            } else if (srvc_pct > 83.87) {
                service_time = 109.1; // Hodnota pro čtvrtý rozsah
            }

            ServiceTime = Time;
            Wait(service_time); // obsluha
            InterService(Time - ServiceTime);
            Leave(InterCounter, 1);
        }
    }
};

class Generator : public Event {             // generátor zákazníků
    void Behavior() {                        // --- popis chování  generátorů ---
        (new BaseModelCustomer)->Activate(); // nový zákazník, aktivace v čase time
        Activate(Time + Exponential(ARRIVAL_TIME));
        ArrivalTable(Time);
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

    NationalCounter.SetCapacity(national);
    InterCounter.SetCapacity(international);
    RandomSeed(time(nullptr));
    int c;
    int i = 0;
    while ((c = args[i++]) != '\0') {
        switch (c) {
        case 'b':
            SetOutput("base.out");
            Print(" BASE MODEL \n");
            printf("Začátek simulace...\n");
            Init(0, (double)SIMULATION_TIME);
            (new Generator)->Activate(); // customer generator
            Run();                       // simulation
            NationalService.Output();
            InterService.Output();
            InterWaitingTable.Output();
            NationalWaitingTable.Output();
            ArrivalTable.Output();
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