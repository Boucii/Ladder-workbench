module automatic funcs
(
    input wire  stop,
    //input reg[63:0] regsout[0:31]

    //mem operation
    output wire[31:0] rdata,
    input wire[63:0] raddr,

    input wire[63:0] waddr,
    input wire[31:0] wdata,
    input wire[7:0] wmask

);

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction


import "DPI-C" function void pmem_read_dpi(
  input longint raddr, output longint rdata);
import "DPI-C" function void pmem_write_dpi(
  input longint waddr, input longint wdata, input byte wmask);
wire [63:0] rdata;
always @(*) begin
  pmem_read(raddr, rdata);
  pmem_write(waddr, wdata, wmask);
end


endmodule
