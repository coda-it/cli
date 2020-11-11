#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

class Persistence {
public:
  Persistence() {
    this->configFile.open("./config.txt", std::fstream::in | std::fstream::out |
                                              std::fstream::app);
    this->configFile.close();
  }

  void addRecord(std::string key, std::string value) {
    this->configFile.open("./config.txt", std::ios_base::app);
    this->configFile << key + ":" + value + "\n";
    this->configFile.close();
  }

  std::string getRecord(std::string key) {
    std::string line;
    std::vector<std::string> record;

    this->configFile.open("./config.txt", std::fstream::in);

    if (this->configFile.is_open()) {
      while (std::getline(this->configFile, line)) {
        record = coda::split(line, ":");

        if (record.size() >= 2) {
          if (record[0] == key) {
            return record[1];
          }
        }
      }
      this->configFile.close();
    }

    return "";
  }

  std::map<std::string, std::string> getAllRecords() {
    std::map<std::string, std::string> deployCommand;
    std::string line;
    std::vector<std::string> record;

    this->configFile.open("./config.txt", std::fstream::in);

    if (this->configFile.is_open()) {
      while (std::getline(this->configFile, line)) {
        record = coda::split(line, ":");

        if (record.size() >= 2) {
          deployCommand[record[0]] = record[1];
        }
      }
      this->configFile.close();
    }

    return deployCommand;
  }

private:
  std::fstream configFile;
};
