#include "Action.h"
#include "Auxiliary.h"
#include <iostream>
#include <stdexcept>
#include <sstream>





using namespace std;

BaseAction::BaseAction() : status(ActionStatus::ERROR), errorMsg("") {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    cout << "Error: " << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}


SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    stringstream ss;
    ss << "SimulateStep " << numOfSteps;
    return ss.str();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}


AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    try {
        Settlement *settlement = simulation.getSettlement(settlementName);
        SelectionPolicy *policy = nullptr;

        if (selectionPolicy == "nve") {
            policy = new NaiveSelection();
        } else if (selectionPolicy == "bal") {
            policy = new BalancedSelection(0, 0, 0);
        } else if (selectionPolicy == "eco") {
            policy = new EconomySelection();
        } else if (selectionPolicy == "env") {
            policy = new SustainabilitySelection();
        } else {
            throw runtime_error("Invalid selection policy");
        }

        simulation.addPlan(settlement, policy);
        complete();
    } catch (const exception &e) {
        error("Cannot create this plan");
    }
}

const string AddPlan::toString() const {
    stringstream ss;
    ss << "AddPlan " << settlementName << " " << selectionPolicy;
    return ss.str();
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}


AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    Settlement *settlement = new Settlement(settlementName, settlementType);

    if (!simulation.addSettlement(settlement)) {
        delete settlement; // Prevent memory leak
        error("Settlement already exists");
        return;
    }
    complete();
}

const string AddSettlement::toString() const {
    stringstream ss;
    ss << "AddSettlement " << settlementName << " " << static_cast<int>(settlementType);
    return ss.str();
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}



AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    FacilityType facility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);

    if (!simulation.addFacility(facility)) {
        error("Facility already exists");
        return;
    }
    complete();
}

const string AddFacility::toString() const {
    stringstream ss;
    ss << "AddFacility " << facilityName << " " << static_cast<int>(facilityCategory) << " " << price << " "
       << lifeQualityScore << " " << economyScore << " " << environmentScore;
    return ss.str();
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}



PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    try {
        Plan &plan = simulation.getPlan(planId);
        plan.printStatus();
        complete();
    } catch (const runtime_error &e) {
        error("Plan does not exist");
    }
}

const string PrintPlanStatus::toString() const {
    stringstream ss;
    ss << "PrintPlanStatus " << planId;
    return ss.str();
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}



ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) 
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    try {
        Plan &plan = simulation.getPlan(planId);
        SelectionPolicy *policy = nullptr;

        if (newPolicy == "nve") {
            policy = new NaiveSelection();
        } else if (newPolicy == "bal") {
            policy = new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());
        } else if (newPolicy == "eco") {
            policy = new EconomySelection();
        } else if (newPolicy == "env") {
            policy = new SustainabilitySelection();
        } else {
            throw runtime_error("Invalid selection policy");
        }

        plan.setSelectionPolicy(policy);
        complete();
    } catch (const runtime_error &e) {
        error("Cannot change policy");
    }
}

const string ChangePlanPolicy::toString() const {
    stringstream ss;
    ss << "ChangePlanPolicy " << planId << " " << newPolicy;
    return ss.str();
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}