#!/usr/bin/env python3

import re
import csv
import argparse

regex_patterns = {
    'its_clusters': r'ITSTracker pulled (\d+) clusters, \d+ RO frames',
    'bc_per_rof': r'ITSAlpideParam.roFrameLengthInBC : (\d+)',
    'ro_frames': r'ITSTracker pulled \d+ clusters, (\d+) RO frames',
    'its_tracks': r'ITSTracker pushed (\d+) tracks and \d+ vertices',
    'its_vertices': r'ITSTracker pushed \d+ tracks and (\d+) vertices',
    'gpu_time_cpu': r'GPU Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'gpu_time_wall': r'GPU Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s \(wall\)',
    'cpu_time_cpu': r'CPU Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'cpu_time_wall': r'CPU Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s \(wall\)',
}

def process_log(logfile, output):
    """Process the log file and extract all occurrences of the desired values."""
    all_data = []
    with open(logfile, 'r') as file:
        log_lines = file.readlines()
        extracted_data = []
        for line in log_lines:
            line_data = []
            for key, pattern in regex_patterns.items():
                match = re.search(pattern, line)
                if match:
                    line_data.append(match.group(1))
                else:
                    line_data.append("NA")
            if any(value != "NA" for value in line_data):
                all_data.append(line_data)

    with open(output, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=' ')
        csvwriter.writerow(regex_patterns.keys())
        csvwriter.writerows(all_data)

    print(f"Done. Check '{output}' for the results.")

def main():
    parser = argparse.ArgumentParser(description="Process a log file and extract relevant data.")
    parser.add_argument('logfile', help="The path to the log file to process")
    args = parser.parse_args()
    process_log(args.logfile, args.logfile.replace(".log",".dat"))

if __name__ == '__main__':
    main()

