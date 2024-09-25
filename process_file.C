#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TH1F.h>
#include <TFile.h>

void process_file(const char* input_file) {
    // Open the input CSV (or .dat) file
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << input_file << std::endl;
        return;
    }

    // Prepare the output ROOT file
    std::string output_file = std::string(input_file);
    output_file.replace(output_file.find(".dat"), 4, ".root");
    TFile* rootFile = new TFile(output_file.c_str(), "RECREATE");

    // Create a histogram for tf_time_wall
    TH1F* hist = new TH1F("tf_time_wall", "TF Time Wall Distribution", 100, 0, 100); // Adjust binning as needed

    std::string line;
    bool firstLine = true;

    // Read the file line by line
    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false; // Skip the header
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Tokenize the line
        while (std::getline(iss, token, '\t')) {
            tokens.push_back(token);
        }

        // Extract the tf_time_wall value (assumed to be in the 7th column)
        if (tokens.size() >= 7) {
            double tf_time_wall = std::stod(tokens[6]);
            hist->Fill(tf_time_wall);
        }
    }

    // Write the histogram to the ROOT file
    hist->Write();

    // Close the files
    rootFile->Close();
    file.close();

    std::cout << "Histogram saved to " << output_file << std::endl;
}