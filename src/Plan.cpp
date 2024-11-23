#include "Plan.h"
#include <iostream>
#include <sstream> // For stringstream in toString
using namespace std;

// Constructor
Plan::Plan(const int planId, Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(&settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
      status(PlanStatus::AVAILABLE), life_quality_score(0), economy_score(0), environment_score(0) {}

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

// Step method (placeholder logic)
void Plan::step()
{
    // Add your logic to progress the plan
    // basically: step in designated selection policy
    cout << "Plan is stepping... update states here." << endl;
}

// Print status of the plan
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