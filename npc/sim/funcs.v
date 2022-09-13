import "DPI-C" function void pmem_read_dpi(input longint raddr, output longint rdata);
import "DPI-C" function void pmem_write_dpi(input longint waddr, input longint wdata, input byte wmask);
module automatic funcs
(
    input wire  stop,
    //input reg[63:0] regsout[0:31]

    //mem operation
    input wire men,
    input wire mwen,
    output wire[63:0] rdata,
    input wire[63:0] raddr,

    input wire[63:0] waddr,
    input wire[63:0] wdata,
    input wire[7:0] wmask

);

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction

  reg [63:0] rdata_in;
  always @(*) begin
    if(men && !mwen) begin
        pmem_read_dpi(raddr, rdata_in);
    end else begin
       rdata_in = 64'b0;	
       end
       if(men && mwen) begin
    pmem_write_dpi(waddr, wdata, wmask);
    end
    end
  assign rdata=rdata_in;
endmodule
