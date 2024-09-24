#!/usr/bin/env python3

import re
import csv
import argparse

# Define regular expressions for the values you want to extract
regex_patterns = {
    'its_clusters': r'ITSTracker pulled (\d+) clusters, \d+ RO frames',
    'bc_per_rof': r'ITSAlpideParam.roFrameBiasInBC : (\d+)    [ CCDB ]',
    'ro_frames': r'ITSTracker pulled \d+ clusters, (\d+) RO frames',
    'its_tracks': r'ITSTracker pushed (\d+) tracks and \d+ vertices',
    'its_vertices': r'ITSTracker pushed \d+ tracks and (\d+) vertices',
    'gpu_time_cpu': r'GPU Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'gpu_time_wall': r'GPU Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s \(wall\)',
    'cpu_time_cpu': r'CPU Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'cpu_time_wall': r'CPU Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s \(wall\)',
}

def process_log(logfile, output):
    """Process the log file and extract all occurrences of the desired values, ensuring one row per Timeframe."""
    all_data = []

    with open(logfile, 'r') as file:
        log_content = file.read()

        # Split the log by Timeframes (look for 'Timeframe' or similar markers)
        sections = re.split(r'Timeframe \d+ processing completed', log_content)

        for section in sections:
            # Dictionary to hold the data for each Timeframe
            extracted_data = {key: "NA" for key in regex_patterns.keys()}
            
            for key, pattern in regex_patterns.items():
                match = re.search(pattern, section)
                if match:
                    extracted_data[key] = match.group(1)  # Store the extracted value

            # Add the dictionary data as a row to all_data
            all_data.append(list(extracted_data.values()))

    # Write the data to a CSV file
    with open(output, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=' ')
        csvwriter.writerow(regex_patterns.keys())  # Write header
        csvwriter.writerows(all_data)  # Write all rows

    print(f"Done. Check '{output}'")

def main():
    parser = argparse.ArgumentParser(description="Process a log file and extract relevant data.")
    parser.add_argument('logfile', help="The path to the log file to process")
    args = parser.parse_args()
    process_log(args.logfile, args.logfile.replace(".log", ".dat"))

if __name__ == '__main__':
    main()
