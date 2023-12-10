/**
 * @file main.h
 * @author Andrea Michlíková, Kateřina Lojdová
 * @brief IMS
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

#define SIMULATION_TIME (sim_time)
#define ARRIVAL_TIME ((sim_time) / people)
#define COUNTER_PCT 37.5 // 37.5 percent people goes to international 

#define GO_LEFT 0
#define GO_RIGHT 1

int national = 0;
int international = 0;
int sim_time = 15 * 60; // 15 minutes
int people = 112;       // 112 people
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

MyHistogram InterWaitingTable("International Waiting Table", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTable("National Waiting Table", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTableLeft("Left National Waiting Table", 0, SIMULATION_TIME, 1);
MyHistogram NationalWaitingTableRight("Right National Waiting Table", 0, SIMULATION_TIME, 1);

Histogram InterService("International Service", 0, 20, 6);
Histogram NationalService("National Service", 0, 20, 6);
Histogram NationalServiceLeft("Left National Service", 0, 20, 6);
Histogram NationalServiceRight("Right National Service", 0, 20, 6);

// Histogram ArrivalTable("Arrival Table", 0, 60, 15);                    // for minutes
// Histogram InterArrivalTable("International Arrival Table", 0, 60, 15); // for minutes
// Histogram NationalArrivalTable("National ArrivalTable", 0, 60, 15);    // for minutes

Histogram ArrivalTable("Arrival Table", 0, SIMULATION_TIME, 1);                    
Histogram InterArrivalTable("International Arrival Table", 0, SIMULATION_TIME, 1); 
Histogram NationalArrivalTable("National ArrivalTable", 0, SIMULATION_TIME, 1);    

MyFacility InterCounter[4];
Queue InterQueue("International Queue");
MyFacility NationalCounter[6];
Queue NationalQueue("National Queue");

Queue NationalSplitQueueLeft("Left National Queue");
Queue NationalSplitQueueRight("Right National Queue");

bool skript = false;
char model = '\0';
const char *short_args = "hi:n:t:p:bdl:r:s";
struct option long_args[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"international", required_argument, NULL, 'i'},
        {"national", required_argument, NULL, 'n'},
        {"time", required_argument, NULL, 't'},
        {"people", required_argument, NULL, 'p'},
        {"base", no_argument, NULL, 'b'},
        {"line_divider", required_argument, NULL, 'd'},
        {"left", required_argument, NULL, 'l'},
        {"right", required_argument, NULL, 'r'},
        {"skript", required_argument, NULL, 's'},
        {0, 0, 0, 0} // ukoncovaci prvek
};

#endif // IMS_MAIN_H