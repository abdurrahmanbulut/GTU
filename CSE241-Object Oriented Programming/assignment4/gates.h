// Abdurrahman BULUT
// Contact : abdurrahmanbulut@yahoo.com
//========================================
// gates.h
// This file contains all class definitions of all gates.


#ifndef GATES_H_BULUT
#define GATES_H_BULUT

#include <iostream>
#include <string>
using std::string;

// Super class
class Gate
{
public:
        virtual int evaluate(){return 0;}
        virtual void set_input1_name(string name){return;} // inline func, it is a setter for input1 name used by derived class.
        virtual string get_input1_name(){return "empty";}  // inline func, it is a getter for input1 name used by derived class.
        virtual void set_input2_name(string name){return;} // inline func, it is a setter for input2 name used by derived class.
        virtual string get_input2_name(){return "empty";}  // inline func, it is a getter for input2 name used by derived class.
        virtual void set_output_name(string name){return;} // inline func, it is a setter for output name used by derived class.
        virtual string get_output_name(){return "empty";}  // inline func, it is a getter for output name used by derived class.
        virtual void set_output1_name(string name){return;} // inline func, it is a setter for output1 name used by derived class.
        virtual string get_output1_name(){return "empty";}  // inline func, it is a getter for output1 name used by derived class.
        virtual void set_output2_name(string name){return;} // inline func, it is a setter for output2 name used by derived class.
        virtual string get_output2_name(){return "empty";}  // inline func, it is a getter for output2 name used by derived class.
        virtual void set_output3_name(string name){return;} // inline func, it is a setter for output3 name used by derived class.
        virtual string get_output3_name(){return "empty";}  // inline func, it is a getter for output3 name used by derived class.
        virtual string get_gate_name(){return "empty";}     // inline func, it is a setter for gate name used by derived class.
        virtual void set_gate_name(string name){name = "";} // inline func, it is a getter for gate name used by derived class.
        virtual int get_output_value(){return 0;}           // inline func, it is a getter for output value used by derived class.
        virtual void set_output_value(int output){return;}  // inline func, it is a setter for output value used by derived class.
        virtual void set_p_input1(Gate* temp){return;}      // inline func, it is a setter for input1 pointer used by derived class.
        virtual void set_p_input2(Gate* temp){return;}      // inline func, it is a setter for input2 pointer used by derived class.
        virtual void set_former_out(int a){return;}         // inline func, it is a setter for former out value used by derived class.
        virtual int get_former_out(){return 0;}             // inline func, it is a getter for former out value used by derived class.
        virtual void set_count_ff(int a){return;}           // inline func, it is a setter count_ff value used by derived class.
        virtual int get_count_ff(){return 0;}               // inline func, it is a getter for count_ff value used by derived class.
        virtual int get_output(){return 0;}                 //  inline func, it is a getter for output value of previous invoking used by derived class.

protected:
        int output_non=0; // Output_non is last output value of any gate. It used when duplication of working
        int former_out=0; // former_out is for tracking flipflop gate value.
        int count_ff=0; // count_ff is number of gate invoking. It used to prevent multiple operations of a gate. ff means flipflop
                        // if it is 1, then that gate does not work again. Just output value of this gate will be used.
};
// Input class has output name, gate name and output value.
class Input : public Gate
{

public:
        int evaluate();
        void set_output_name(string name){ this->output_name = name;}
        string get_output_name(){ return this->output_name; }
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_output_value(int output){this->output = output;}
        void set_count_ff(int a){count_ff = a;}

private:
        string output_name;
        string gate_name;
        int output;
};
// Output class has input name, gate name and input1 pointer that points previous gate..
class Output : public Gate
{
public:
        int evaluate();
        void set_input1_name(string name) { this->input_name = name; }
        string get_input1_name() { return this->input_name; }
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input1 = temp;}
        int get_output(){return output_non;}
        void set_count_ff(int a){count_ff = a;}
private:
        string input_name;
        string gate_name;
        Gate* input1;
};
// And class has input1 name, input2 name, gate name, output name, input1 pointer and input2 pointer those point previous gates.
class And : public Gate{
    
public:
        int evaluate();
        void set_input1_name(string name){ this->input1_name = name;}
        string get_input1_name(){ return this->input1_name; }
        void set_input2_name(string name){ this->input2_name = name;}
        string get_input2_name(){ return this->input2_name; }
        void set_output_name(string name){ this->output_name = name;}
        string get_output_name(){ return this->output_name; }
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input1 = temp;}
        void set_p_input2(Gate* temp){input2 = temp;}
        void set_count_ff(int a){count_ff = a;}
        int get_output(){return output_non;}

private:
        string input1_name;
        string input2_name;
        string output_name;
        string gate_name;
        Gate* input1;
        Gate* input2;
};
// Or class has input1 name, input2 name, gate name, output name, input1 pointer and input2 pointer those point previous gates.
class Or : public Gate{
    
public:
        int evaluate();
        void set_input1_name(string name){ this->input1_name = name;}
        string get_input1_name(){ return this->input1_name; }
        void set_input2_name(string name){ this->input2_name = name;}
        string get_input2_name(){ return this->input2_name; }
        void set_output_name(string name){ this->output_name = name;}
        string get_output_name(){ return this->output_name; }
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input1 = temp;}
        void set_p_input2(Gate* temp){input2 = temp;}
        int get_output(){return output_non;}
        void set_count_ff(int a){count_ff = a;}

private:
        string input1_name;
        string input2_name;
        string output_name;
        string gate_name;
        Gate* input1;
        Gate* input2;
};

// Not class has input name, gate name, output name and input1 pointer that points previous gate.
class Not : public Gate{
    
public:
        int evaluate();
        void set_input1_name(string name){ this->input_name = name; }
        string get_input1_name(){ return this->input_name; }
        void set_output_name(string name){ this->output_name = name; }
        string get_output_name(){ return this->output_name; }
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input1 = temp;}
        int get_output(){return output_non;}
        void set_count_ff(int a){count_ff = a;}
private:
        string input_name;
        string output_name;
        string gate_name;
        Gate* input1;
};
// FlipFlop class has input name,  gate name, output name and input pointer that points previous gate.
class FlipFlop : public Gate{
    
public:
        int evaluate();
        void set_input1_name(string name){ this->input_name = name; }
        string get_input1_name(){ return this->input_name; }
        void set_output_name(string name){ this->output_name = name; }
        string get_output_name(){ return this->output_name; }
        string get_gate_name(){return this->gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input = temp;}
        void set_former_out(int a){former_out = a;}
        int get_former_out(){return former_out;}
        void set_count_ff(int a){count_ff = a;}
        int get_count_ff(){return count_ff;}
        int get_output(){return output_non;}

private:
        string input_name;
        string output_name;
        string gate_name;
        Gate* input;
};

// Decoder class has input1-input2 name, gate name, output-output1-output2-output3 names, input1 pointer and input2 pointer those point previous gates.
class Decoder : public Gate{
    
public:
        int evaluate();
        void set_input1_name(string name) {this->input1_name = name;}
        string get_input1_name() {return this->input1_name;}
        void set_input2_name(string name) {this->input2_name = name;}
        string get_input2_name() {return this->input2_name;}
        void set_output_name(string name){output_name = name;}
        string get_output_name(){return output_name;}
        void set_output1_name(string name){output1_name = name;}
        string get_output1_name(){return output1_name;}
        void set_output2_name(string name){output2_name = name;}
        string get_output2_name(){return output2_name;}
        void set_output3_name(string name){output3_name = name;}
        string get_output3_name(){return output3_name;}
        string get_gate_name(){return gate_name;}
        void set_gate_name(string name){this->gate_name = name;}
        void set_p_input1(Gate* temp){input1 = temp;}
        void set_p_input2(Gate* temp){input2 = temp;}
        int get_output(){return output_non;}
        void set_count_ff(int a){count_ff = a;}
private:
        string input1_name;
        string input2_name;
        string output_name;
        string output1_name;
        string output2_name;
        string output3_name;
        string gate_name;
        Gate* input1;
        Gate* input2;
};


#endif