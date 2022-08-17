package cpu 
import chisel3._
import chisel3.experimental._
import cpu.Defs._
import chisel3.util._


class IDU extends Module{
  val io=IO(new Bundle{
    val PcIn=Input(UInt(XLEN.W))
    val InstIn=Input(UInt(32.W))
    val Op=Output(UInt(32.W))
    val Imm=Output(UInt(XLEN.W))
    val RegWrite=Output(Bool())
    val RegWriteAddr=Output(UInt(5.W))
    val PcOut=Output(UInt(XLEN.W))
    val InstOut=Output(UInt(32.W))
    val RegRead1=Output(UInt(5.W))
    val RegRead2=Output(UInt(5.W))
  })
  val ADD=1.U(32.W)
  val EBREAK=2.U(32.W)
    
  io.RegWrite:=0.B
  io.RegWriteAddr:=0.U
  io.Op:=0.U
  io.Imm:=0.U
  io.RegRead1:=0.U
  io.RegRead2:=0.U
  io.PcOut:=io.PcIn
  io.InstOut:=io.InstIn

  val inst=io.InstIn
  val rd=inst(11,7)
  val rs1=inst(19,15)
  val rs2=inst(24,20)

  val Signext=Mux(inst(31)===1.B,inst(31,20),Cat(0xfffffffffffffL.U,inst(31,20)))
  val Usignext=inst(31,12)<<12
  when(io.InstIn(14,12)===0.U&&(inst(6,0)==="b0010011".U)){//addi
    println(p"oooo$ADD\n")
    //println("oooo${ADD}")
    io.Op:=ADD
    io.Imm:=Signext
    io.RegWrite:=1.U
    io.RegRead1:=rs1
    io.RegWriteAddr:=rd

  }.elsewhen(io.InstIn===0x00100073.U){ //ebreak
    io.Op:=EBREAK
    io.RegWrite:=0.U
    

  }.otherwise{

      printf(p"aaao${inst(6,0)}\n")
      //println("aaao${ADD}")
  }
  }
