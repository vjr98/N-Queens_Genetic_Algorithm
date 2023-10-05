//
//  main.cpp
//  genetic algorithm vm
//
//  Created by Victoria Miller on 9/21/23.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int candidates = 8;
int candidateSize = 8;
float keep = 0.5;
float mutateRate = 0.2;
int limit = 1000;
bool debug = false;
fstream in;

struct sampleG
{
    string code;
    int value;
    int eval;
};

void generate(vector<sampleG> &samplesIn);
void showCandidates(vector<sampleG> samplesIn);
int toDec(string decIn);
int f(sampleG sampleIn);
void evaluate(vector<sampleG> &sampleIn);
void sort(vector<sampleG> &sampleIn);
void crossover(vector<sampleG> &sampleIn);
void mutate(vector<sampleG> &sampleIn);
bool write_csv(std::string filename, std::string field_one, std::string field_two);
sampleG maxVal;

int main(int argc, char **argv)
{
    cout << "Genetic Algorithm" << endl;
    cout << "Queens      Attacks" << endl;
    
    //initialization
    srand(time(NULL));
    
    vector<sampleG> candidates;
    
    //generate
    generate(candidates);
    showCandidates(candidates);
    
    bool writeToFile = write_csv("/Users/victoria/Documents/genetic algorithm vm/genetic algorithm vm/Genetic_Algorithm_Result.csv", "Attacks", "Max");
    
    for(int loop = 0; loop < limit; loop++)
    {
        cout << "Generation " << loop << endl;
        
        //evaluate
        //candidates[0].code = "47352065";
        evaluate(candidates);
        
        int avgAttacks = 0;
        for (int i = 0; i < candidates.size(); i++)
        {
            avgAttacks += candidates[i].eval;
        }
        
        avgAttacks = (avgAttacks / candidates.size());
        
        cout << "Avg Attacks: " << avgAttacks << endl;
        
        std::string attacks = std::to_string(avgAttacks);
        
        //sort
        sort(candidates);
        cout << "Queens        Attacks" << endl;
        showCandidates(candidates);
        
        writeToFile = write_csv("/Users/victoria/Documents/genetic algorithm vm/genetic algorithm vm/Genetic_Algorithm_Result.csv", attacks, maxVal.code);
        
        //crossover
        crossover(candidates);
        
        //mutate
        mutate(candidates);
        //showCandidates(candidates);
    }
}

void generate(vector<sampleG> &samplesIn)
{
    for(int num = 0; num < candidates; num++)
    {
        sampleG sample1;
        
        //generate a code based on candidateSize
        string temp = "";
        for (int i = 0; i < candidateSize; i++)
        {
            temp += '0' + (rand() % candidateSize);
        }
        sample1.code = temp;
        
        //determine value of the string code
        sample1.value = toDec(sample1.code);
        
        //pass the value to the fitness function
        sample1.eval = -1;
        
        samplesIn.push_back(sample1);
    }
}

void showCandidates(vector<sampleG> samplesIn)
{
    for(int i = 0; i < samplesIn.size(); i++)
    {
        cout << samplesIn[i].code << "      " << samplesIn[i].eval << endl;
    }
    cout << "=================================" << endl << endl;
}

int toDec(string decIn)
{
    int value = 0;
    int power = 0;
    
    for(int i = decIn.size() - 1; i >= 0; i--)
    {
        value += (pow(2,power++)*(decIn[i] - '0'));
    }
    
    return value;
}

int f(sampleG sampleIn)
{
    int conflicts = 0;
    int n = sampleIn.code.length();
    
    for(int i = 0; i < n; i++)
    {
        for(int j = (i+1); j < n; j++)
        {
            if (debug) cout << sampleIn.code[i] << " = " << sampleIn.code[j] << " = " << (i-j);
            if(sampleIn.code[i] == sampleIn.code[j] || (sampleIn.code[i]) == (sampleIn.code[j] + (j-i)) || (sampleIn.code[i]) == (sampleIn.code[j] - (j-i)))
            {
                conflicts++;
                if (debug) cout << "***";
            }
            
            if (debug) cout << endl;
        }
    }
        
    return conflicts;
}

void evaluate(vector<sampleG> &sampleIn)
{
    for(int i = 0; i < sampleIn.size(); i++)
    {
        sampleIn[i].eval = f(sampleIn[i]);
    }
}

void sort(vector<sampleG> &sampleIn)
{
    for(int i = 0; i < sampleIn.size() - 1; i++)
    {
        for(int j = i; j < sampleIn.size(); j++)
        {
            if(sampleIn[i].eval > sampleIn[j].eval)
            {
                //switch
                sampleG temp = sampleIn[i];
                sampleIn[i] = sampleIn[j];
                sampleIn[j] = temp;
            }
            
            maxVal.code = sampleIn[0].code;
        }
    }
    
    cout << "Best: " << maxVal.code << endl;
}

void crossover(vector<sampleG> &sampleIn)
{
    //make temp vector for new samples
    vector<sampleG> newCandidates;
    
    //loop on keep percentage of population size
    int keepCandidates = (candidates * keep);
    
    sampleG sample1;
    sampleG sample2;
    
    for(int i = 0; i < candidates/2; i++)
    {
        //choose two candidates from keep population
        sample1 = sampleIn[(rand() % keepCandidates)];
        sample2 = sampleIn[(rand() % keepCandidates)];
        
        sampleG newSample1;
        sampleG newSample2;
        
        //choose cut location
        int cut = rand() % (candidateSize - 2) + 1;
        
        //make two new candidates
        newSample1.code = "";
        newSample2.code = "";
        for(int j = 0; j < candidateSize; j++)
        {
            if(j < cut)
            {
                newSample1.code += sample1.code[j];
                newSample2.code += sample1.code[j];
            }
            else
            {
                newSample1.code += sample2.code[j];
                newSample2.code += sample1.code[j];
            }
        }
        
        //save them
        newSample1.value = toDec(newSample1.code);
        newSample2.value = toDec(newSample2.code);
        
        //verify valid nums
        newSample1.eval = -1;
        newSample2.eval = -1;
        
        newCandidates.push_back(newSample1);
        newCandidates.push_back(newSample2);
    }
    
    //replace old candidate list w the new one
    for(int i = 0; i < candidates; i++)
    {
        sampleIn[i] = newCandidates[i];
    }
}

void mutate(vector<sampleG> &sampleIn)
{
    for(int i = 0; i < sampleIn.size(); i++)
    {
        if(((rand() % 100)/(float)100) < mutateRate)
        {
            int location = rand() % candidateSize;
            sampleIn[i].code[location] = '0' + (rand() % candidateSize);
        }
    }
}

bool write_csv(std::string filename, std::string field_one, std::string field_two){
    // Make a CSV file with one column of integer values
    // filename - the name of the file
    // colname - the name of the one and only column
    // vals - an integer vector of values
    
    std::ofstream file;
    file.open(filename,std::ios::app);
    
    file << field_one << "," << field_two << '\n';
    file.close();
    
    return true;
}
