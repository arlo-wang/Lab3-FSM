module lfsr_7 (
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [8:1] data_out
);

logic [8:1] sreg;

always_ff @(posedge clk or posedge rst)
    if (rst)
        sreg <= 8'b1;
    else if (en)
        sreg <= {sreg[8:2], sreg[3] ^ sreg[7]}

assign data_out = sreg;
endmodule
