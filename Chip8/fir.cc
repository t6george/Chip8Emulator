#include "fir.h"

void fir::fir_main(){
  //reset
  out.write(0);
  wait(); //wait for one clock cycle

  //functionality
  while(true){
    //shift register
    sc_int<16> taps[5];
    for(int i = 4; i >= 1; i--){
      taps[i] = taps[i-1];
    }
    taps[0] = inp.read();
    sc_int<16> sum;
    for(int i = 0; i < 5; i++){
      sum += taps[i];
    }
    out.write(sum);
    wait();
  }
}