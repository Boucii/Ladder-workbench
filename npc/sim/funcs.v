module automatic funcs
(
    input wire  stop
    //input reg[63:0] regsout[0:31]
);

  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", stop);
    return stop;
  endfunction

endmodule
