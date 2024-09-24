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
    'tf_time_cpu': r'(?:GPU|CPU) Reconstruction time for this TF ([\d.]+) s \(cpu\)',
    'tf_time_wall': r'(?:GPU|CPU) Reconstruction time for this TF [\d.]+ s \(cpu\), ([\d.]+) s',
}

def process_log(logfile, output):
    all_data = []
    tf_count = 0
    with open(logfile, 'r') as file:
        log_content = file.read()
        sections = re.split(r'\(wall\)', log_content)
        sections.pop() # last token contains garbage

        extracted_data = {key: "NA" for key in regex_patterns.keys()}
        for section in sections:
            if tf_count == 0:
                print(f'{section}')
            for key, pattern in regex_patterns.items():
                match = re.search(pattern, section)
                if match:
                    extracted_data[key] = match.group(1)
            all_data.append(list(extracted_data.values()))
            tf_count += 1

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
