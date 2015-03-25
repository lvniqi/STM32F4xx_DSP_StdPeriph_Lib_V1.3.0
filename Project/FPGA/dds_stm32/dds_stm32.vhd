-- Octal D-Type Register with 3-State Outputs
-- Simple model of an Octal D-type register with three-state outputs using two concurrent statements.
-- download from: www.fpga.com.cn & www.pld.com.cn

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

ENTITY FrewSave_16 IS
   PORT(clock, oebar : IN std_logic; 
        data : IN std_logic_vector(15 DOWNTO 0);
        qout : OUT std_logic_vector(15 DOWNTO 0));
END ENTITY FrewSave_16;

ARCHITECTURE one OF FrewSave_16 IS
   --internal flip-flop outputs
   SIGNAL qint : std_logic_vector(15 DOWNTO 0);
BEGIN
   qint <= data WHEN rising_edge(clock); --d-type flip flops
   qout <= "0000000100000000"; --three-state buffers
END ARCHITECTURE one;
