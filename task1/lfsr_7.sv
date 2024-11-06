module lfsr_7 (
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [7:1] data_out
);

logic [7:1] sreg;

always_ff @(posedge clk or posedge rst) begin
    if (rst) begin
        sreg <= 7'b0000001;
    end else if (en) begin
        sreg <= {sreg[6:1], sreg[7] ^ sreg[3]}; 
    end
end

assign data_out = sreg;

endmodule
