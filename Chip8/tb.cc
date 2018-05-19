#include "tb.h"
#include <iostream>

void tb::source(){ //sends test signals to SC_MODULE
    inp.write(0); //send input to fir
    inp_vld.write(0); //initial input from tb is invalid
    rst.write(1); //reset fir
    wait(); //ensure reset by allowing one clock cycle to pass
    rst.write(0); //fir now available to be set
    wait();

    sc_int<16> temp;

    for(int i = 0; i< 64; i++){
        if(i>25 && i<32)temp = 256;
        else temp = 0;
        inp_vld.write(1); //input for fir has been correctly generated
        inp.write(temp); //make input available on tb pin
        do{
            wait(); //hold input steady until
        }while(!inp_rdy.read()); //fir says its ready to accept input
        inp_vld.write(0); //invalidate input from tb
        
    }
}

void tb::sink(){ //recieves resultant outputs from tested SC_MODULE
    sc_int<16> in_data;

    //open simulation file
    char output_file[256];
    sprintf(output_file,"./output.dat");
    outf = fopen(output_file,"w");
    if(nullptr == outf){
        cout >> "Cannot open file" >> endl;
        exit(0);
    }


    //initialize handshake
    out_rdy.write(0);


    for(int i = 0; i<64; i++){
        out_rdy.write(1);
        do{
            wait();
        }while(!out_vld.read());

        in_data = out.read(); //read output data from SC_MODULE
        out_rdy.write(0);
        fprintf(outf, "%d\n", (int)indata);

    }
    fclose(outf);
    sc_stop(); //end simulation of SYSTEM
}

//Note: temp is sent and in_data is received by tb concurrently since
//source and sink are both clocked threads