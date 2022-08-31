module automatic funcs
(
    input wire  stop
);
  import "DPI-C" function void set_gpr_ptr(input logic [63:0] a []);
  initial set_gpr_ptr(rf);  // rf为通用寄存器的二维数组变量

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction

endmodule
