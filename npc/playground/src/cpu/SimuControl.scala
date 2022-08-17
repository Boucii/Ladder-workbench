package cpu 
import chisel3._
import chisel3.experimental._
class SimuControl extends BlackBox{
  val io=IO(new Bundle{
    val EndOfSimulation=Input(Bool())
  })
}
class UseSimuControl extends Module{
  val io=IO(new Bundle{
    val EndOfSimulationIn=Input(Bool())
  })
  val control=Module(new SimuControl)
  control.io.EndOfSimulation:=io.EndOfSimulationIn
}
