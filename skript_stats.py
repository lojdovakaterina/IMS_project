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

    # extract file number to get the experiment numbers
    filenum = re.match(r"base_(\d+).out", filename).group(1)
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

            # Initialize counter for ic if not present
            if ic_number not in count_ic:
                count_ic[ic_number] = 0

            total_ic[ic_number] = total_ic.get(ic_number, 0) + ic_value
            count_ic[ic_number] += 1

        elif line.startswith('NC'):
            nc_number = line.split()[0][2:]  # Extract the number after 'nc'
            nc_value = float(line.split()[1])

            # Initialize counter for nc if not present
            if nc_number not in count_nc:
                count_nc[nc_number] = 0

            total_nc[nc_number] = total_nc.get(nc_number, 0) + nc_value
            count_nc[nc_number] += 1

    avg_iw = total_iw / count_iw if count_iw > 0 else None
    avg_nw = total_nw / count_nw if count_nw > 0 else None

    total_avg_ic = 0        
    total_avg_nc = 0       
    total_count_ic = 0        
    total_count_nc = 0       
    for i, count in count_ic.items():
        avg_ic = total_ic[i] / count
        total_avg_ic += total_ic[i]
        total_count_ic += count

    for n, count in count_nc.items():
        avg_nc = total_nc[n] / count
        total_avg_nc += total_nc[n]
        total_count_nc += count

    total_avg_ic = total_avg_ic / total_count_ic
    total_avg_nc = total_avg_nc / total_count_nc

    result_string = f"{filenum}\t"
    result_string += f"{total_avg_ic*100:.2f}\t"
    result_string += f"{total_avg_nc*100:.2f}\t"
    result_string += f"{avg_iw:.2f}\t"
    result_string += f"{avg_nw:.2f}"

    return result_string + '\n'

# set file path
script_directory = os.path.dirname(os.path.abspath(__file__))
directory_path = out_directory = os.path.join(script_directory, 'out')

output_file = os.path.join(directory_path, "plot_inter.txt")
with open(output_file, 'a') as outfile:
    outfile.write(f"#N\tEXP\tIC\t\tNC\t\tIW\t\tNW\n")

    filenames = [filename for filename in os.listdir(directory_path) if filename.startswith("base") and filename.endswith(".out")]
    filenames.sort()

    linenum = 0
    # go through all files in folder
    for filename in filenames:
        file_path = os.path.join(directory_path, filename)
        # count averages from one file
        result = calculate_avgs(file_path, filename)
        # add line number for gnuplot
        result = f"{linenum}\t" + result
        linenum += 1
        #append to output file
        outfile.write(result)

output_file = os.path.join(directory_path, "plot_national.txt")
with open(output_file, 'a') as outfile:
    outfile.write(f"#N\tEXP\tIC\t\tNC\t\tIW\t\tNW\n")

    filenames = [filename for filename in os.listdir(directory_path) if filename.startswith("base") and filename.endswith(".out")]
    filenames.sort()
    filenames.sort(key=lambda filename: re.match(r"base_(\d)(\d).out", filename).group(2))

    linenum = 0
    # go through all files in folder
    for filename in filenames:
        file_path = os.path.join(directory_path, filename)
        # count averages from one file
        result = calculate_avgs(file_path, filename)
        # add line number for gnuplot
        result = f"{linenum}\t" + result
        linenum += 1
        #append to output file
        outfile.write(result)