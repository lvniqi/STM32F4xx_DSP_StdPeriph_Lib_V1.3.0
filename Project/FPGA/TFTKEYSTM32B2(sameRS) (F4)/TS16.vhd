library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity TS16 is
port(en:in std_logic;
     di:in std_logic_vector(15 downto 0);
     do:out std_logic_vector(15 downto 0));
end TS16;

architecture one of TS16 is
begin
process(en,di)
begin
  if(en='0') then
      do<=di;
   else
      do<="ZZZZZZZZZZZZZZZZ";
   end if;
end process;
end one;
