library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity DLATCH8 is
port(clk:in std_logic;
     d: in std_logic_vector(7 downto 0);
     q: out std_logic_vector(7 downto 0));
end DLATCH8;

architecture one of DLATCH8 is
begin
process(clk)
begin
if(clk='0') then q<=d;
end if;
end process;
end one;