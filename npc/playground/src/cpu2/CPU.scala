package cpu2 
import chisel3._
import chisel3.experimental._
import chisel3.util._

class TOP extends Module{
    val io=IO(new Bundle{
    //dmemory ports
    val Mwout=Output((UInt(1.W)))
    val Maddr=Output(UInt(64.W))
    val Men=Output(UInt(1.W))
    val Mlen=Output(UInt(32.W))
    val MdataIn=Input(UInt(64.W))
    val MdataOut=Output(UInt(64.W))

    //imemory port
    val InstAddr=Output(UInt(64.W))
    val InstIn=Input(UInt(32.W))  //still has a 32 bit wide port for now
  })
  val pc=RegInit(0x80000000L.U(64.W))//todo :modify pc to branches
  val regs=Module(new Regfile)
  val stopflag=Wire(UInt(1.W))

  stopflag:=0.U
  regs.io.waddr:=0.U
  regs.io.raddr1:=0.U
  regs.io.raddr2:=0.U
  regs.io.wdata:=0.U
  regs.io.wen:=0.U

  //instruction fetch
  val inst=io.InstIn
  io.InstAddr:=Cat(0x00000000.U,pc)
  //inst:=io.InstIn

  val dpc=Wire(UInt(32.W))
  dpc:=pc+4.U

  //decode
  val pt0=inst(6,0)
  val pt1=inst(11,7)
  val pt2=inst(16,12)
  val pt3=inst(19,17)
  val pt4=inst(24,20)
  val pt5=inst(29,25)
  val pt6=inst(31,30)
  
  val rs1=Wire(UInt(64.W))
  val rs2=Wire(UInt(64.W))
  val rd=Wire(UInt(64.W))

  rs1:=inst(19,15)
  rs2:=inst(24,20)
  rd:=inst(11,7)

  //imm decode ext
  val immI=Mux(inst(31)===1.U,inst(31,20),Cat(0xfffffffffffffL.U,inst(31,20)))
  val immS=Cat(Mux(inst(31)===1.U,inst(31,25),Cat(0xff.U,inst(31,25)))<<5,inst(11,7))
  val immU=inst(31,12)<<12
  val immJ=Cat(Cat(Cat(Mux(inst(31)===1.U,inst(31,31),Cat(0xff.U,inst(31,31)))<<20,inst(19,12)<<12),inst(20,20)<<11),inst(30,21)<<1) //optimization?
  val immB=Cat(Cat(Cat(Mux(inst(31)===1.U,inst(31,31),Cat(0xff.U,inst(31,31)))<<12,inst(7,7)<<11),inst(30,25)<<5),inst(11,8)<<1)

  //decode to src and dest
  val dest=Wire(UInt(64.W))
  val src1=Wire(UInt(64.W))
  val src2=Wire(UInt(64.W))

  dest:=0.U
  src1:=0.U
  src2:=0.U
  dest:=rd//default is rd,todo and change

  //itermidiate
  val intermediate=Wire(UInt(64.W))
  intermediate:=0.U

  //initiallization
  io.Mwout:=0.U//rename :todo
  io.Maddr:=0.U
  io.Men:=0.U
  io.Mlen:=0.U
  io.MdataOut:=0.U

when(pt3==="b011".U && pt5==="b01000".U && pt6==="b11".U){    // sd     
  dest:=immS
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1;src2:=regs.io.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=8.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b010".U && pt5==="b01000".U && pt6==="b11".U){    // sw     
  dest:=immS
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1;src2:=regs.io.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=4.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b001".U && pt5==="b01000".U && pt6==="b11".U){    // sh     
  dest:=immS
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1;src2:=regs.io.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=2.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b000".U && pt5==="b01000".U && pt6==="b11".U){    // sb     
  dest:=immS
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1;src2:=regs.io.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=1.U
  io.MdataOut:=src2
}.elsewhen(pt0==="b0000000".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // add    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1+src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // addw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=Mux((src1+src2)(31)===1.U,(src1+src2),Cat(0xffffffffL.U,(src1+src2)))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b111".U && pt5==="b01100".U && pt6==="b11".U){    // and    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1&src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b01100".U && pt6==="b11".U){    // sll    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=(src1(63,0)<<src2(5,0))
  regs.io.waddr:=dest(18,0)
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b01110".U && pt6==="b11".U){    // sllw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=(src1(63,0)<<src2(4,0))(31,0)
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b010".U && pt5==="b01100".U && pt6==="b11".U){    // slt    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=(Mux(src1.asSInt<src2.asSInt,0.U,1.U)).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b011".U && pt5==="b01100".U && pt6==="b11".U){    // sltu   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
   regs.io.wdata:=Mux(src1.asUInt<src2.asUInt,0.U,1.U) 
   regs.io.waddr:=dest
   regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // sra    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=(src1.asSInt>>src2(5,0).asUInt).asUInt//????
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // sraw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=(src1.asSInt>>src2(4,0)).asUInt
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // srl    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1.asUInt>>src2(5,0).asUInt//????
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // srlw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=(src1.asUInt>>src2(4,0)).asUInt
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // sub    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1+src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // subw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=Mux((src1-src2)(31)===1.U,(src1-src2),Cat(0xffffffffL.U,(src1-src2)))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b100".U && pt5==="b01100".U && pt6==="b11".U){    // xor    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1^src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b110".U && pt5==="b01110".U && pt6==="b11".U){    // remw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b111".U && pt5==="b01110".U && pt6==="b11".U){    // remuw  
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b111".U && pt5==="b01100".U && pt6==="b11".U){    // remu   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1%src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b110".U && pt5==="b01100".U && pt6==="b11".U){    // rem    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=(src1.asSInt%src2.asSInt).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b110".U && pt5==="b01100".U && pt6==="b11".U){    // or     
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1|src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // mul    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1*src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b001".U && pt5==="b01100".U && pt6==="b11".U){    // mulh   //??
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=src1*src2
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b010".U && pt5==="b01100".U && pt6==="b11".U){    // mulhsu 
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b011".U && pt5==="b01100".U && pt6==="b11".U){    // mulhu  
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // mulw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=Mux((src1*src2)(31)===1.U,(src1*src2),Cat(0xffffffffL.U,(src1*src2)))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b100".U && pt5==="b01100".U && pt6==="b11".U){    // div    
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=(src1.asSInt/src2.asSInt).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // divu   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  regs.io.wdata:=src1/src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // divuw  
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=src1(31,0).asUInt/src2(31,0).asUInt
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b100".U && pt5==="b01110".U && pt6==="b11".U){    // divw   
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  intermediate:=(src1(31,0).asSInt/src2(31,0).asSInt)
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00100".U && pt6==="b11".U){    // addi   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=src1+src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00110".U && pt6==="b11".U){    // addiw  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  intermediate:=(src1+src2)(31,0)
  regs.io.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffffL.U,intermediate))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b111".U && pt5==="b00100".U && pt6==="b11".U){    // andi   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=src1&src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b00000".U && pt6==="b11".U){    // ld     
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=8.U
  regs.io.wdata:=io.MdataIn
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b010".U && pt5==="b00000".U && pt6==="b11".U){    // lw     
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.io.wdata:=Mux((io.MdataIn)(31)===1.U,(io.MdataIn),Cat(0xffffffffL.U,(io.MdataIn(31,0))))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b001".U && pt5==="b00000".U && pt6==="b11".U){    // lh     
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.io.wdata:=Mux((io.MdataIn)(15)===1.U,(io.MdataIn),Cat(0xffffffffffffL.U,(io.MdataIn(15,0))))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00000".U && pt6==="b11".U){    // lb     
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.io.wdata:=Mux((io.MdataIn)(7)===1.U,(io.MdataIn),Cat(0xffffffffffffL.U,(io.MdataIn(7,0))))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b110".U && pt5==="b00000".U && pt6==="b11".U){    // lwu    
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.io.wdata:=io.MdataIn
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b101".U && pt5==="b00000".U && pt6==="b11".U){    // lhu    
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=2.U
  regs.io.wdata:=io.MdataIn
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b100".U && pt5==="b00000".U && pt6==="b11".U){    // lbu    
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=1.U
  regs.io.wdata:=io.MdataIn
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b00100".U && pt6==="b11".U){    // slli   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=(src1(63,0)<<src2(5,0))
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b00110".U && pt6==="b11".U){    // slliw  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt3==="b010".U && pt5==="b00100".U && pt6==="b11".U){    // slti   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=(Mux(src1.asSInt<src2.asSInt,0.U,1.U)).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b00100".U && pt6==="b11".U){    // sltiu  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=Mux(src1<src2,0.U,1.U)
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b00100".U && pt6==="b11".U){    // srai   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=(src1(63,0).asSInt>>src2).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b00110".U && pt6==="b11".U){    // sraiw  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=(src1.asSInt>>src2).asUInt
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b00100".U && pt6==="b11".U){    // srli   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b00110".U && pt6==="b11".U){    // srliw  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt3==="b100".U && pt5==="b00100".U && pt6==="b11".U){    // xori   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=src1^src2// is the symbol right?
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b11001".U && pt6==="b11".U){    // jalr   
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  pc:=src1+src2
  regs.io.wdata:=pc+4.U
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b110".U && pt5==="b00100".U && pt6==="b11".U){    // ori    
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
  regs.io.wdata:=src1|src2
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b11100".U && pt6==="b11".U){    // csrrc  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt3==="b111".U && pt5==="b11100".U && pt6==="b11".U){    // csrrci 
}.elsewhen(pt3==="b010".U && pt5==="b11100".U && pt6==="b11".U){    // csrrs  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt3==="b110".U && pt5==="b11100".U && pt6==="b11".U){    // csrrsi 
}.elsewhen(pt3==="b001".U && pt5==="b11100".U && pt6==="b11".U){    // csrrw  
  regs.io.raddr1:=rs1
  src1:=regs.io.rdata1
  src2:=immI
}.elsewhen(pt3==="b101".U && pt5==="b11100".U && pt6==="b11".U){    // csrrwi 
}.elsewhen(pt0==="b0011000".U && pt1==="b00010".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // mret   
}.elsewhen(pt5==="b00101".U && pt6==="b11".U){    // auipc  
  src1:=immU
  regs.io.wdata:=src1+pc
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt5==="b01101".U && pt6==="b11".U){    // lui    
  src1:=immU
  regs.io.wdata:=src1
  regs.io.waddr:=dest
  regs.io.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b11000".U && pt6==="b11".U){    // beq    
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  dpc:=(Mux((src1.asSInt===src2.asSInt),pc,pc+dest)).asUInt
}.elsewhen(pt3==="b101".U && pt5==="b11000".U && pt6==="b11".U){    // bge    
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  pc:=(Mux((src1.asSInt>=src2.asSInt),pc,pc+dest)).asUInt
}.elsewhen(pt3==="b111".U && pt5==="b11000".U && pt6==="b11".U){    // bgeu   
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  pc:=Mux((src1.asUInt>=src2.asUInt),pc,pc+dest)
}.elsewhen(pt3==="b100".U && pt5==="b11000".U && pt6==="b11".U){    // blt    
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  dpc:=(Mux((src1.asUInt<src2.asUInt),pc,pc+dest)).asUInt
}.elsewhen(pt3==="b110".U && pt5==="b11000".U && pt6==="b11".U){    // bltu   
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
}.elsewhen(pt3==="b001".U && pt5==="b11000".U && pt6==="b11".U){    // bne    
  dest:=immB
  regs.io.raddr1:=rs1
  regs.io.raddr2:=rs2
  src1:=regs.io.rdata1
  src2:=regs.io.rdata2
  dpc:=Mux((src1.asUInt=/=src2.asUInt),pc,pc+dest)
}.elsewhen(pt5==="b11011".U && pt6==="b11".U){    // jal    
  src1:=immJ
  regs.io.wdata:=pc+4.U
  regs.io.waddr:=dest
  regs.io.wen:=1.U
  dpc:=pc+src1
}.elsewhen(pt0==="b0000000".U && pt1==="b00000".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // ecall  
}.elsewhen(pt0==="b0000000".U && pt1==="b00001".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // ebreak 
  stopflag:=1.U
}.otherwise{    // inv 
  stopflag:=1.U
}
  //update pc reg
  pc:=Mux(stopflag===1.U,dpc,pc)
}
