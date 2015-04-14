library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity CNT4A is
port(clk:in std_logic;
      en:in std_logic;
       q:buffer std_logic_vector(1 downto 0));
end CNT4A;

architecture one of CNT4A is
begin
process(clk,en)
begin
  if(clk'event and clk='0') then
     if(en='1') then
       q<=q+1;
     end if;
  end if;
end process;
end one;