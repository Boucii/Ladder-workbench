package cpu2
import chisel3.stage._
import cpu2._
import chisel3._

object GenVerilog extends App {
  //(new chisel3.stage.ChiselStage).emitVerilog(new TOP)
//(new chisel3.stage.ChiselStage).execute(args, Seq(chisel3.stage.ChiselGeneratorAnnotation(() => new TOP())))
    (new chisel3.stage.ChiselStage).emitVerilog(new TOP, args)
}
/*
final def GenVerilog(gen:=>RawModule,args:Array[String]=A){
  (new chisel3.stage.ChiselStage).emitVerilog(new TOP)))
}
*/
