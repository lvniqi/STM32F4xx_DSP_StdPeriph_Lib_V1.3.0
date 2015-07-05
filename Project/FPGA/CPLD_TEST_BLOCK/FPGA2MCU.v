/*
* 保存MCU地址
* module SAVE_ADDR
* */
module SAVE_ADDR(input NADV,input [15:0] AD_IN,
					  input A16,input A17,input A18,
					  output reg[18:0] ADDR);
	always@(posedge NADV)begin
			ADDR <= {A18,A17,A16,AD_IN};
		end
endmodule
/*
* 根据地址选择器件
* module SELECT_ADDR
* */
module SELECT_ADDR(input [18:0] ADDR,
						 output reg BUFF,
						 output reg read_en);
	always @(*)begin
		if(ADDR[18:15] == {4'b1010})begin
				BUFF <= 1;
				read_en <= 1;
				end
		else begin
			BUFF <= 0;
			read_en <= 0;
			end
		end
endmodule
/*
* 缓冲器发送数据
* module BUFF_SEND_DATA
* */
module BUFF_SEND_DATA(input[15:0] DATA_IN,input READ,input en,
					  output reg[15:0] DATA_OUT,output reg FINISH);
	wire READ_2 = READ;
	always@(negedge READ_2 or posedge READ)begin
			if(READ_2 == 0)
				if(en)begin
					DATA_OUT <= DATA_IN;
					end
			if(READ == 1)begin
				DATA_OUT <= 16'bz;
				FINISH <= !FINISH;
				end
		end
endmodule
/*
* 缓冲器
* module BUFF
* */
module BUFF(input en,input write,input [15:0] DATA_IN,
				output reg[15:0] DATA_OUT,output reg FINISH);
	wire en2 = en;
	always@(posedge write)begin
		if(en == 1)
			DATA_OUT <= DATA_IN;
			FINISH <= !FINISH;
			//flag <= !flag;
		end
endmodule 

module TEST_Block(input clk,output reg[23:0] oclk);
	integer i;
	always@(*)begin
		for(i=0;i<24;i=i+1)begin
			oclk[i] = clk;
			end
		end
endmodule 