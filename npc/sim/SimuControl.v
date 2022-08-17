module automatic SimuControl(
input wire EndOfSimulation
);
//import "DPI-C" context function void CheckEnd(logic a);
import "DPI-C" context function helloFromCpp();
export "DPI-C" function Check;
export "DPI-C" task check;
/*
always @(*) begin
   CheckEnd(EndOfSimulation); 
end
*/
function void Check();
        $display("\nhere comes dpi-c\n");
	//$display("EndOfSimulation is %0d", EndOfSimulation);
//	return EndOfSimulation;
endfunction

task check();
     $display("\nhere comes dpi-c\n");
     endtask

endmodule
