module AD9854(input en,input write,input [15:0] DATA_IN,
					output [5:0] ADDR,output [7:0] DATA_out,output  WR);
	assign WR = (en == 1'b1)?write:1'b1;
	assign DATA_out = DATA_IN[7:0];
	assign ADDR = DATA_IN[13:8];
endmodule 

module AD9854_CON(input en,input write,input [15:0] DATA_IN,
				  output UCLK,output FSK,output RST);
	reg [2:0]Dat;
	assign UCLK = Dat[2];
	assign FSK = Dat[1];
	assign RST = Dat[0];
	always@(posedge write)begin
		if(en)begin
			Dat = DATA_IN[2:0];
			end
		end
endmodule 