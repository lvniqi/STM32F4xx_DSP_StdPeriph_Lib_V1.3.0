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
						 output reg BUF1,BUF2,BUF3,
						 output reg read_en);
	always @(*)begin
		case (ADDR[18:15])
			{4'b1010}:begin 
				BUF1 <= 1;
				BUF2 <= 0;
				BUF3 <= 0;
				read_en <= 1;
				end
			{4'b1110}:begin
				BUF1 <= 0;
				BUF2 <= 1;
				BUF3 <= 0;
				end
			{4'b1111}:begin
				BUF1 <= 0;
				BUF2 <= 0;
				BUF3 <= 1;
				end
			default:begin
				BUF1 <= 0;
				BUF2 <= 0;
				BUF3 <= 0;
				read_en <= 0;
				end
		endcase
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
