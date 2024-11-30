#include "Plan.h"
#include <iostream>
#include <sstream> // For stringstream in toString
using namespace std;

// Constructor
Plan::Plan(const int planId, const Settlement *settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(*settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
      status(PlanStatus::AVAILABLE), life_quality_score(0), economy_score(0), environment_score(0)
      {

      }

// Getters for scores
int const Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

int const Plan::getEconomyScore() const
{
    return economy_score;
}

int const Plan::getEnvironmentScore() const
{
    return environment_score;
}

// Setter for selection policy
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}

void Plan::step() {
    // Step 1: Use the selection policy to select and add facilities
    if (status == PlanStatus::AVAILABLE) {
        const FacilityType &facilityType = selectionPolicy->selectFacility(facilityOptions);
        Facility *newFacility = new Facility(facilityType, settlement.getName());
        addFacility(newFacility); 
    }

    // Step 2: Update facilities' progress and status
    for (int i = 0; i < underConstruction.size(); i++) {
        Facility *facility = underConstruction[i];
        FacilityStatus updatedStatus = facility->step();
    
        if (updatedStatus == FacilityStatus::OPERATIONAL) {
            // Update scores
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
            underConstruction.erase( std::next(underConstruction.begin(), i) );
        }
    }

    // Step 4: Update the plan's status based on remaining under-construction facilities
    status = PlanStatus::BUSY; 
    if (settlement.getType() == SettlementType::VILLAGE && underConstruction.size() < 1)    {
        status = PlanStatus::AVAILABLE;
    }
    else if (settlement.getType() == SettlementType::CITY && underConstruction.size() < 2)
    {
        status = PlanStatus::AVAILABLE; 
   }
    else if (settlement.getType() == SettlementType::METROPOLIS && underConstruction.size() < 3)
    {
        status = PlanStatus::AVAILABLE;
    }
}

// this is a place holder, to be implemented with "PrintPlanStatus" base action
void Plan::printStatus()
{
    cout << "Plan ID: " << plan_id << ", Status: ";
    cout << (status == PlanStatus::AVAILABLE ? "Available" : "Busy") << endl;
}

// Get the facilities
const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

// Add a facility to the plan
void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
}

// Convert the plan to string representation
const string Plan::toString() const
{
    stringstream ss;
    ss << "Plan ID: " << plan_id << "\n"
       << "Status: " << (status == PlanStatus::AVAILABLE ? "Available" : "Busy") << "\n"
       << "Life Quality Score: " << life_quality_score << "\n"
       << "Economy Score: " << economy_score << "\n"
       << "Environment Score: " << environment_score << "\n";
    return ss.str();
}