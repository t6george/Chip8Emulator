#include "tb.h"

void tb::source(){ //sends test signals to SC_MODULE
    inp.write(0); //send input to fir
    rst.write(1); //reset fir
    wait(); //ensure reset by allowing one clock cycle to pass
    rst.write(0); //fir now available to be set
    wait();

    sc_int<16> temp;

    for(int i = 0; i< 64; i++){
        if(i>25 && i<32)temp = 256;
        else temp = 0;
        inp.write(temp); //generates a square wave input
        wait();
    }
}

void tb::sink(){ //recieves resultant outputs from tested SC_MODULE
    sc_int<16> in_data;

    for(int i = 0; i<64; i++){
        in_data = out.read(); //read output data from SC_MODULE
        wait();
    }
    sc_stop(); //end simulation of SYSTEM
}

//Note: temp is sent and in_data is received by tb concurrently since
//source and sink are both clocked threads