package cpu2 
import chisel3._
import chisel3.experimental._
import cpu.Defs._

class DECODE extends Module{
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
  val stopflag=0.U(1.W)

  //instruction fetch
  val inst=0.U(64.W)
  inst:=InstIn

  val dpc=pc+4

  //decode
  val pt0=inst(6,0)
  val pt1=inst(11,7)
  val pt2=inst(16,12)
  val pt3=inst(19,17)
  val pt4=inst(24,20)
  val pt5=inst(29,25)
  val pt6=inst(31,30)
  
  val rs1=inst(19,15)
  val rs2=inst(24,20)
  val rd=inst(11,7)

  //imm decode ext
  val immI=Mux(inst(31)===1.B,inst(31,20),Cat(0xfffffffffffffL.U,inst(31,20)))
  val immS=Cat(Mux(inst(31==1.B,inst(31,25),Cat(0xff.U,inst(31,25))))<<5,inst(11,7))
  val immU=inst(31,12)<<12
  val immJ=Cat(Cat(Cat(Mux(inst(31)===1.B,inst(31,31),Cat(0xff.U,inst(31,31)))<<20,inst(19,12)<<12),inst(20,20)<<11),inst(30,21)<<1) //optimization?
  val immB=Cat(Cat(Cat(Mux(inst(31)===1.B,inst(31,31),Cat(0xff.U,inst(31,31)))<<12,inst(7,7)<<11),inst(30,25)<<5),inst(11,8)<<1)

  //decode to src and dest
  val dest=0.U(64.W)
  val src1=0.U(64.W)
  val src2=0.U(64.W)
  
  //todo:connect src to regs or mem

  dest:=rd//default is rd,todo and change
  //itermidiate
  val intermediate=0.U(64.W)

  //initiallization
  io.Mwout=0.U//rename :todo
  io.Maddr=0.U
  io.Men=0.U
  io.Mlen=0.U
  io.MdataIn=0.U
  io.MdataOut=0.U

when(pt3==="b011".U && pt5==="b01000".U && pt6==="b11".U){    // sd     
  dest:=immS
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1;src2:=regs.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=8.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b010".U && pt5==="b01000".U && pt6==="b11".U){    // sw     
  dest:=immS
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1;src2:=regs.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=4.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b001".U && pt5==="b01000".U && pt6==="b11".U){    // sh     
  dest:=immS
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1;src2:=regs.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=2.U
  io.MdataOut:=src2
}.elsewhen(pt3==="b000".U && pt5==="b01000".U && pt6==="b11".U){    // sb     
  dest:=immS
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1;src2:=regs.rdata2
  io.Men:=1.U
  io.Mwout:=1.U
  io.Maddr:=src1+dest
  io.Mlen:=1.U
  io.MdataOut:=src2
}.elsewhen(pt0==="b0000000".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // add    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1+src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // addw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=Mux((src1+src2)(31)===1.U,(src1+src2),Cat(0xffffffff.U,(src1+src2)))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b111".U && pt5==="b01100".U && pt6==="b11".U){    // and    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1&src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b01100".U && pt6==="b11".U){    // sll    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1<<src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b01110".U && pt6==="b11".U){    // sllw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  intermediate=(src1<<src2(4,0))(31,0)
  regs.wdata:=(intermediate(31)===1,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b010".U && pt5==="b01100".U && pt6==="b11".U){    // slt    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=Mux(src1.S<src2.S,0.U,1.U)
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b011".U && pt5==="b01100".U && pt6==="b11".U){    // sltu   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
   regs.wdata:=Mux(src1.U<src2.U,0.U,1.U) 
   regs.waddr:=dest
   regs.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // sra    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1.S>>src2(5,0).U//????
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // sraw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  intermediate=src1.S>>src2(4,0)
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // srl    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1.U>>src2(5,0).U//????
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // srlw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  intermediate=src1.U>>src2(4,0)
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // sub    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1+src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // subw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=Mux((src1-src2)(31)===1.U,(src1-src2),Cat(0xffffffff.U,(src1-src2)))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b100".U && pt5==="b01100".U && pt6==="b11".U){    // xor    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1^src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b110".U && pt5==="b01110".U && pt6==="b11".U){    // remw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b111".U && pt5==="b01110".U && pt6==="b11".U){    // remuw  
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b111".U && pt5==="b01100".U && pt6==="b11".U){    // remu   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1%src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b110".U && pt5==="b01100".U && pt6==="b11".U){    // rem    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1.S%src2.S
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b110".U && pt5==="b01100".U && pt6==="b11".U){    // or     
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1|src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b000".U && pt5==="b01100".U && pt6==="b11".U){    // mul    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1*src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b001".U && pt5==="b01100".U && pt6==="b11".U){    // mulh   //??
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  intermediate:=src1*src2
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b010".U && pt5==="b01100".U && pt6==="b11".U){    // mulhsu 
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b011".U && pt5==="b01100".U && pt6==="b11".U){    // mulhu  
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
}.elsewhen(pt0==="b0000001".U && pt3==="b000".U && pt5==="b01110".U && pt6==="b11".U){    // mulw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=Mul((src1*src2)(31)===1,(src1*src2),Cat(0xffffffff.U,(src1*src2)))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b100".U && pt5==="b01100".U && pt6==="b11".U){    // div    
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1.S/src2.S
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b101".U && pt5==="b01100".U && pt6==="b11".U){    // divu   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  regs.wdata:=src1/src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b101".U && pt5==="b01110".U && pt6==="b11".U){    // divuw  
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  intermediate=src1(31,0).U/src2(31,0).U
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000001".U && pt3==="b100".U && pt5==="b01110".U && pt6==="b11".U){    // divw   
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  val intermediate=src1(31,0).S/src2(31,0).S
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00100".U && pt6==="b11".U){    // addi   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1+src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00110".U && pt6==="b11".U){    // addiw  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  intermediate=(src1+src2)(31,0)
  regs.wdata:=Mux(intermediate(31)===1.U,intermediate,Cat(0xffffffff.U,intermediate))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b111".U && pt5==="b00100".U && pt6==="b11".U){    // andi   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1&src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b00000".U && pt6==="b11".U){    // ld     
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=8.U
  regs.wdata:=MdataIn
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b010".U && pt5==="b00000".U && pt6==="b11".U){    // lw     
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.wdata:=Mux((MdataIn)(31)===1.U,(MdataIn),Cat(0xffffffff.U,(MdataIn(31,0))))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b001".U && pt5==="b00000".U && pt6==="b11".U){    // lh     
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.wdata:=Mux((MdataIn)(15)===1.U,(MdataIn),Cat(0xffffffffffffL.U,(MdataIn(15,0))))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b00000".U && pt6==="b11".U){    // lb     
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.wdata:=Mux((MdataIn)(7)===1.U,(MdataIn),Cat(0xffffffffffffL.U,(MdataIn(7,0))))
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b110".U && pt5==="b00000".U && pt6==="b11".U){    // lwu    
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=4.U
  regs.wdata:=MdataIn
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b101".U && pt5==="b00000".U && pt6==="b11".U){    // lhu    
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=2.U
  regs.wdata:=MdataIn
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b100".U && pt5==="b00000".U && pt6==="b11".U){    // lbu    
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  io.Men:=1.U
  io.Maddr:=src1+src2
  io.Mlen:=1.U
  regs.wdata:=MdataIn
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b000000?".U && pt3==="b001".U && pt5==="b00100".U && pt6==="b11".U){    // slli   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1<<src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0000000".U && pt3==="b001".U && pt5==="b00110".U && pt6==="b11".U){    // slliw  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt3==="b010".U && pt5==="b00100".U && pt6==="b11".U){    // slti   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=Mux(src1.S<src2.S,0.U,1.U)
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b00100".U && pt6==="b11".U){    // sltiu  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=Mux(src1<src2,0.U,1.U)
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b010000?".U && pt3==="b101".U && pt5==="b00100".U && pt6==="b11".U){    // srai   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1.S>>src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b0100000".U && pt3==="b101".U && pt5==="b00110".U && pt6==="b11".U){    // sraiw  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1.S>>src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt0==="b000000?".U && pt3==="b101".U && pt5==="b00100".U && pt6==="b11".U){    // srli   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt0==="b0000000".U && pt3==="b101".U && pt5==="b00110".U && pt6==="b11".U){    // srliw  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt3==="b100".U && pt5==="b00100".U && pt6==="b11".U){    // xori   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1^src2// is the symbol right?
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b11001".U && pt6==="b11".U){    // jalr   
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  pc:=src1+src2
  regs.wdata:=pc+4
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b110".U && pt5==="b00100".U && pt6==="b11".U){    // ori    
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
  regs.wdata:=src1|src2
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b011".U && pt5==="b11100".U && pt6==="b11".U){    // csrrc  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt3==="b111".U && pt5==="b11100".U && pt6==="b11".U){    // csrrci 
}.elsewhen(pt3==="b010".U && pt5==="b11100".U && pt6==="b11".U){    // csrrs  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt3==="b110".U && pt5==="b11100".U && pt6==="b11".U){    // csrrsi 
}.elsewhen(pt3==="b001".U && pt5==="b11100".U && pt6==="b11".U){    // csrrw  
  regs.raddr1:=rs1
  src1:=regs.rdata1
  src2:=immI
}.elsewhen(pt3==="b101".U && pt5==="b11100".U && pt6==="b11".U){    // csrrwi 
}.elsewhen(pt0==="b0011000".U && pt1==="b00010".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // mret   
}.elsewhen(pt5==="b00101".U && pt6==="b11".U){    // auipc  
  src1:=immU
  regs.wdata:=src1+pc
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt5==="b01101".U && pt6==="b11".U){    // lui    
  src1:=immU
  regs.wdata:=src1
  regs.waddr:=dest
  regs.wen:=1.U
}.elsewhen(pt3==="b000".U && pt5==="b11000".U && pt6==="b11".U){    // beq    
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  dpc:=Mux((src1.S===src2.S),pc,pc+dest)
}.elsewhen(pt3==="b101".U && pt5==="b11000".U && pt6==="b11".U){    // bge    
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  pc:=Mux((src1.S>=src2.S),pc,pc+dest)
}.elsewhen(pt3==="b111".U && pt5==="b11000".U && pt6==="b11".U){    // bgeu   
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  pc:=Mux((src1.U>=src2.U),pc,pc+dest)
}.elsewhen(pt3==="b100".U && pt5==="b11000".U && pt6==="b11".U){    // blt    
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  dpc:=Mux((src1.U<src2.U),pc,pc+dest)
}.elsewhen(pt3==="b110".U && pt5==="b11000".U && pt6==="b11".U){    // bltu   
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
}.elsewhen(pt3==="b001".U && pt5==="b11000".U && pt6==="b11".U){    // bne    
  dest:=immB
  regs.raddr1:=rs1
  regs.raddr2:=rs2
  src1:=regs.rdata1
  src2:=regs.rdata2
  dpc:=Mux((src1.U=/=src2.U),pc,pc+dest)
}.elsewhen(pt5==="b11011".U && pt6==="b11".U){    // jal    
  src1:=immJ
  regs.wdata:=pc+4
  regs.waddr:=dest
  regs.wen:=1.U
  dpc:=pc+src1
}.elsewhen(pt0==="b0000000".U && pt1==="b00000".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // ecall  
}.elsewhen(pt0==="b0000000".U && pt1==="b00001".U && pt2==="b00000".U && pt3==="b000".U && pt4==="b00000".U && pt5==="b11100".U && pt6==="b11".U){    // ebreak 
  stopflag1:=1.U
}.otherwise{    // inv 
  stopflag:=1.U
}
  //update pc reg
  pc:=Mux(stopflag===1.U,dnpc,pc)
}
