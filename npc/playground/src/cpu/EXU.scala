package cpu 
import chisel3._
import chisel3.experimental._
import cpu.Defs._
class EXU extends Module{
  val io=IO(new Bundle{
    val PcIn=Input(UInt(XLEN.W))
    val InstIn=Input(UInt(32.W))

    val OpIn=Input(UInt(32.W))
    val Reg1In=Input(UInt(XLEN.W))
    val Reg2In=Input(UInt(XLEN.W))
    val ImmIn=Input(UInt(XLEN.W))
  
    val RegWriteData=Output(UInt(XLEN.W))

    val EndOfSimulation=Output(Bool())
  })
    dontTouch(io.RegWriteData)
    val ADD=1.U
    val EBREAK=2.U 
    io.RegWriteData:=0.U
    io.EndOfSimulation:=0.U
    
    when(io.OpIn===ADD){
      io.RegWriteData:=io.Reg1In+io.ImmIn
    }.elsewhen(io.OpIn===EBREAK){
      io.EndOfSimulation:=1.B
      
    }
}
