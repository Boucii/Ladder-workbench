module Regfile(
  input wire clk,
  input wire rst,

  input wire[4:0] raddr1,
  output reg[63:0] rdata1,

  input wire[4:0] raddr2,
  output reg[63:0] rdata2,  

  input wire wen,
  input wire[4:0] waddr,
  input wire[63:0] wdata
);
  reg[63:0] regs[0:31];

  always @(posedge clk) begin
	  if(!rst) begin
		  if(wen && (waddr!=5'h0)) begin
			  regs[waddr]<=wdata;
		  end
	  end
  end

  always @(*) begin
	  if(rst) begin
		  rdata1=64'h0;
	  end else begin
		  rdata1=regs[raddr1];
	  end
  end
  
  always @(*) begin
	  if(rst) begin
		  rdata2=64'h0;
	  end else begin
		  rdata2=regs[raddr2];
	  end
  end

  import "DPI-C" function void set_gpr_ptr(input logic [63:0] a [0:31]);
  initial set_gpr_ptr(regs);  // rf为通用寄存器的二维数组变量


endmodule
