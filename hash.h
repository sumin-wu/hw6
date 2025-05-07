#ifndef HASH_H
#define HASH_H

#include <deque>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>


typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        std::deque<unsigned long long> chunkValues; //stores converted base36 chunk
        unsigned long long chunkTotal = 0;
        int charPosition = 0;//tracks power

        std:: string remaining = k;
        while (!remaining.empty()){
            char currentChar = remaining.back();
            remaining.pop_back();

            //convert base36 digit and add to current chunks chunkTotal
            chunkTotal += letterDigitToNumber(currentChar) * static_cast<unsigned long long>(pow(36, charPosition));
            ++charPosition;

            if(charPosition ==6){
                chunkValues.push_front(chunkTotal);
                chunkTotal=0;
                charPosition=0;
            }
            charPosition = charPosition%6;
        }

        //handle final chunk (if less than 6 characters)
        if (charPosition>0){
            chunkValues.push_front(chunkTotal);
            chunkTotal=0;
        }

        while(chunkValues.size()<5){
            chunkValues.push_front(0);
        }

        HASH_INDEX_T finalHash = 0;
        for (int i =0; i<chunkValues.size(); i++){
            finalHash += rValues[i]*chunkValues[i];
        }
        return finalHash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char character) const
    {
        // Add code here or delete this helper function if you do not want it
        character = static_cast<char>(tolower(character));

        if(character>='a' && character<='z'){
            return static_cast<HASH_INDEX_T>(character - 'a');//0 to 25
        }
        else if (character>='0' && character<='9'){
            return static_cast<HASH_INDEX_T>(character - '0' +26); //26 to 35
        }
        else return 0;

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
