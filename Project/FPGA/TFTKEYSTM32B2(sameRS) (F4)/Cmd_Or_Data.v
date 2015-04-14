module CMD_DATA_SELECT(input En,input [15:0] DATA_IN,
								output reg RS,output reg[15:0]  DATA_OUT );
		
		always @(En == 0) begin
			//如果为地址
			if(~DATA_IN[15]) begin
				RS <= 0;
				DATA_OUT <= DATA_IN;
				end
			//如果为数据
			else begin
				RS <= 1;
				//如果不为颜色
				if(~DATA_IN[14]) begin
					DATA_OUT <= {1'b0,DATA_IN[14:0]};
					end 
				//如果为颜色
				else begin 
					DATA_OUT <= {DATA_IN[13:10],DATA_IN[10:5],DATA_IN[5:0]};
					end
				end
			end
		always @(En) begin
			DATA_OUT <= 16'bz;
			RS <= 1'bz;
			end
endmodule 