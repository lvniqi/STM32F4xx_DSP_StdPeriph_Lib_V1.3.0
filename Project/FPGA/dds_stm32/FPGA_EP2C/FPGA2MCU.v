/*
* 保存MCU地址
* module SAVE_ADDR
* */
module SAVE_ADDR(input NADV,input [15:0] AD_IN,
					  input A16,input A17,input A18,
					  output reg[18:0] ADDR);
	wire NADV2 = NADV;
	always@(posedge NADV or negedge NADV2)begin
		if(NADV == 1)
			ADDR <= {A18,A17,A16,AD_IN};
		if(NADV2 == 0)
			ADDR <= 19'b0;
		end
endmodule
/*
* 根据地址选择器件
* module SELECT_ADDR
* */
module SELECT_ADDR(input [18:0] ADDR,
						 output reg BUF1);
	always @(*)begin
		if(ADDR == {3'b101,15'b0,1'b1})
			BUF1 <= 1;
		else
			BUF1 <= 0;
		end
endmodule
/*
* 缓冲器发送数据
* module BUFF_SEND_DATA
* */
module BUFF_SEND_DATA(input[15:0] DATA_IN,input READ,
					  output reg[15:0] DATA_OUT,output reg FINISH);
	wire READ_2 = READ;
	always@(negedge READ_2 or posedge READ)begin
		if(READ_2 == 0)
			DATA_OUT <= DATA_IN;
		if(READ == 1)
			DATA_OUT <= 16'bz;
			FINISH <= !FINISH;
		end
endmodule
/*
* 缓冲器
* module BUFF
* */
module BUFF(input en,input write,input [15:0] DATA_IN,
				//output reg[15:0] DATA_OUT,output reg flag);
				output reg[15:0] DATA_OUT,output reg FINISH);
	wire en2 = en;
	reg[15:0] DATA_BUF;
	always@(posedge write)begin
		if(en == 1)
			DATA_BUF <= DATA_IN;
			FINISH <= !FINISH;
			//flag <= !flag;
		end
	always@(posedge en or negedge en2)begin
		if(en == 1)
			DATA_OUT <= DATA_BUF;
		if(en2 == 0)
			DATA_OUT <= 16'bz;
		end
endmodule 
