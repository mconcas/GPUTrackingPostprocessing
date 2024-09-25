#!/usr/bin/env python3

import re
import csv
import argparse

# Define regular expressions for the values you want to extract
regex_patterns = {
    'bc_per_rof': r'ITSAlpideParam\.roFrameLengthInBC\s*:\s*(\d+)',
    'its_clusters': r'ITSTracker pulled (\d+) clusters, \d+ RO frames',
    'ro_frames': r'ITSTracker pulled \d+ clusters, (\d+) RO frames',
    'its_tracks': r'ITSTracker pushed (\d+) tracks and \d+ vertices',
    'its_vertices': r'ITSTracker pushed \d+ tracks and (\d+) vertices',
    'fitting_time': r'Parallel fit took: ([\d.]+) ms',
    'tf_time_cpu': r'(?:GPU|CPU) Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'tf_time_wall': r'(?:GPU|CPU) Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s',
}

def process_log(logfile, output):
    all_data = []
    tf_count = 0
    bc_per_rof_value = None  # To store the persistent bc_per_rof value

    with open(logfile, 'r') as file:
        log_content = file.read()
        sections = re.split(r'\(wall\)', log_content)
        sections.pop()  # Last token contains garbage

        for section in sections:
            extracted_data = {key: "NA" for key in regex_patterns.keys()}

            # Check and persist bc_per_rof value
            if not bc_per_rof_value:
                bc_match = re.search(regex_patterns['bc_per_rof'], section)
                if bc_match:
                    bc_per_rof_value = bc_match.group(1)
            if bc_per_rof_value:
                extracted_data['bc_per_rof'] = bc_per_rof_value

            # Collect and sum all fitting times in the section
            fitting_times = re.findall(regex_patterns['fitting_time'], section)
            if fitting_times:
                fitting_time_sum = sum(float(time) for time in fitting_times)
                extracted_data['fitting_time'] = f"{fitting_time_sum:.3f}"

            # Extract all other data points
            for key, pattern in regex_patterns.items():
                if key != 'fitting_time' and key != 'bc_per_rof':  # Already handled these
                    match = re.search(pattern, section)
                    if match:
                        extracted_data[key] = f"{float(match.group(1)):.3f}" if '.' in match.group(1) else match.group(1)

            all_data.append(list(extracted_data.values()))
            tf_count += 1

    # Write the CSV with formatted data
    with open(output, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter='\t')
        csvwriter.writerow(regex_patterns.keys())
        csvwriter.writerows(all_data)

    print(f"processed '{tf_count} TFs'. Check '{output}'")

def main():
    parser = argparse.ArgumentParser(description="Process a log file and extract relevant data.")
    parser.add_argument('logfile', help="The path to the log file to process")
    args = parser.parse_args()
    process_log(args.logfile, args.logfile.replace(".log", ".dat"))

if __name__ == '__main__':
    main()