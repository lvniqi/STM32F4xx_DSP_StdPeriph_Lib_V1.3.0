library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity ENCODE is
port(I0,I1,I2,I3:in bit;
     y0,y1,ka:out bit);
end ENCODE;

architecture one of ENCODE is
begin 
y1<=(I0 and I1 and (not I2)) or (I0 and I1 and (not I3));
y0<=(I0 and (not I1)) or (I0 and I2 and (not I3));
ka<=I0 and I1 and I2 and I3;
end one;