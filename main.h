/**
 * @file main.h
 * @author Andrea Michlíková, Kateřina Lojdová
 * @brief
 */
#ifndef IMS_MAIN_H
#define IMS_MAIN_H

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <simlib.h>
#include <stdio.h>
#include <sstream>

#define SIMULATION_TIME (sim_time)
#define ARRIVAL_TIME ((sim_time) / people)
#define COUNTER_PCT 37.5 // menší hodnota přepážky procenta

#define GO_LEFT 0
#define GO_RIGHT 1

int national = 0;
int international = 0;
int sim_time = 15 * 60; // 15 minut
int people = 112;       // 112 lidí
int nationalLeft = 0;
int nationalRight = 0;

class MyHistogram : public Histogram {
public:
    MyHistogram(const char *title, double min, double max, double width) : Histogram(title, min, max, width) {}

    void MyOutput() const {
        if (stat.Number() == 0)
            printf("%s 0\n", Name().c_str());
        else
            printf("%s %-25g\n", Name().c_str(), stat.MeanValue());
    }
};
class MyFacility : public Facility {
public:
    MyFacility() : Facility() {}

    void MyOutput() const {
        printf("%s %-25g\n", Name().c_str(), tstat.MeanValue());
    }
};

MyHistogram InterWaitingTable("IW", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTable("NW", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTableLeft("Počty čekání vnitrostátní LEVA", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTableRight("Počty čekání vnitrostátní PRAVA", 0, SIMULATION_TIME, 1);

Histogram InterService("Obsluha mezinárodní", 0, 20, 6);
Histogram NationalService("Obsluha vnitrostátní", 0, 20, 6);
Histogram NationalServiceLeft("Obsluha vnitrostátní LEVA", 0, 20, 6);
Histogram NationalServiceRight("Obsluha vnitrostátní PRAVA", 0, 20, 6);

// Histogram ArrivalTable("Počty příchodů", 0, 60, 15);               // po minutách
// Histogram InterArrivalTable("Počty příchodů mezinárodní", 0, 60, 15);  // po minutách
// Histogram NationalArrivalTable("Počty příchodů vnitrostátní", 0, 60, 15); // po minutách
Histogram ArrivalTable("Počty příchodů", 0, SIMULATION_TIME, 1);               // po minutách
Histogram InterArrivalTable("Počty příchodů mezinárodní", 0, SIMULATION_TIME, 1);     // po minutách
Histogram NationalArrivalTable("Počty příchodů vnitrostátní", 0, SIMULATION_TIME, 1); // po minutách

MyFacility InterCounter[4];
Queue InterQueue("Čekání Mezinárodní");
MyFacility NationalCounter[6];
Queue NationalQueue("Čekání Vnitrostátní");

Queue NationalSplitQueueLeft("Čekání Vnitrostátní Leva");
Queue NationalSplitQueueRight("Čekání Vnitrostátní Prava");

std::string args = "xxxx"; // možné modely
const char *short_args = "hi:n:t:p:bswl:";
struct option long_args[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"international", required_argument, NULL, 'i'},
        {"national", required_argument, NULL, 'n'},
        {"time", required_argument, NULL, 't'},
        {"people", required_argument, NULL, 'p'},
        {"base", no_argument, NULL, 'b'},
        {"self_checkout", no_argument, NULL, 's'},
        {"side_window", no_argument, NULL, 'w'},
        {"line_divider", required_argument, NULL, 'l'},
        {0, 0, 0, 0} // ukoncovaci prvek
};

#endif // IMS_MAIN_H