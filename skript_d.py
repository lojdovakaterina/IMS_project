import os
import re

def calculate_avgs(file_path, filename):
    with open(file_path, 'r') as file:
        data = file.read()

    # var initialisation
    total_rnw = 0
    total_lnw = 0

    total_rnc = {}
    total_lnc = {}

    count_rnw = 0  
    count_lnw = 0  
    count_rnc = {}
    count_lnc = {}

    lines = data.split('\n')

    for line in lines :

        if line.startswith('RNW'):
            rnw_value = float(line.split()[1])
            total_rnw += rnw_value
            count_rnw += 1
        
        elif line.startswith('LNW'):
            lnw_value = float(line.split()[1])
            total_lnw += lnw_value
            count_lnw += 1

        elif line.startswith('RNC'):
            rnc_number = line.split()[0][2:]  # Extract the number after 'nc'
            rnc_value = float(line.split()[1])

            # Initialize counter for this nc number if not present
            if rnc_number not in count_rnc:
                count_rnc[rnc_number] = 0

            total_rnc[rnc_number] = total_rnc.get(rnc_number, 0) + rnc_value
            count_rnc[rnc_number] += 1
        
        elif line.startswith('LNC'):
            lnc_number = line.split()[0][2:]  # Extract the number after 'nc'
            lnc_value = float(line.split()[1])

            # Initialize counter for this nc number if not present
            if lnc_number not in count_lnc:
                count_lnc[lnc_number] = 0

            total_lnc[lnc_number] = total_lnc.get(lnc_number, 0) + lnc_value
            count_lnc[lnc_number] += 1

    avg_rnw = total_rnw / count_rnw if count_rnw > 0 else None
    avg_lnw = total_lnw / count_lnw if count_lnw > 0 else None

    result_string = f"----------------- File: {filename} -----------------\n"

    total_avg_rnc = 0       
    total_avg_lnc = 0       
    total_count_rnc = 0       
    total_count_lnc = 0       

    result_string += "-----------------\n"
    for n, count in count_rnc.items():
        avg_rnc = total_rnc[n] / count
        result_string += f"RNC{n}: {avg_rnc*100:.2f} %\n"
        total_avg_rnc += total_rnc[n]
        total_count_rnc += count
    
    for n, count in count_lnc.items():
        avg_lnc = total_lnc[n] / count
        result_string += f"LNC{n}: {avg_lnc*100:.2f} %\n"
        total_avg_lnc += total_lnc[n]
        total_count_lnc += count

    result_string += "-----------------\n"
    total_avg_rnc = total_avg_rnc / total_count_rnc
    total_avg_lnc = total_avg_lnc / total_count_lnc

    result_string += f"RNC: {total_avg_rnc*100:.2f} %\n"
    result_string += f"LNC: {total_avg_lnc*100:.2f} %\n"
    result_string += f"RNW: {avg_rnw:.2f} s\n"
    result_string += f"LNW: {avg_lnw:.2f} s\n"
    return result_string


# set file path
script_directory = os.path.dirname(os.path.abspath(__file__))
directory_path = out_directory = os.path.join(script_directory, 'line_out')

# go through all files in folder
for filename in os.listdir(directory_path):
    if filename.startswith("line") and filename.endswith(".out"):
        file_path = os.path.join(directory_path, filename)

        # count averages from one file
        result = calculate_avgs(file_path, filename)

        # append to file
        output_file = file_path = os.path.join(directory_path, "statistic.out")
        with open(file_path, 'a') as file:
            file.write(result)
