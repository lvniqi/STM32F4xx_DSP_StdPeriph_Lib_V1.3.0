transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8 {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/FPGA2MCU.v}
vlog -vlog01compat -work work +incdir+D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8 {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/FIFO_ADIN.v}
vlog -vlog01compat -work work +incdir+D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8 {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/state_wrrqe.v}
vcom -93 -work work {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/phase_addr.vhd}
vcom -93 -work work {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/cylon_1_ran.vhd}
vcom -93 -work work {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/pll_2.vhd}

vlog -vlog01compat -work work +incdir+D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/simulation/modelsim {D:/study/STM32F4xx_DSP_StdPeriph_Lib_V1.3.0/Project/FPGA/MCU2DDS+DAC(EP1C6T144c8/simulation/modelsim/FPGA_EP2C.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclone_ver -L rtl_work -L work -voptargs="+acc"  FPGA_EP2C_vlg_tst

add wave *
view structure
view signals
run -all
