#include "fir.h"
#include <systemc.h>

void fir::fir_main(){
  //reset
  out.write(0);
  wait(); //wait for one clock cycle

  //initialize handshake
  inp_rdy.write(0); //not ready to accept tb input
  out_vld.write(0); //not ready to generate a valid output yet

  //functionality
  while(true){
    sc_int<16> in_val;
    sc_int<16> sum;

    inp_rdy.write(1);

    do{
      wait();
    }while(!inp_vld.read());

    in_val = inp.read();
    inp_rdy.write(0);

    //shift register
    sc_int<16> taps[5];
    for(int i = 4; i >= 1; i--){
      taps[i] = taps[i-1];
    }
    taps[0] = in_val;
    
    for(int i = 0; i < 5; i++){
      sum += taps[i];
    }

    out_vld.write(1);
    out.write(sum);
    do{
      wait();
    }while(!out_rdy.read());
    out_vld.write(0);
  }
}