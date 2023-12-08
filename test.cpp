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
    void Behavior() {
        ArrivalTime = Time;
        Store &counter = (Random() * 100 > 45.39) ? NationalCounter : InterCounter; // ktera prepazka?

        Enter(counter, 1);
        Wait((Random() * 100 > 42.31) ? 45 : 15); // cekani i platby kartou a hotove
        Leave(counter, 1);

        Table(Time - ArrivalTime);
    }
};

class Generator : public Event {             // generátor zákazníků
    void Behavior() {                        // --- popis chování  generátorů ---
        (new BaseModelCustomer)->Activate(); // nový zákazník, aktivace v čase time
        Activate(Time + Exponential(4.62));  // příchod cestujících do systému TODO
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
    argParse(argc, argv);
    NationalCounter.SetCapacity(national);
    InterCounter.SetCapacity(international);
    int c;
    int i = 0;
    while ((c = args[i++]) != '\0') {
        switch (c) {
        case 'b':
            SetOutput("base.out");
            Print(" BASE MODEL \n");
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