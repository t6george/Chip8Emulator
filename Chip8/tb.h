#include <systemc.h>

SC_MODULE(tb){
    sc_in<bool> rst;
    sc_in<bool> clk;
    sc_out<sc_int<16>> inp;
    sc_in<sc_int<16>> out;

    void source();
    void sink();

    SC_CTOR(tb){
        SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(sink, clk.pos());
    }
};