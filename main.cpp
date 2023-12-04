////////////////////////////////////////////////////////////////////////////
// Model model2.cc                SIMLIB/C++
//
// Simple test model of queuing system
//

#include "simlib.h"

// global objects:
Facility Box("Box");
Histogram Table("Table", 0, 25, 20);

class Customer : public Process
{
    double Prichod; // atribute of each customer
    void Behavior()
    {                          // --- behavoir specification ---
        Prichod = Time;        // incoming time
        Seize(Box);            // start of service
        Wait(10);              // time of service
        Release(Box);          // end of service
        Table(Time - Prichod); // waiting and service time
    }
};

class Generator : public Event
{ // model of system's input
    void Behavior()
    {                                            // --- behavior specification ---
        (new Customer)->Activate();              // new customer
        Activate(Time + Exponential(1e3 / 150)); //
    }
};

int main()
{ // experiment description
    Print(" model2 - SIMLIB/C++ example\n");
    SetOutput("model2.out");
    Init(0, 1000);               // experiment initialization for time 0..1000
    (new Generator)->Activate(); // customer generator
    Run();                       // simulation
    Box.Output();                // print of results
    Table.Output();
    return 0;
}
