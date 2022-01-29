#pragma once
#include <iostream>
#include <vector>
#include <fstream>


class Member {
public:
	Member();
	Member(std::vector<float>&, std::vector<float>&);

	std::vector<bool> selection = std::vector<bool>(400);
	
	float getFitness();
	void setFitness(std::vector<float>&, std::vector<float>&);

	void printSelection();

	float getProbability();
	void setProbability(float);

	float getCumlitiveProb();
	void setCumlitiveProb(float);

private:
	float fitness = 0.0;
	float probability = 0.0;
	float cumlitiveProbability = 0.0;
};