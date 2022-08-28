package cpu2
import chisel3._
import chisel3.experimental._
import cpu2.Defs._


class Regfile extends Module{
  val io=IO(new Bundle{
  val raddr1=Input(UInt(5.W))
  val rdata1=Output(UInt(XLEN.W))
  val raddr2=Input(UInt(5.W))
  val rdata2=Output(UInt(XLEN.W))
  val wen=Input(Bool())
  val waddr=Input(UInt(5.W))
  val wdata=Input(UInt(XLEN.W))
  })
  //val regs=Wire(RegInit(Vec(32, UInt(XLEN.W))))
  val regs=Reg(Vec(32,UInt(XLEN.W)))
  /*
  for (i<-0 to 32-1) {
      regs(i):=0.U
  }
  */

  val data1=Mux((io.raddr1===0.U),0.U,regs(io.raddr1))
  val data2=Mux((io.raddr2===0.U),0.U,regs(io.raddr2))
  dontTouch(regs)
  io.rdata1:=data1
  io.rdata2:=data2
  when(io.wen && io.waddr.orR) {
  regs(io.waddr) := io.wdata
  }
  }
