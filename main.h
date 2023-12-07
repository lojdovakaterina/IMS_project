/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef IMS_MAIN_H
#define IMS_MAIN_H

#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <simlib.h>
#include <stdio.h>

#define SECOND *c_SECOND
#define MINUTE *c_MINUTE
#define HOUR *c_HOUR
#define DAY *c_DAY

const unsigned long long c_SECOND = 1;
const unsigned long long c_MINUTE = c_SECOND * 60;
const unsigned long long c_HOUR = c_MINUTE * 60;
const unsigned long long c_DAY = c_HOUR * 24;

int national = 0;
int international = 0;

Store InterCounter("Mezinárodní přepážky", 0);
Store NationalCounter("Vnitrostátní přepážky", 0);

Histogram Table("Table", 0, 25, 20);

std::string args = "xxxx"; // možné modely
const char* short_args = "hi:n:bswl";
struct option long_args[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"international", required_argument, NULL, 'i'},
        {"national", required_argument, NULL, 'n'},
        {"base", no_argument, NULL, 'b'},
        {"self_checkout", no_argument, NULL, 's'},
        {"side_window", no_argument, NULL, 'w'},
        {"line_divider", no_argument, NULL, 'l'},
        {0, 0, 0, 0} // ukoncovaci prvek
};

unsigned long long simulation_time = 1 DAY;

#endif // IMS_MAIN_H