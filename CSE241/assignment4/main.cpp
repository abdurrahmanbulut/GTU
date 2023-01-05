// Abdurrahman BULUT
// Contact : abdurrahmanbulut@yahoo.com
//========================================
// main.cpp

// Compile it using      ->   "make"        command
// Run it using          ->   "make run"    command
// Clean .o files using  ->   "make clean"  command

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>

#include "gates.h"
#include "reader.h"  // read circuit file once ant returns gates count, outputs count and inputs count


using namespace std;

int main(int argc, char **argv)
{
    Gate** p; 
    
    int gateCount, outputCount, inputCount, temp, i, j=0, trackGate=0, flipflop;
    string temp1, temp2, temp3; // they are used for stringstream temps.

    readCircuit("circuit.txt", &gateCount, &outputCount, &inputCount); // gatecount includes inputs and outputs

    p = new Gate*[gateCount];               // gatecount = 11 -> i i i i g g g o o o o
    Input* in = new Input[inputCount];
    Output* out = new Output[outputCount];


    ifstream inpFile, cirFile;
    inpFile.open("input.txt");
    cirFile.open("circuit.txt");

    string inpLine, cirLine;
    stringstream ss;
    while (getline(inpFile, inpLine))  // takes number from input file row by row.
    {
        trackGate = 0; // track gate is used to be informed which gate is in which index
        ss.str(inpLine);
        i = 0;
        while (ss >> temp1) // temp1 -> input values
        {
            in[i].set_output_value(stoi(temp1));
            p[i] = &in[i]; 
            i++;
        }
        ss.clear();
        while (getline(cirFile, cirLine)) // takes gate names and inputs output names of gates and store it.
        {
            ss.str(cirLine);
            ss >> temp1;
            if(temp1 == "INPUT")
            {   i = 0;
                while (ss >> temp1)
                {
                    p[i]->set_output_name(temp1);
                    p[i++]->set_gate_name("INPUT");
                    trackGate++;
                }
            }
            else if(temp1 == "OUTPUT")
            {
                j = 0;
                i = gateCount - outputCount; // i i i i g g g g g g o(i) o o o 
                while (ss >> temp1)
                {
                    p[i] = &out[j++];
                    p[i]->set_input1_name(temp1);
                    p[i++]->set_gate_name("OUTPUT");
                }
            }
            else if(temp1 == "AND")
            {
                i = trackGate;
                And* an = new And;  // create new And object and 
                p[i] = an;          // connect to Gate pointer
                ss >> temp1;
                p[i]->set_gate_name("AND");
                p[i]->set_output_name(temp1);
                ss >> temp1;
                p[i]->set_input1_name(temp1);
                ss >> temp1;
                p[i]->set_input2_name(temp1);
                for (j = 0; j < trackGate; j++)
                {  
                    if(p[j]->get_output_name() == p[i]->get_input1_name())
                    {
                        p[i]->set_p_input1(p[j]);  // connections are made in here. input1* in p[i] stores p[j] address.
                    }
                    if(p[j]->get_output_name() == p[i]->get_input2_name())
                    {
                        p[i]->set_p_input2(p[j]); // connections are made in here. input2* in p[i] stores p[j] address.
                    }
                }
                trackGate++;
            }
            else if(temp1 == "OR")
            {
                i = trackGate;
                Or* orr = new Or; // create new Or object and 
                p[i] = orr;       // connect to Gate pointer
                ss >> temp1;    
                p[i]->set_gate_name("OR");                
                p[i]->set_output_name(temp1);
                ss >> temp1;
                p[i]->set_input1_name(temp1);
                ss >> temp1;
                p[i]->set_input2_name(temp1);
                for (j = 0; j < trackGate; j++)
                {
                    if(p[j]->get_output_name() == p[i]->get_input1_name())
                    {
                        p[i]->set_p_input1(p[j]); // connections are made in here. input1* in p[i] stores p[j] address.
                    }
                    if(p[j]->get_output_name() == p[i]->get_input2_name())
                    {
                        p[i]->set_p_input2(p[j]); // connections are made in here. input2* in p[i] stores p[j] address.
                    }
                }
                trackGate++;
            }
            else if(temp1 == "NOT")
            {
                i = trackGate;
                Not* nott = new Not;  // create new Not object and 
                p[i] = nott;            // connect to Gate pointer
                ss >> temp1;
                p[i]->set_gate_name("NOT");
                p[i]->set_output_name(temp1);
                ss >> temp1;
                p[i]->set_input1_name(temp1);
                for (j = 0; j < trackGate; j++)
                {
                    if(p[j]->get_output_name() == p[i]->get_input1_name())
                    {
                        p[i]->set_p_input1(p[j]);  // connections are made in here. input1* in p[i] stores p[j] address.
                    }
                }
                trackGate++;
            }
            else if(temp1 == "FLIPFLOP")
            {
                i = trackGate;
                flipflop = i;      // flipflop variable is used for informed about flip flop index.
                FlipFlop* ff = new FlipFlop;  // create new FlipFlop object and 
                p[i] = ff;                       // connect to Gate pointer
                ss >> temp1;
                p[i]->set_gate_name("FlipFlop");
                p[i]->set_output_name(temp1);
                ss >> temp1;
                p[i]->set_input1_name(temp1);
                for (j = 0; j < trackGate; j++)
                {  
                    if(p[j]->get_output_name() == p[i]->get_input1_name())
                    {
                        p[i]->set_p_input1(p[j]);  // connections are made in here. input1* in p[i] stores p[j] address.
                    }
                }
                trackGate++;
            }
            else if(temp1 == "DECODER")
            {
                i = trackGate;
                Decoder* d = new Decoder;  // create new Decoder object and 
                p[i] = d;                   // connect to Gate pointer
                ss >> temp1;
                p[i]->set_gate_name("DECODER");
                p[i]->set_output_name(temp1);
                ss >> temp1;
                p[i]->set_output1_name(temp1);
                ss >> temp1;
                p[i]->set_output2_name(temp1);
                ss >> temp1;
                p[i]->set_output3_name(temp1);
                ss >> temp1;
                p[i]->set_input1_name(temp1);
                ss >> temp1;
                p[i]->set_input2_name(temp1);
                for (j = 0; j < trackGate; j++)
                {
                    if(p[j]->get_output_name() == p[i]->get_input1_name())
                    {
                        p[i]->set_p_input1(p[j]);  // connections are made in here. input1* in p[i] stores p[j] address.
                    }
                    if(p[j]->get_output_name() == p[i]->get_input2_name())
                    {
                        p[i]->set_p_input2(p[j]);  // connections are made in here. input2* in p[i] stores p[j] address.
                    }
                }
                trackGate++;
            }
        
            ss.clear(); // to refresh stringstream for new lines.
        }
         
        for(int k = 0; k < outputCount; k++)
        {
            for (j = 0; j < gateCount - outputCount; j++)
            {
                if (p[j]->get_output_name() == p[gateCount - outputCount + k]->get_input1_name())
                {
                    p[gateCount - outputCount + k]->set_p_input1(p[j]); // output objects connections
                }
            }
        }

        int out_decoder=0;
        int d1, d2, d3,d4;
        for (j = 0; j < outputCount; j++)
        {
            // if last gate is decoder, it calculates it and prints result.
            if (p[gateCount - outputCount - 1 + j]->get_gate_name() == "DECODER")
            {
                out_decoder = p[gateCount - outputCount]->evaluate();
                d4 = out_decoder % 10;
                out_decoder /= 10;
                d3 = out_decoder % 10;
                out_decoder /= 10;
                d2 = out_decoder % 10;
                out_decoder /= 10;
                d1 = out_decoder % 10;
                cout << d1 << " " << d2 << " " << d3 << " " << d4 << " ";
                j += 4; // decoder has 4 outputs
            }
            else
            {
                out_decoder = p[gateCount - outputCount + j]->evaluate();
                cout << out_decoder << " ";
                j++;
            }
        }
        cout << endl;
        for (int c = 0; c < gateCount; c++)
        {
            p[c]->set_count_ff(0); // to refresh circuit
        }
        
    }
    
    cirFile.close();
    inpFile.close();

    // delete is not necessary bacause the program reaches end of the main.

	return 0;
}
