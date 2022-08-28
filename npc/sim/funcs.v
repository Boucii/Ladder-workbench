module automatic funcs;
(
    input wire  stop,
);
  export "DPI-C" function Check;
  function bit Check();
    $display("Check result: stop= %0d", b);
    return stop;
  endfunction

endmodule
