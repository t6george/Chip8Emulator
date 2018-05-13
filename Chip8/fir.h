#include <systemc.h>

SC_MODULE(fir){
  sc_in<bool> rst;
  sc_in<bool> clk;
  sc_in<sc_int<16>> inp;
  sc_out<sc_int<16>> out;

  void fir_main();
  
  SC_CTOR(fir){
    SC_CTHREAD(fir_main, clk.pos());
    reset_signal_is(rst,true); //put in line right after cthread
  }

}