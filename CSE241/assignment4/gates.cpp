// Abdurrahman BULUT
// Contact : abdurrahmanbulut@yahoo.com
//========================================
// gates.cpp
// This file contains all evaluate functions implementations of all gates.


#include <iostream>
#include "gates.h"
#include <fstream>
#include <cstdlib>
#include <sstream>


using namespace std;

// Recursive end-point.
// It returns ınput value which is taken from file.
int Input::evaluate()
{
    return this->output;
}

// Recursive start-point
// It returns result of each output object
int Output::evaluate()
{
    int temp=0;
    
    // if previous gate is worked once, do not work it again. Same thing is happened all gates except input gate.
    if(input1->get_count_ff() == 0)
    {
        input1->set_count_ff(1);
        temp = input1->evaluate();
    }
    else
        temp = input1->get_output();

    return temp;
}
// It opearates "and" of input1 and input2.
// returns 1 or 0
int And::evaluate()
{
    int temp1;
    int temp2;
    if(input1->get_count_ff() == 0)
    {
        input1->set_count_ff(1);
        temp1 = input1->evaluate();
    }
    else
        temp1 = input1->get_output();

    if(input2->get_count_ff() == 0)
    { 
        input2->set_count_ff(1);
        temp2 = input2->evaluate();
    }
    else
        temp2 = input2->get_output();
    
    output_non = (temp1 && temp2); // to keep output_value to avoid work duplicate.
    return (temp1 && temp2);
}
// It opearates "or" of input1 and input2.
// returns 1 or 0
int Or::evaluate()
{
    int temp1;
    int temp2;

    if(input1->get_count_ff() == 0)
    {
        input1->set_count_ff(1);
        temp1 = input1->evaluate();
    }
    else
        temp1 = input1->get_output();

    if(input2->get_count_ff() == 0)
    { 
        input2->set_count_ff(1);
        temp2 = input2->evaluate();
    }
    else
        temp2 = input2->get_output();

    output_non = (temp1 || temp2); // to keep output_value to avoid work duplicate.
    return (temp1 || temp2);
}
// It opearates "not" of input
// returns 1 or 0
int Not::evaluate()
{
    int temp;
    if(input1->get_count_ff() == 0)
    {
        input1->set_count_ff(1);
        temp = input1->evaluate();
    }
    else
        temp = input1->get_output();

    output_non = (!temp); // to keep output_value to avoid work duplicate.
    return !(temp);
}
// It applicates flipflop gate logic.
// returns 0 or 1
int FlipFlop::evaluate()
{
    int temp = 0;
    
    if(input->get_count_ff() == 0)
    {
        input->set_count_ff(1);
        temp = input->evaluate();
    }
    else
        temp = input->get_output();
    
    if (temp == 0 && former_out == 0)
    {
        output_non = 0; // to keep output_value to avoid work duplicate.
        return 0;
    }
    else if (temp == 0 && former_out == 1)
    {
        output_non = 1; // to keep output_value to avoid work duplicate.
        return 1;
    }
    else if (temp == 1 && former_out == 0)
    {
        output_non = 1; // to keep output_value to avoid work duplicate.
        former_out = 1;
        return 1;
    }
    else
    {
        output_non = 0; // to keep output_value to avoid work duplicate.
        former_out = 0;
        return 0;
    }
}
// It applicates Decoder gate.
// Decoder has 2 inputs and 4 outputs.
///// It returns just an integer. This integer has 5 digit.
///// First digit is always 1. Other digits are actual output as 4 digit number.
///// This number will used for demonstration result.
int Decoder::evaluate()
{
    
    int temp1;
    int temp2;

    if(input1->get_count_ff() == 0)
    {
        input1->set_count_ff(1);
        temp1 = input1->evaluate();
    }
    else
        temp1 = input1->get_output();

    if(input2->get_count_ff() == 0)
    { 
        input2->set_count_ff(1);
        temp2 = input2->evaluate();
    }
    else
        temp2 = input2->get_output();

    int output=0;
    
    if (temp1 == 0 && temp2 == 0)
    {
        output_non = 11000; // to keep output_value to avoid work duplicate.
        output = 11000;
    }
    else if (temp1 == 0 && temp2 == 1)
    {
        output_non = 10100; // to keep output_value to avoid work duplicate.
        output = 10100;
    }
    else if (temp1 == 1 && temp2 == 0)
    {
        output_non = 10010; // to keep output_value to avoid work duplicate.
        output = 10010;
    }
    else if (temp1 == 1 && temp2 == 1)
    {
        output_non = 10001; // to keep output_value to avoid work duplicate.
        output = 10001;
    }
    return output;
}
