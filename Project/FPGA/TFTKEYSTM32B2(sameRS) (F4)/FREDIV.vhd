library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity FREDIV is
port(clkin: in std_logic;
      clkout: out std_logic);
end FREDIV;

architecture one of FREDIV is
signal q:std_logic_vector(9 downto 0);
begin
process(clkin)
begin
  if(clkin'event and clkin='1') then
     if(q="1111111111") then
        q<="0000000000";
     else 
        q<=q+1;
     end if;
  end if;
end process;
clkout<=q(9);
end one;