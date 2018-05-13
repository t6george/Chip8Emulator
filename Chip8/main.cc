#include <systemc.h>
#include <"fir.h>
#include <"tb.h>

SC_MODULE(SYSTEM){
    tb *tb0;
    fir *fir0;

    sc_signal<bool> rst_sig;
    sc_signal<sc_int<16>> inp_sig;
    sc_signal<sc_int<16>> out_sig;

    sc_clock clk_sig;

    SC_CTOR(SYSTEM): tb0(new tb("tb0")), fir0(new fir("fir0 ")),clk_sig("clk_sig",10,SC_NS){ //10 nanosecond period
        tb0->clk(clk_sig);
        fir0->clk(clk_sig);
        
        tb0->rst(rst_sig);
        fir0->rst(rst_sig);

        tb0->inp(inp_sig);
        fir0->inp(inp_sig);

        tb0->out(out_sig);
        fir0->out(out_sig);

    }

    ~SYSTEM(){
        delete tb0;
        delete fir0;
    }
};

SYSTEM *top = NULL;

int sc_main(int argc, char* argv[]){
    top = new SYSTEM("top");
    sc_start(); //start simulation of overhead system
    return 0;
}