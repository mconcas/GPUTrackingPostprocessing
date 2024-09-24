#!/usr/bin/env python3

import re
import csv
import argparse

# Define regular expressions for the values you want to extract
regex_patterns = {
    'its_clusters': r'ITSTracker pulled (\d+) clusters, \d+ RO frames',
    'bc_per_rof': r'ITSAlpideParam.roFrameLengthInBC : (\d+)',
    'ro_frames': r'ITSTracker pulled \d+ clusters, (\d+) RO frames',
    'its_tracks': r'ITSTracker pushed (\d+) tracks and \d+ vertices',
    'its_vertices': r'ITSTracker pushed \d+ tracks and (\d+) vertices',
    'gpu_time_cpu': r'GPU Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'gpu_time_wall': r'GPU Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s \(wall\)',
}

def process_log(logfile, output):
    """Process the log file and extract all occurrences of the desired values."""
    all_data = []
    with open(logfile, 'r') as file:
        log_content = file.read()
        for match_set in re.finditer(r'\*', log_content):
            extracted_data = []
            log_slice = log_content[match_set.start():]
            for key, pattern in regex_patterns.items():
                matches = re.search(pattern, log_slice)
                if matches:
                    extracted_data.append(matches.group(1))
                else:
                    extracted_data.append("NA")
            all_data.append(extracted_data)
    with open(output, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=' ')
        csvwriter.writerow(regex_patterns.keys())
        csvwriter.writerows(all_data)

    print(f"Done. Check '{output}'")

def main():
    parser = argparse.ArgumentParser(description="Process a log file and extract relevant data.")
    parser.add_argument('logfile', help="The path to the log file to process")
    args = parser.parse_args()
    process_log(args.logfile, args.logfile.replace(".log",".dat"))

if __name__ == '__main__':
    main()
