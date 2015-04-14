library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity REG4 is
port(clk:in std_logic;
     d:in std_logic_vector(3 downto 0);
     q:out std_logic_vector(3 downto 0));
end REG4;

architecture one of REG4 is
signal q_temp:std_logic_vector(3 downto 0);
begin
process(clk)
begin
if(clk'event and clk='0') then
    q_temp<=d;
end if;
end process;
q<=q_temp;
end one;