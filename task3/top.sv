module top (
    input logic [15:0]  N,
    input logic         en,
    input logic         rst,
    input logic         clk,
    output logic        fsm_en,
    output logic [7:0]  data_out
);

logic clktick_tick;


clktick clktick_inst (
    .N      (N),
    .en     (en),
    .rst    (rst),
    .clk    (clk),
    .tick   (clktick_tick)
);

always_comb begin
    fsm_en = clktick_tick ? 1 : 0;
end

f1_fsm f1_sm (
    .rst        (rst),
    .en         (fsm_en),
    .clk        (clk),
    .data_out   (data_out)
);

endmodule