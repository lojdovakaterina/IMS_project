/**
 * @file main.cpp
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

#define SIMULATION_TIME (sim_time)
#define ARRIVAL_TIME ((sim_time) / people)
#define COUNTER_PCT 37.5 // menší hodnota přepážky procenta

int national = 0;
int international = 0;
int sim_time = 15 * 60; // 15 minut
int people = 112;       // 112 lidí

// Histogram NationalWaitingTable("Počty čekání vnitrostátní", 0, 5, 20);
// Histogram InterWaitingTable("Počty čekání mezinárodní", 0, 5, 20);
Histogram NationalWaitingTable("Počty čekání vnitrostátní", 0, SIMULATION_TIME, 1);
Histogram InterWaitingTable("Počty čekání mezinárodní", 0, SIMULATION_TIME, 1);

Histogram NationalService("Obsluha vnitrostátní", 0, 20, 6);
Histogram InterService("Obsluha mezinárodní", 0, 20, 6);

// Histogram ArrivalTable("Počty příchodů", 0, 60, 15);               // po minutách
// Histogram NArrivalTable("Počty příchodů vnitrostátní", 0, 60, 15); // po minutách
// Histogram IArrivalTable("Počty příchodů mezinárodní", 0, 60, 15);  // po minutách
Histogram ArrivalTable("Počty příchodů", 0, SIMULATION_TIME, 1);               // po minutách
Histogram NArrivalTable("Počty příchodů vnitrostátní", 0, SIMULATION_TIME, 1); // po minutách
Histogram IArrivalTable("Počty příchodů mezinárodní", 0, SIMULATION_TIME, 1);  // po minutách

Facility InterCounter[4];
Queue InterQueue("Čekání Mezinárodní");
Facility NationalCounter[6];
Queue NationalQueue("Čekání Vnitrostátní");

std::string args = "xxxx"; // možné modely
const char *short_args = "hi:n:t:p:bswl";
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
        {"line_divider", no_argument, NULL, 'l'},
        {0, 0, 0, 0} // ukoncovaci prvek
};

#endif // IMS_MAIN_H