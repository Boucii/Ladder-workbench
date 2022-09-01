module automatic funcs
(
    input wire  stop,
    input reg[63:0] regsout[0:31]
);
  import "DPI-C" function void set_gpr_ptr(input logic [63:0] a []);
  initial set_gpr_ptr(regsout);  // rf为通用寄存器的二维数组变量

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction

endmodule
