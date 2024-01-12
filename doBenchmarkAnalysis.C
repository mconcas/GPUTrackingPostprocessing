#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <TH1F.h>
#include <TFile.h>

void doBenchmarkAnalysis(const std::string logFileName = "its_time_benchmarks_hip")
{
  std::ifstream logFile(logFileName + ".txt");
  std::string line;
  std::map<std::string, TH1F*> histograms;
  std::string operation;
  float metric;

  // Read each line of the log file
  while (std::getline(logFile, line)) {
    std::stringstream ss(line);
    ss >> operation >> metric;

    // Create a histogram if it doesn't exist
    if (histograms.find(operation) == histograms.end()) {
      histograms[operation] = new TH1F(operation.c_str(), operation.c_str(), 100, 0, 5000); // Adjust binning as needed
    }

    // Fill the histogram
    histograms[operation]->Fill(metric);
  }

  logFile.close();

  // Create a file to store the histograms
  std::string outputFileName = logFileName + ".root";
  TFile outputFile(outputFileName.c_str(), "RECREATE");

  // Calculate average and RMS, and write histograms to file

  // Output Markdown table
  std::cout << "|-----------|---------|-----|\n";
  std::cout << "| Operation | Average (ms) | RMS (ms) |\n";
  std::cout << "|-----------|---------|-----|\n";

  // Calculate average and RMS, and output in Markdown format
  for (auto& [op, hist] : histograms) {
    std::cout << "| " << op << " | " << hist->GetMean() << " | " << hist->GetRMS() << " |\n";
    hist->Write(); // Clean up the histogram object
  }
  std::cout << "|-----------|---------|-----|\n";

  outputFile.Close();
}
