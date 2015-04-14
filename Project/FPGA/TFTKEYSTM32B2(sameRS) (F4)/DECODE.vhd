library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity DECODE is
port(a:in std_logic_vector(1 downto 0);
     y:out std_logic_vector(3 downto 0));
end DECODE;

architecture one of DECODE is
begin
y(0)<='0' when a="00" else '1';
y(1)<='0' when a="01" else '1';
y(2)<='0' when a="10" else '1';
y(3)<='0' when a="11" else '1';
end one;
