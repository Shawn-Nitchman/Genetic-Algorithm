#include "Member.h"

//defualt constructor of the member and it opens the file incase a member is created when the file is closed. 
//It should not be used much but just incase it exist
Member::Member() {
	std::ifstream myfile("../input/Input.txt");
	int randomNum;
	float utility = 0.0, weight = 0.0, x, y;

	//this assigns the chromosome with a random numbers and finds the fitness
	for (int i = 0; i < selection.size(); i++) {
		randomNum = rand() % 100 + 1;

		myfile >> x;
		myfile >> y;

		//a 10% chance worked best for me to prevent all of them being overweight
		if (randomNum <= 10) {
			selection.at(i) = true;
			utility += x;
			weight += y;
		}
		else {
			selection.at(i) = false;
		}
	}

	if (weight > 500) {
		this->fitness = 1;
	}
	else
		this->fitness = utility;

	myfile.close();
}

//very similiar to the defualt consturctor excpet it doesn't have to open a file and uses already filled vectors
Member::Member(std::vector<float>& inpUtility , std::vector<float>& inpWeight) {
	int randomNum;
	float utility = 0.0, weight = 0.0, x, y;

	for (int i = 0; i < selection.size(); i++) {
		randomNum = rand() % 100 + 1;

		if (randomNum <= 10) {
			selection.at(i) = true;
			utility += inpUtility.at(i);
			weight += inpWeight.at(i);
		}
		else {
			selection.at(i) = false;
		}
	}

	if (weight > 500) {
		this->fitness = 1;
	}
	else
		this->fitness = utility;
}

//I have no idea what this does
float Member::getFitness() {
	return this->fitness;
}

//This sets the fitness but uses the item vectors to get there values. 
void Member::setFitness(std::vector<float>& inpUtility, std::vector<float>& inpWeight) {
	float utility = 0.0, weight = 0.0, x, y;

	for (int i = 0; i < selection.size(); i++) {

		if (selection.at(i) == true) {
			utility += inpUtility.at(i);
			weight += inpWeight.at(i);
		}
	}

	if (weight > 500) {
		this->fitness = 1;
	}
	else {
		this->fitness = utility;
	}
}

//Just a helper function to print of the selection(chromosome)
void Member::printSelection() {
	for (int i = 0; i < selection.size(); i++) {
		std::cout << selection.at(i) << " ";
		std::cout << " ";
	}
	std::cout << std::endl;
}

float Member::getProbability() {
	return this->probability;
}

void Member::setProbability(float prob) {
	this->probability = prob;
}

float Member::getCumlitiveProb() {
	return this->cumlitiveProbability;
}

void Member::setCumlitiveProb(float prob) {
	this->cumlitiveProbability = prob;
}
