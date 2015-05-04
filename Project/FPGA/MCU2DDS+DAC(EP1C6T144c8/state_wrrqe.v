module ADC_FIFO( 
		  input clk,
        input rd_empty,
		  input rst_n,
		  input wr_full,
		  output reg rd_enable=0,
		  output reg wr_enable=1
		  );
   //define state
   parameter [1:0] IDLE=2'b00,START=2'b01;
   reg [1:0] current_state=IDLE;
	reg [1:0] next_state=IDLE;
	//状态转换
	always@(posedge clk or negedge rst_n)begin
		if(!rst_n)
			current_state<= IDLE;
		else
			current_state<= next_state;
		end
	//第二个进程，组合逻辑always模块，描述状态转移条件判断
	always@(current_state or rd_empty or wr_full or rst_n)begin
		next_state = IDLE;
		if(!rst_n)
			next_state = IDLE;
		else begin
			case(current_state)
				//空闲状态
				IDLE:begin
					//外部使能 进入发送
					if(wr_full)begin
						next_state = START;
						end
					else
						next_state = IDLE;
					end
				START:begin
					//正在发送，进入下一状态
					if(rd_empty)begin
						next_state = IDLE;
						end
					else
						next_state = START;
					end
				default:next_state = IDLE;
				endcase
			end
		end
	//第三个进程，同步时序always模块，格式化描述次态寄存器输出
	always@(posedge clk or negedge rst_n)begin
		if(!rst_n)begin
			wr_enable <= 1;
			rd_enable <= 0;
			end
		else begin
			case(next_state)
				IDLE: begin
					wr_enable <= 1;
					rd_enable <= 0;
					end
				START:begin
					wr_enable <= 0;
					rd_enable <= 1;
					end
				default:begin
					wr_enable <= 0;
					rd_enable <= 1;
					end
				endcase
			end
		end
	endmodule 

`define SPI_LEN 16
module SPI_OUT( 
		  input clk,
        input [`SPI_LEN-1:0]data_in,
		  input rst_n,
		  input en,
		  output reg sclk,
		  output reg dout
		  );
   //define state
   parameter [1:0] IDLE=2'b00,SEND=2'b01,SEND_n = 2'b10,END = 2'b11;
   reg [1:0] current_state=IDLE;
	reg [1:0] next_state=IDLE;
	reg [3:0] counter=`SPI_LEN;
	reg [`SPI_LEN-1:0] data_in_save;
	//状态转换
	always@(posedge clk or negedge rst_n)begin
		if(!rst_n)
			current_state<= IDLE;
		else
			current_state<= next_state;
		end
	//第二个进程，组合逻辑always模块，描述状态转移条件判断
	always@(current_state or sclk  or rst_n or en)begin
		next_state = IDLE;
		if(!rst_n)
			next_state = IDLE;
		else begin
			case(current_state)
				//空闲状态
				IDLE:begin
					//外部使能 进入发送
					if(en)begin
						next_state = SEND;
						end
					else
						next_state = IDLE;
					end
				SEND:begin
					if(!sclk)//正在发送，进入下一状态
						next_state = SEND_n;
					else
						next_state = SEND;
					end
				SEND_n:begin
					if(counter==0)
						next_state=IDLE;
					else if(sclk)
						next_state = SEND;
					else
						next_state = SEND_n;
					end
				default:next_state = IDLE;
				endcase
			end
		end
	//第三个进程，同步时序always模块，格式化描述次态寄存器输出
	always@(posedge clk or negedge rst_n)begin
		if(!rst_n)begin
			end
		else begin
			case(next_state)
				IDLE: begin
					data_in_save <= data_in;
					counter <=`SPI_LEN-1;
					sclk <= 1'b1;
					end
				SEND:begin
					dout <= data_in_save[counter];
					sclk <= 1'b0;
					end
				SEND_n:begin
					sclk <= 1'b1;
					counter <= counter-1;
					end
				default:begin
					end
				endcase
			end
		end
	endmodule 