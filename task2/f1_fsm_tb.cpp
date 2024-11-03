#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_fsm.h"

#include "vbuddy.cpp"
#include <chrono>
#include <thread>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

#define ADDRESS_WIDTH 8
#define ROM_SZ 256

int main(int argc, char **argv, char **env) {
    // i counts the number of clock cycles to simulate.
    // clk is the module clock signal.
    int simcyc;
    int tick;

    std::srand(std::time(nullptr));
    int wait_time = 1000 + (std::rand() % 1001); // between 1000ms to 2000ms

    Verilated::commandArgs(argc, argv);
    // init top verilaog instance
    Vf1_fsm* top = new Vf1_fsm;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("f1_fsm.vcd");

    if (vbdOpen()!=1) return(-1);
    vbdHeader("L3: Formula1");
    //vbdSetMode(1);        // Flag mode set to one-shot

    // initialize simulation inputs
    top->clk = 1;
    top->en = 0;
    top->rst = 0;
    
    vbdSetMode(1);

    for (simcyc = 0; simcyc <= 7; simcyc++) {
        // dump variables into VCD file and toggle clock
        for (tick = 0; tick < 2; tick++) {
            tfp->dump (2*simcyc+tick);  // unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        } 

        for (int i = 0; i < 4; i++) {
            while (top->en != 1) top->en = vbdFlag();

            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            top->en = 1;
            vbdBar(top->data_out & 0xFF);
        }

        top->en = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        top->rst = 1;
        top->rst = 0;

        if ((Verilated::gotFinish()) || (vbdGetkey()=='q'))  exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
