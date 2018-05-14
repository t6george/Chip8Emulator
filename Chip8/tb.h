#include <systemc.h>

SC_MODULE(tb){
    sc_in<bool> rst;
    sc_in<bool> clk;
    sc_in<sc_int<16>> out; 
    sc_in<bool> inp_rdy; //fir ready to accept input
    sc_in<bool> out_vld;

    sc_out<sc_int<16>> inp;
    sc_out<bool> inp_vld; //testbench ready to output 
    sc_out<bool> out_rdy;

    void source();
    void sink();

    SC_CTOR(tb){
        SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(sink, clk.pos());
    }
};