#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include "Member.h"

//This fuction makes the population 
void fillInitPop(std::vector<Member>&);

// this function does most of the L2 normailization
void L2norm(std::vector<Member>&);

// this function takes the population and make a new population from the children
std::vector<Member> reproduce(std::vector<Member>&);

// This function takes the L2 normailization and selects all the parents to make the children
std::vector<Member> selection(std::vector<Member>&);

//this function takes the two parents and makes the two children
std::vector<Member> crossover(std::vector<Member>&);

//this function takes the children and applies the mutation
std::vector<Member> mutation(std::vector<Member>&);

// This funtcion prints off the best fitness for the whole population
void printBest(std::vector<Member>&);

// this is a helper function to print off vector
void printVector(std::vector<Member>&);

// this function gets the input from the input file
void getInput();
//this is a helper fucnction to make sure input are correct
void printInput();

//yes global variables, I had so much trouble with the inputs and will explain more in the paper
std::vector<float> inpUtility;
std::vector<float> inpWeight;


int main()
{
    srand(time(NULL));

    getInput();

    //creating the population that is full of Members
    std::vector<Member> population;
    fillInitPop(population);

    int generation = 0;

    //This while loop will go through 1000 generations 
    while (generation < 1000) {
        //Gets the "mating" pool ready by finding the L2 and getting ready for selection
        L2norm(population);

        // replaces the population with the children of the current one
        population = reproduce(population);

        std::cout << std::endl << "generation: " << generation << std::endl;
        printBest(population);
        
        generation++;
    }

}

// this opens the file and takes in the utility and weight and assigns them to the right list
void getInput() {
    {
        std::ifstream myfile("../input/Input.txt");
        float x, y;

        for (int i = 0; i < 400; i++) {
            myfile >> x;
            myfile >> y;

            inpUtility.push_back(x);
            inpWeight.push_back(y);
        }

        myfile.close();
    }
}

// This is where the size of population is set and fills the population vector with all the members 
void fillInitPop(std::vector<Member>& population) {

    int size = 1000;

    for (int i = 0; i < size; i++) {
        Member member(inpUtility, inpWeight);
        population.push_back(member);
    }
}

// This does L2 normilization
void L2norm(std::vector<Member>& population) {
    int size = population.size();
    float sum = 0.0, probability = 0.0, cumlitive = 0.0;

    //first we need to get the sum of population fitness score and squared
    for (int i = 0; i < size; i++) {
        sum += pow(population.at(i).getFitness(), 2);
    }

    //For each member they have a proabability which shows how good they are but it needs to be compared
    //cumulaztive value will identify which members are actually the best
    for (int i = 0; i < size; i++) {
        probability = pow(population.at(i).getFitness(), 2) / sum;
        population.at(i).setProbability(probability);

        cumlitive += probability;
        population.at(i).setCumlitiveProb(cumlitive);
    }
}

//Now that we have the cumulatice distribution "table" we can select parents and have them make children
std::vector<Member> reproduce(std::vector<Member>& population) {
    //since selecting 2 parents only need to go over the vector half the amount
    int size = population.size() / 2;
    std::vector<Member> newPopulation;

    for (int i = 0; i < size; i++) {
        //making a parent vector that the crossover function will use. 
        std::vector<Member> parents = selection(population);

        //making a child vector that will then be added to the new population
        std::vector<Member> childs = crossover(parents);

        for (int j = 0; j < childs.size(); j++) {
            newPopulation.push_back(childs.at(j));
        }
    }

    //returning all the children to be assigned as the population
    return newPopulation;
}

// Picks a number between 0 and 1 and finds the value that the member Cumulative is > than generated number
std::vector<Member> selection(std::vector<Member>& population) {
    std::vector<Member> temp;
    int size = population.size();

    //from my understanding of the last slide on normilization this is how it can be done and it does it twice for 2 childs
    for (int i = 0; i < 2; i++)
    {
        float r = (float)rand() / (float)RAND_MAX;
        for (int j = 0; j < size; j++) {
            //once the cumulative is bigger add that parent to the vector to make a child.
            if (population.at(j).getCumlitiveProb() > r) {
                temp.push_back(population.at(j));
                break;
            }
        }
    }
    
    return temp;
}

//This will take a random number and take parent 1 and parent 2 selection(chromosome) and make 2 children
std::vector<Member> crossover(std::vector<Member>& parents) {
    //make a child and parent vector equal to do half the work for me. 
    std::vector<Member> childs = parents;
    int randomCut = rand() % 400 + 1;

    // child and parent vector are the same. So the first child matches the first parents but it needs parent 2 dna 
    // so we go to the random cut off point and grab all of parent 2 dna for child 1  parents.at(1).selection.at(i)
    for (int i = randomCut; i < 400; i++) {
        childs.at(0).selection.at(i) = parents.at(1).selection.at(i);
    }

    // child and parent vector are the same. So the second child matches the second parents but it needs parent 1 dna 
    // so we go to the random cut off point and grab all of parent 1 dna for child 2
    for (int i = randomCut; i < 400; i++) {
        childs.at(1).selection.at(i) = parents.at(0).selection.at(i);
    }

    //Since I have the children selection(chromose) done I can see if they need to be mutated
    childs = mutation(childs);

    //I need to set there fitness again otherwise it would be the same as the parent
    for (int i = 0; i < childs.size(); i++) {
        childs.at(i).setFitness(inpUtility, inpWeight);
    }

    return childs;
}

//This will go over the child vector and then look at each selection(chromosome) and see if it gets randomly changed
std::vector<Member> mutation(std::vector<Member>& childs) {
    int randomChance = 0;

    for (int i = 0; i < childs.size(); i++) {
        for (int j = 0; j < childs.at(i).selection.size(); j++) {
            randomChance = rand() % 10000 + 1;
            //flips the bool variable if it actually was a 1/10000 chance
            if (randomChance == 1) {
                childs.at(i).selection.at(j) = !childs.at(i).selection.at(j);
            }
        }
    }

    return childs;
}

//This prints which member had the best fitness score
void printBest(std::vector<Member>& population) {
    unsigned int size = population.size();
    float best = 0.0;
    int index;

    for (unsigned int i = 0; i < size; i++) {
        if (population.at(i).getFitness() > best) {
            best = population.at(i).getFitness();
            index = i;
        }
    }

    std::cout << "fitness score: " << population.at(index).getFitness() <<std::endl;

}

//just prints off a vector
void printVector(std::vector<Member>& population) {
    unsigned int size = population.size();

    for (unsigned int i = 0; i < size; i++) {
        std::cout << "print vector at " << i << std::endl;
        population.at(i).printSelection();
        std::cout << std::endl;
    }

}

//Used to check that inputs where correct
void printInput() {
    for (int i = 0; i < 400; i++) {
        std::cout << "Item " << i + 1 << ": " << std::endl;
        std::cout << "    Utility: " << inpUtility.at(i) << std::endl;
        std::cout << "    Weight : " << inpWeight.at(i) << std::endl;
    }
}

/*
SETUP
    Step 0: Read from a file
    Step 1: initialize- create a population of 1000 elements, each with randomly generated DNA

LOOP
    Step 2: Selection- Evaluate the fitness of each elemenet of the population and build a mating pool

    Step 3: Reproduction- Repeat until average fintess improves less then 1% across 10 generations:
        a. Pick two parents with L2
        b. crossover- create a "child" by combining the DNA of these two parents
        c. Mutation- mutate the child's DNA based on 0.0001
        d. Add the new child to a new populatiom

    Step 4: replace the old population with the new population and return to step 2

RESULTS
    Step 5: Print to a file the highest fitness selection
*/
