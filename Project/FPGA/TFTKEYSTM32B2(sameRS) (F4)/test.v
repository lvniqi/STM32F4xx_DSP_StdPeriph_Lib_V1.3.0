module TEST(input nadv,input da2,input da1,input a16,
				output reg tft_cs,output reg rst_cs,output reg key_cs);
wire[1:0] PIN;
assign PIN={da2,da1};
always @(posedge nadv) begin
	if(a16 == 0) begin
		case(PIN)
			2'b00: begin
				tft_cs<= 0;
				rst_cs<= 1;
				key_cs<= 1;
				end
			2'b10: begin
				tft_cs<= 1;
				rst_cs<= 0;
				key_cs<= 1;
				end
			2'b11: begin
				tft_cs<= 1;
				rst_cs<= 1;
				key_cs<= 0;
				end
			default:begin
				tft_cs<= 1;
				rst_cs<= 1;
				key_cs<= 1;
				end
			endcase	
		end
	else begin
		tft_cs<= 1;
		rst_cs<= 1;
		key_cs<= 1;
		end
	end
endmodule 