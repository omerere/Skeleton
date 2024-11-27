#include "SelectionPolicy.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

// ================== NaiveSelection ==================
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    const FacilityType& selected = facilitiesOptions[lastSelectedIndex];
    return selected;
}

const string NaiveSelection::toString() const {
    return "Naive Selection Policy";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

// ================== BalancedSelection ==================
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {

    int minIndex = 0; // Index of the facility with the smallest imbalance
    int minDistance = std::numeric_limits<int>::max(); // Initialize to a large value

    for (int i = 0; i < facilitiesOptions.size(); ++i) {
        const FacilityType& facility = facilitiesOptions[i];

        // Calculate the score distance for the current facility
        int lifeScore = LifeQualityScore + facility.getLifeQualityScore();
        int econScore = EconomyScore + facility.getEconomyScore();
        int envScore = EnvironmentScore + facility.getEnvironmentScore();
        int maxScore = std::max({lifeScore, econScore, envScore});
        int minScore = std::min({lifeScore, econScore, envScore});
        int distance = maxScore - minScore;

        // Update the minimum distance and index if necessary
        if (distance < minDistance) {
            minDistance = distance;
            minIndex = i;
        }
    }

    // Return the facility with the smallest imbalance
    return facilitiesOptions[minIndex];
}

const string BalancedSelection::toString() const {
    return "Balanced Selection Policy";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

// ================== EconomySelection ==================
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    int i = (lastSelectedIndex+ 1) % facilitiesOptions.size();
    while(i != lastSelectedIndex){
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = i; // Move to the next facility for the next call
            return facilitiesOptions[i];
        }
        else
            i = (i + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[i];    
}

const string EconomySelection::toString() const {
    return "Economy Selection Policy";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

// ================== SustainabilitySelection ==================
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    int i = (lastSelectedIndex+ 1) % facilitiesOptions.size();
    while(i != lastSelectedIndex){
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = i; // Move to the next facility for the next call
            return facilitiesOptions[i];
        }
        else
            i = (i + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[i];   
}

const string SustainabilitySelection::toString() const {
    return "Sustainability Selection Policy";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}