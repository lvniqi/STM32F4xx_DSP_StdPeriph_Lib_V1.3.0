library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity CNT16A is 
port(clk,clr: in std_logic;
     dav:out std_logic);
end CNT16A;

architecture one of CNT16A is
signal q:std_logic_vector(7 downto 0);
begin 
process(clk,clr)
begin
  if(clk'event and clk='1') then
      if(clr='1') then
         q<="00000000";
		elsif(q>"01000000") then
			q<="00000000";
      else 
         q<=q+1;
      end if;
  end if;
end process;
process(q)
begin
  if(q>"01000000") then
      dav<='0';
   else
      dav<='1';
   end if;
end process;
end one;
