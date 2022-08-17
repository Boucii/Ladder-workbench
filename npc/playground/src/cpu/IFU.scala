package cpu 
import chisel3._
import chisel3.experimental._
import cpu.Defs._

class MemReadPort extends Bundle{
  val addr=Output(UInt(XLEN.W))
  val data=Input(UInt(32.W))
}
class IFU extends Module{
    val io=IO(new Bundle{
    val InstReadPort=new MemReadPort()
    val PcOut=Output(UInt(XLEN.W))
    val InstOut=Output(UInt(32.W))
    })
  //io.InstReadPort.addr:=0x80000000L.U(XLEN.W)
  //io.InstOut:=0.U

  io.InstReadPort.addr:=0.U

  //pc reg
  val pc=RegInit(0x80000000L.U(XLEN.W))
  pc:=pc+4.U
  io.PcOut:=pc
  io.InstReadPort.addr:=pc
    
  //fetch instruction
  val ir=io.InstReadPort.data
  io.InstOut:=ir
}
