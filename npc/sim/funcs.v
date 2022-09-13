import "DPI-C" function void pmem_read_dpi(input longint raddr, output longint rdata);
import "DPI-C" function void pmem_write_dpi(input longint waddr, input longint wdata, input byte wmask);
module automatic funcs
(
    input wire clk,
    input wire  stop,
    //input reg[63:0] regsout[0:31]

    //mem operation
    input wire men,
    input wire mwen,
    output reg[63:0] rdata,
    input reg[63:0] raddr,

    input reg[63:0] waddr,
    input reg[63:0] wdata,
    input reg[7:0] wmask

);

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction

  reg [63:0] rdata_in;
  reg done;
  always @(*) begin
       		rdata_in = 64'b0;	
	  if(done==1'b0) begin
	  if(men && !mwen) begin
		$display("From verilog,raddr is: %H.", raddr);
        	pmem_read_dpi(raddr, rdata_in);
	  end else begin 
       		rdata_in = 64'b0;	
       		end
    if(men && mwen) 
          pmem_write_dpi(waddr, wdata, wmask);
    end
    done=1'b1;
    end

    always @(negedge clk) begin
	    done=1'b0;
    end


  assign rdata=rdata_in;
endmodule
