////////////////////////////////////////////////////////////////////////////
// main.cpp                SIMLIB/C++
//
// Bla TODO
//

#include <cstdlib>
#include <cstring>
#include <simlib.h>

int national = 0;
int international = 0;
bool base = false;
bool side_window = false;
bool line_divide = false;
bool self_checkout = false;

#define WAIT_QUEUE 3

Store pokladny("Pokladny", 0);

// global objects:
// Facility Box("Box");
Histogram Table("Table", 0, 25, 20);


class NationalConnections : public Process
{
    void Behavior()
    {
        Enter(pokladny, 1);
        Wait(Exponential(1)); // u pokladny // 37.13 s TODO
        Leave(pokladny, 1);
    }
};

class InternationalConnections : public Process
{
    void Behavior()
    { 
        Enter(pokladny, 1);
        Wait(Exponential(2)); // u pokladny // 43.58 s TODO
        Leave(pokladny, 1);
    }
};

class Customer : public Process
{

    // double Prichod; // atribute of each customer
    void Behavior()
    {                     // --- behavoir specification ---
        // Prichod = Time;   // incoming time
        // Seize(Box);       // start of service
        Wait(WAIT_QUEUE); // time of service       //TODO vteriny?
        double p = Uniform(0, 100);
        if (p > 45.39)
        {
            (new NationalConnections)->Activate();
        }
        else
        {
            (new InternationalConnections)->Activate();
        }
        // Release(Box);          // end of service
        // Table(Time - Prichod); // waiting and service time
    }
};

class Generator : public Event
{ // generátor zákazníků
    void Behavior()
    {                                    // --- popis chování  generátorů ---
        (new Customer)->Activate();      // nový zákazník, aktivace v čase time
        Activate(Time + Exponential(5)); // příchod cestujících do systému TODO
    }
};

void argParse(int argc, char *argv[])
{
    if ((argc == 2 && (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)) || argc < 5)
    {
        printf("Použití: [-i] international [-n] national [-b] [-sd] [-ld] [-sc]\n" // TODOO
               "[-i] Počet otevřených mezinárodních přepážek\n"
               "[-n] Počet otevřených vnitrostátních přepážek\n"
               "[-b] Základní model\n"
               "[-sd] Model otevření postraní přepážky\n"
               "[-ld] Model rozdělení fronty\n"
               "[-sc] Model přidání automatu na jízdenky\n"
               "Parametry [international] [national] jsou povinné a zadávají se v intervalu <1,6>\n");
        std::exit(EXIT_SUCCESS);
    }

    int arg = 0;
    while (++arg != argc)
    {
        if (!strcmp("-i", argv[arg]))
        {
            international = atoi(argv[++arg]);
        }
        else if (!strcmp("-n", argv[arg]))
        {
            national = atoi(argv[++arg]);
        }
        else if (!strcmp("-b", argv[arg]))
        {
            base = true;
        }
        else if (!strcmp("-sd", argv[arg]))
        {
            side_window = true;
        }
        else if (!strcmp("-ld", argv[arg]))
        {
            line_divide = true;
        }
        else if (!strcmp("-sc", argv[arg]))
        {
            self_checkout = true;
        }
        else
        {
            printf("Špatně zadaný parametr\n\n"
                   "Použití: [-i] international [-n] national [-b] [-sd] [-ld] [-sc]\n" // TODOO
                   "[-i] Počet otevřených mezinárodních přepážek\n"
                   "[-n] Počet otevřených vnitrostátních přepážek\n"
                   "[-b] Základní model\n"
                   "[-sd] Model otevření postraní přepážky\n"
                   "[-ld] Model rozdělení fronty\n"
                   "[-sc] Model přidání automatu na jízdenky\n"
                   "Parametry [international] [national] jsou povinné a zadávají se v intervalu <1,6>\n");
            std::exit(EXIT_SUCCESS);
        }
    }
    if (international == 0 || national == 0)
    {
        printf("Nutné zadat parametr -i a -n");
        std::exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    argParse(argc, argv);

    pokladny.SetCapacity(national);
    {
        printf("Začátek simulace... %ld\n", pokladny.Capacity());
        SetOutput("base.out");
        Init(0, 100);               // experiment initialization for time 0..100
        (new Generator)->Activate(); // customer generator
        Run();                       // simulation
        // Box.Output();                // print of results
        Table.Output();
        printf("Konec simulace...\n");
    }
    return 0;
}
/*** End of file main.cpp ***/