import os
import re

def calculate_avgs(file_path, filename):
    with open(file_path, 'r') as file:
        data = file.read()

    # var initialisation
    total_iw = 0
    total_nw = 0

    total_ic = {}
    total_nc = {}

    count_iw = 0
    count_nw = 0
    count_ic = {}   
    count_nc = {}

    lines = data.split('\n')

    for line in lines :
        if line.startswith('IW'):
            iw_value = float(line.split()[1])
            total_iw += iw_value
            count_iw += 1

        elif line.startswith('NW'):
            nw_value = float(line.split()[1])
            total_nw += nw_value
            count_nw += 1

        elif line.startswith('IC'):
            ic_number = line.split()[0][2:]  # Extract the number after 'ic'
            ic_value = float(line.split()[1])

            # Initialize counter for this ic number if not present
            if ic_number not in count_ic:
                count_ic[ic_number] = 0

            total_ic[ic_number] = total_ic.get(ic_number, 0) + ic_value
            count_ic[ic_number] += 1

        elif line.startswith('NC'):
            nc_number = line.split()[0][2:]  # Extract the number after 'nc'
            nc_value = float(line.split()[1])

            # Initialize counter for this nc number if not present
            if nc_number not in count_nc:
                count_nc[nc_number] = 0

            total_nc[nc_number] = total_nc.get(nc_number, 0) + nc_value
            count_nc[nc_number] += 1

    avg_iw = total_iw / count_iw if count_iw > 0 else None
    avg_nw = total_nw / count_nw if count_nw > 0 else None

    result_string = f"----------------- File: {filename} -----------------\n"

    total_avg_ic = 0        
    total_avg_nc = 0       
    total_count_ic = 0        
    total_count_nc = 0       
    for i, count in count_ic.items():
        avg_ic = total_ic[i] / count
        result_string += f"IC{i}: {avg_ic*100:.2f} %\n"
        total_avg_ic += total_ic[i]
        total_count_ic += count

    result_string += f"-----------------\n"
    for n, count in count_nc.items():
        avg_nc = total_nc[n] / count
        result_string += f"NC{n}: {avg_nc*100:.2f} %\n"
        total_avg_nc += total_nc[n]
        total_count_nc += count

    result_string += f"-----------------\n"
    total_avg_ic = total_avg_ic / total_count_ic
    total_avg_nc = total_avg_nc / total_count_nc

    result_string += f"IC: {total_avg_ic*100:.2f} %\n"
    result_string += f"NC: {total_avg_nc*100:.2f} %\n"
    result_string += f"IW: {avg_iw:.2f} s\n"
    result_string += f"NW: {avg_nw:.2f} s\n"
    return result_string


# set file path
script_directory = os.path.dirname(os.path.abspath(__file__))
directory_path = out_directory = os.path.join(script_directory, 'out')

# go through all files in folder
for filename in os.listdir(directory_path):
    if filename.startswith("base") and filename.endswith(".out"):
        file_path = os.path.join(directory_path, filename)

        # count averages from one file
        result = calculate_avgs(file_path, filename)

        # append to file
        output_file = file_path = os.path.join(directory_path, "statistic.out")
        with open(file_path, 'a') as file:
            file.write(result)
