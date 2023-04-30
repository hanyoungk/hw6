#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true) //if true, should set r val to above debugging vals. false -> select randomized val
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here

        //stored the base-36 stuff so don't have to use pow()
        unsigned long long base[6] = {1, 36, 1296, 46656, 1679616, 60466176};

        //1) convert letters into a value 0-35

        int size = k.size();
        HASH_INDEX_T converted[size-1];

        for(int i = 0; i < size; i++) {
            converted[i] = letterDigitToNumber(k[size - i - 1]); // call helperFunction on current index/letter
            //converted[] has the value stored in reverse order so i can start from 0 for cnt
        }


        //2) translate substring of 6 letters a1 a2 a3 a4 a5 a6 into unsigned long long w[i]
        //      base-36 -> decimal
        //      w[i] = 36^5*a1 + 36^4*a2 + 36^3*a3 + 36^2*a4 + 36^1*a5 + 36^0*a6
        //      use base conversion approach 

        unsigned long long w[5] = {0,0,0,0,0};
        int cnt = 0;//start at 0, and "reset" every time it goes over 6
        int intIndex = 4; //index of w[i]
        int expo = 0; //start at 0 bc we start from the back of the 
        int c = 0; //to iterate thru the converted[]

        for(int i = size-1; i >= 0; i--) {
            
            //case 1: < 6 letters. put leading zeros in front
            if(cnt < 6) {
                w[intIndex] += converted[c] * base[expo];
                expo++;
                cnt++;
                c++;
            }
            //case 2: >= 6 letters. reset exponent count. do (3) for last 6 letters, repeat for the next set of 6 letters, etc.
            else if(cnt >= 6) {
                cnt = 0;
                expo = 0;
                intIndex--; //move on to the next index in w[i]
                i++; //use if condition is cnt >= 6
            }
        }

        //5) hash the string: 
        //      h(k) = (r[0]*w[0] + r[1]*w[1] + r[2]*w[2] + r[3]*w[3] + r[4]*w[4])
        HASH_INDEX_T result = 0;
        for(int i = 0; i < 5; i++) {
            result += rValues[i] * w[i];
        }

        return result;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        HASH_INDEX_T result;
        //if alphabet, subtract 97
        if(isalpha(letter)) { 
            //ex) 'a' = 0 , 'b' = 1
            letter = tolower(letter);
            result = toascii(letter) - 'a';
        }
        //if number, subtract 22
        if(isdigit(letter)) {
            //ex) 0 = 26 , 1 = 27
            result = (toascii(letter) - '0') + 26;
        }

        return result;
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
