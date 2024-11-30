#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
#include "Facility.h"
#include "Plan.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Constructor
Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0) {
    // Open the configuration file
    std::ifstream configFile(configFilePath);

    std::string line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "settlement") {
            std::string name;
            int settlementType;
            iss >> name >> settlementType;

            Settlement *newSettlement = new Settlement(name, static_cast<SettlementType>(settlementType));
            if (!addSettlement(newSettlement)) {
                delete newSettlement; // Clean up if the settlement already exists
                throw std::runtime_error("Duplicate settlement in config file");
            }
        } else if (command == "facility") {
            std::string name;
            int category, price, lifeQualityImpact, economyImpact, environmentImpact;
            iss >> name >> category >> price >> lifeQualityImpact >> economyImpact >> environmentImpact;

            FacilityType facility(name, static_cast<FacilityCategory>(category), price, lifeQualityImpact, economyImpact, environmentImpact);
            if (!addFacility(facility)) {
                throw std::runtime_error("Duplicate facility in config file");
            }
        } else if (command == "plan") {
            std::string settlementName, policyType;
            iss >> settlementName >> policyType;

            SelectionPolicy *policy = nullptr;
            if (policyType == "nve") {
                policy = new NaiveSelection();
            } else if (policyType == "bal") {
                policy = new BalancedSelection(0, 0, 0);
            } else if (policyType == "eco") {
                policy = new EconomySelection();
            } else if (policyType == "env") {
                policy = new SustainabilitySelection();
            } else {
                throw std::runtime_error("Unknown selection policy type in config file");
            }

            try {
                Settlement *settlement = getSettlement(settlementName);
                addPlan(settlement, policy);
            } catch (const std::exception &e) {
                delete policy; // Clean up if the settlement doesn't exist
                throw std::runtime_error("Error creating plan: " + std::string(e.what()));
            }
        } else {
            throw std::runtime_error("Invalid command in config file: " + command);
        }
    }
}

// Start the simulation
void Simulation::start() {
    isRunning = true;
    std::cout << "The simulation has started" << std::endl;
}

// Add a plan
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy) {
Plan newPlan(planCounter, settlement, selectionPolicy, facilitiesOptions);
planCounter++;
plans.push_back(newPlan);
    
}

// Add an action
void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

// Add a settlement
bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())) {
        delete settlement;
        std::cout << "Settlement already exists." << std::endl;
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

// Add a facility type
bool Simulation::addFacility(FacilityType facility) {
    for (const FacilityType &existingFacility : facilitiesOptions) {
        if (existingFacility.getName() == facility.getName()) {
            std::cout << "Facility already exists." << std::endl;
            return false; // Duplicate facility found
        }
    }
    facilitiesOptions.push_back(facility); // Add the facility if no duplicates
    return true;
}

// Check if a settlement exists
bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement *settlement : settlements) { 
        if (settlement->getName() == settlementName) { 
            return true;
        }
    }    
    return false; 
}

// Get a settlement by name
Settlement *Simulation::getSettlement(const string &settlementName) {
    for (Settlement *settlement : settlements) { 
        if (settlement->getName() == settlementName) { 
            return settlement; 
        }
    }
    throw std::runtime_error("Settlement not found"); 
}

// Get a plan by Id
Plan &Simulation::getPlan(const int planId) {
    if (planId>= planCounter)
        throw std::runtime_error("Plan not found");
    else
         return plans[planId];   
}

// Perform a simulation step
void Simulation::step() {
    for (Plan &plan : plans) {
        plan.step();
    }
}

// Close the simulation
void Simulation::close() {
    isRunning = false;
    std::cout << "Simulation closed." << std::endl;
}

// Open the simulation
void Simulation::open() {
    isRunning = true;
    std::cout << "Simulation opened." << std::endl;
}
