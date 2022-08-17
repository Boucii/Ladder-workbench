package cpu 
import chisel3._
import chisel3.experimental._
import cpu.Defs._
import cpu._


class TOP extends Module{
  val io=IO(new Bundle{
    val InstReadPort=new MemReadPort()
  })
  val Regfile0=Module(new Regfile)
  val IFU0=Module(new IFU)
  val IDU0=Module(new IDU)
  val EXU0=Module(new EXU)
  val UseSimuControl0=Module(new UseSimuControl)
  
  //io.InstReadPort.addr:=IFU0.io.
  io.InstReadPort<>IFU0.io.InstReadPort


  IDU0.io.PcIn:=IFU0.io.PcOut
  IDU0.io.InstIn:=IFU0.io.InstOut
   
  EXU0.io.PcIn:=IDU0.io.PcOut
  EXU0.io.InstIn:=IDU0.io.InstOut
  EXU0.io.OpIn:=IDU0.io.Op
  EXU0.io.ImmIn:=IDU0.io.Imm
  Regfile0.io.wen:=IDU0.io.RegWrite
  Regfile0.io.waddr:=IDU0.io.RegWriteAddr
  Regfile0.io.raddr1:=IDU0.io.RegRead1
  Regfile0.io.raddr2:=IDU0.io.RegRead2
   
  Regfile0.io.wdata:=EXU0.io.RegWriteData
  EXU0.io.Reg1In:=Regfile0.io.rdata1
  EXU0.io.Reg2In:=Regfile0.io.rdata2

  UseSimuControl0.io.EndOfSimulationIn:=EXU0.io.EndOfSimulation
}
